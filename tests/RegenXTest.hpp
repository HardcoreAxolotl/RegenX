#pragma once
// RegenXTest.hpp - tiny header-only test runner (C++20)
// Put this in: tests/RegenXTest.hpp

#include <chrono>
#include <cstring>
#include <exception>
#include <iostream>
#include <string_view>
#include <vector>

namespace rx_test {

// -----------------------------
// Test metadata
// -----------------------------
enum TestFlags : unsigned {
    RXF_NONE    = 0u,
    RXF_LIBRARY = 1u << 0, // mark tests that specifically validate third-party/system libraries
};

struct TestCase {
    const char* name;
    void (*fn)();
    unsigned flags;
};

inline std::vector<TestCase>& registry() {
    static std::vector<TestCase> tests;
    return tests;
}

struct Registrar {
    Registrar(const char* name, void (*fn)(), unsigned flags = RXF_NONE) {
        registry().push_back(TestCase{name, fn, flags});
    }
};

// -----------------------------
// Context (shared by all tests)
// -----------------------------
struct Context {
    int checks = 0;
    int failed_checks = 0;
    int skipped_tests = 0;

    const char* current_test = nullptr;
    bool current_skipped = false;

    void reset() {
        checks = 0;
        failed_checks = 0;
        skipped_tests = 0;
        current_test = nullptr;
        current_skipped = false;
    }

    void fail(const char* expr, const char* file, int line, const char* msg) {
        failed_checks++;
        std::cerr << "  [FAIL] " << (current_test ? current_test : "(unknown test)")
                  << "\n         Expr: " << expr
                  << "\n         At:   " << file << ":" << line;
        if (msg && msg[0] != '\0') std::cerr << "\n         Msg:  " << msg;
        std::cerr << "\n";
    }

    void skip(const char* file, int line, const char* msg) {
        current_skipped = true;
        std::cerr << "  [SKIP] " << (current_test ? current_test : "(unknown test)")
                  << "\n         At:   " << file << ":" << line;
        if (msg && msg[0] != '\0') std::cerr << "\n         Msg:  " << msg;
        std::cerr << "\n";
    }
};

inline Context& ctx() {
    static Context c;
    return c;
}

// -----------------------------
// Utilities
// -----------------------------
inline bool match_filter(const char* test_name, std::string_view filter) {
    if (filter.empty()) return true;
    if (!test_name) return false;
    return std::string_view(test_name).find(filter) != std::string_view::npos;
}

inline void print_help() {
    std::cout
        << "RegenX tests\n"
        << "Usage:\n"
        << "  regenx_tests                Run all tests\n"
        << "  regenx_tests --list         List tests\n"
        << "  regenx_tests --filter=X     Run only tests whose name contains X\n"
        << "  regenx_tests --libs         Run only library tests\n"
        << "  regenx_tests --quiet        Less output\n";
}

struct RunOptions {
    std::string_view filter;
    bool list_only = false;
    bool quiet = false;
    bool libs_only = false;
    bool print_summary = true;
    bool reset_context = true;
};

inline int run_with_options(const RunOptions& opt) {
    if (opt.reset_context) ctx().reset();

    auto& tests = registry();

    if (opt.list_only) {
        for (auto& t : tests) {
            if (opt.libs_only && (t.flags & RXF_LIBRARY) == 0u) continue;
            if (match_filter(t.name, opt.filter)) std::cout << "- " << t.name << "\n";
        }
        return 0;
    }

    int ran = 0;
    int failed_tests = 0;

    for (auto& t : tests) {
        if (opt.libs_only && (t.flags & RXF_LIBRARY) == 0u) continue;
        if (!match_filter(t.name, opt.filter)) continue;

        ran++;
        ctx().current_test = t.name;
        ctx().current_skipped = false;

        const int before_failed = ctx().failed_checks;
        const int before_checks = ctx().checks;

        auto start = std::chrono::high_resolution_clock::now();

        try {
            t.fn();
        } catch (const std::exception& e) {
            ctx().fail("exception", __FILE__, __LINE__, e.what());
        } catch (...) {
            ctx().fail("unknown exception", __FILE__, __LINE__, "");
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        const bool test_failed = (ctx().failed_checks != before_failed);
        const int did_checks = ctx().checks - before_checks;

        if (ctx().current_skipped) {
            ctx().skipped_tests++;
            if (!opt.quiet) {
                std::cout << "[SKIP] " << t.name << "  (" << did_checks << " checks, " << ms << " ms)\n";
            }
            continue;
        }

        if (test_failed) failed_tests++;

        if (!opt.quiet) {
            std::cout << (test_failed ? "[FAIL] " : "[ OK ] ")
                      << t.name << "  (" << did_checks << " checks, " << ms << " ms)\n";
        }
    }

    if (opt.print_summary) {
        std::cout << "\nSummary: " << ran << " tests, "
                  << ctx().skipped_tests << " skipped, "
                  << ctx().checks << " checks, "
                  << ctx().failed_checks << " failed checks\n";
    }

    return (failed_tests == 0) ? 0 : 1;
}

// Normal CLI entry (what you already had) + adds --libs
inline int run_all(int argc, char** argv) {
    RunOptions opt;

    for (int i = 1; i < argc; ++i) {
        const char* a = argv[i];
        if (std::strcmp(a, "--help") == 0 || std::strcmp(a, "-h") == 0) {
            print_help();
            return 0;
        } else if (std::strcmp(a, "--list") == 0) {
            opt.list_only = true;
        } else if (std::strcmp(a, "--quiet") == 0) {
            opt.quiet = true;
        } else if (std::strcmp(a, "--libs") == 0) {
            opt.libs_only = true;
        } else if (std::strncmp(a, "--filter=", 9) == 0) {
            opt.filter = std::string_view(a + 9);
        } else {
            // Treat unknown arg as filter shortcut: regenx_tests SDL
            opt.filter = std::string_view(a);
        }
    }

    return run_with_options(opt);
}

// -----------------------------
// The SINGLE function you wanted:
// call this from your code or from one test to validate *all libraries*.
// It runs ONLY tests registered with RX_LIB_TEST.
// -----------------------------
inline bool run_all_libraries(bool quiet = false, std::string_view filter = {}) {
    RunOptions opt;
    opt.quiet = quiet;
    opt.libs_only = true;
    opt.filter = filter;
    opt.print_summary = true;
    opt.reset_context = true;

    return run_with_options(opt) == 0;
}

// Optional: register one test that calls the function above.
// Useful when you want "AllLibs" to appear as ONE test in the output.
inline void test_all_libraries() {
    // Run library tests, but don't print the per-test lines twice.
    RunOptions opt;
    opt.quiet = true;
    opt.libs_only = true;
    opt.print_summary = false;
    opt.reset_context = false; // keep counts under the outer run

    const int before_failed = ctx().failed_checks;
    (void)run_with_options(opt);

    // Make this wrapper test fail if any library test failed
    if (ctx().failed_checks != before_failed) {
        ctx().fail("All library tests", __FILE__, __LINE__, "One or more library tests failed");
    }
}

} // namespace rx_test

// ---------- Macro helpers ----------
#define RX_JOIN2(a,b) a##b
#define RX_JOIN(a,b) RX_JOIN2(a,b)

// Fix for the original macro bug: use __COUNTER__ only once.
#define RX_TEST_IMPL(NAME_LITERAL, ID) \
    static void RX_JOIN(rx_test_fn_, ID)(); \
    static ::rx_test::Registrar RX_JOIN(rx_test_reg_, ID)(NAME_LITERAL, &RX_JOIN(rx_test_fn_, ID), ::rx_test::RXF_NONE); \
    static void RX_JOIN(rx_test_fn_, ID)()

#define RX_TEST(NAME_LITERAL) RX_TEST_IMPL(NAME_LITERAL, __COUNTER__)

// Library test: included in run_all_libraries() and in CLI via --libs
#define RX_LIB_TEST_IMPL(NAME_LITERAL, ID) \
    static void RX_JOIN(rx_test_lib_fn_, ID)(); \
    static ::rx_test::Registrar RX_JOIN(rx_test_lib_reg_, ID)(NAME_LITERAL, &RX_JOIN(rx_test_lib_fn_, ID), ::rx_test::RXF_LIBRARY); \
    static void RX_JOIN(rx_test_lib_fn_, ID)()

#define RX_LIB_TEST(NAME_LITERAL) RX_LIB_TEST_IMPL(NAME_LITERAL, __COUNTER__)

// Register one "meta test" that runs all library tests
#define RX_TEST_ALL_LIBS() \
    RX_TEST("AllLibraries") { ::rx_test::test_all_libraries(); }

// Check macros
#define RX_CHECK(EXPR) do { \
    ::rx_test::ctx().checks++; \
    if (!(EXPR)) ::rx_test::ctx().fail(#EXPR, __FILE__, __LINE__, ""); \
} while(0)

#define RX_CHECK_MSG(EXPR, MSG) do { \
    ::rx_test::ctx().checks++; \
    if (!(EXPR)) ::rx_test::ctx().fail(#EXPR, __FILE__, __LINE__, (MSG)); \
} while(0)

#define RX_REQUIRE(EXPR) do { \
    ::rx_test::ctx().checks++; \
    if (!(EXPR)) { ::rx_test::ctx().fail(#EXPR, __FILE__, __LINE__, "REQUIRE"); return; } \
} while(0)

#define RX_SKIP(MSG) do { \
    ::rx_test::ctx().skip(__FILE__, __LINE__, (MSG)); \
    return; \
} while(0)

// One-liner main:
#define RX_TEST_MAIN() \
    int main(int argc, char** argv) { return ::rx_test::run_all(argc, argv); }
