#pragma once
// RegenXTest.hpp - tiny header-only test runner (C++20)
// Put this in: tests/RegenXTest.hpp

#include <chrono>
#include <cstring>
#include <exception>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace rx_test {

struct TestCase {
    const char* name;
    void (*fn)();
};

inline std::vector<TestCase>& registry() {
    static std::vector<TestCase> tests;
    return tests;
}

struct Registrar {
    Registrar(const char* name, void (*fn)()) {
        registry().push_back(TestCase{name, fn});
    }
};

struct Context {
    int checks = 0;
    int failed_checks = 0;
    const char* current_test = nullptr;

    void fail(const char* expr, const char* file, int line, const char* msg) {
        failed_checks++;
        std::cerr << "  [FAIL] " << (current_test ? current_test : "(unknown test)")
                  << "\n         Expr: " << expr
                  << "\n         At:   " << file << ":" << line;
        if (msg && msg[0] != '\0') std::cerr << "\n         Msg:  " << msg;
        std::cerr << "\n";
    }
};

inline Context& ctx() {
    static Context c;
    return c;
}

inline bool match_filter(const char* test_name, std::string_view filter) {
    if (filter.empty()) return true;
    if (!test_name) return false;
    return std::string_view(test_name).find(filter) != std::string_view::npos;
}

inline void print_help() {
    std::cout
        << "RegenX tests\n"
        << "Usage:\n"
        << "  regenx_tests            Run all tests\n"
        << "  regenx_tests --list     List tests\n"
        << "  regenx_tests --filter=X Run only tests whose name contains X\n"
        << "  regenx_tests --quiet    Less output\n";
}

inline int run_all(int argc, char** argv) {
    std::string_view filter;
    bool list_only = false;
    bool quiet = false;

    for (int i = 1; i < argc; ++i) {
        const char* a = argv[i];
        if (std::strcmp(a, "--help") == 0 || std::strcmp(a, "-h") == 0) {
            print_help();
            return 0;
        } else if (std::strcmp(a, "--list") == 0) {
            list_only = true;
        } else if (std::strcmp(a, "--quiet") == 0) {
            quiet = true;
        } else if (std::strncmp(a, "--filter=", 9) == 0) {
            filter = std::string_view(a + 9);
        } else {
            // Treat unknown arg as filter shortcut: regenx_tests SDL
            filter = std::string_view(a);
        }
    }

    auto& tests = registry();

    if (list_only) {
        for (auto& t : tests) {
            if (match_filter(t.name, filter))
                std::cout << "- " << t.name << "\n";
        }
        return 0;
    }

    int ran = 0;
    int failed_tests = 0;

    for (auto& t : tests) {
        if (!match_filter(t.name, filter)) continue;

        ran++;
        ctx().current_test = t.name;
        int before_failed = ctx().failed_checks;
        int before_checks = ctx().checks;

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

        bool test_failed = (ctx().failed_checks != before_failed);
        if (test_failed) failed_tests++;

        if (!quiet) {
            int did_checks = ctx().checks - before_checks;
            std::cout << (test_failed ? "[FAIL] " : "[ OK ] ")
                      << t.name << "  (" << did_checks << " checks, " << ms << " ms)\n";
        }
    }

    std::cout << "\nSummary: " << ran << " tests, "
              << ctx().checks << " checks, "
              << ctx().failed_checks << " failed checks\n";

    return (failed_tests == 0) ? 0 : 1;
}

} // namespace rx_test

// ---------- Macro helpers ----------
#define RX_JOIN2(a,b) a##b
#define RX_JOIN(a,b) RX_JOIN2(a,b)

// Define a test:
// RX_TEST("name") { ... }
#define RX_TEST(NAME_LITERAL) \
    static void RX_JOIN(rx_test_fn_, __COUNTER__)(); \
    static ::rx_test::Registrar RX_JOIN(rx_test_reg_, __COUNTER__)(NAME_LITERAL, &RX_JOIN(rx_test_fn_, __COUNTER__)); \
    static void RX_JOIN(rx_test_fn_, __COUNTER__)()

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

// One-liner main:
#define RX_TEST_MAIN() \
    int main(int argc, char** argv) { return ::rx_test::run_all(argc, argv); }
