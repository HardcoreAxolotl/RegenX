#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <utility>
#include <sstream>

namespace filesystem = std::filesystem;

bool file_exists(const std::string& name) {
    return (filesystem::exists(name));
}

void file_delete(const std::string& path) {
    if (std::remove(path.c_str()) == 0) {
        std::cout << "File successfully deleted: " << path << std::endl;
    } else {
        std::cerr << "Error deleting file: " << path << std::endl;
    }
}

void file_create(const std::string& path, const std::string& content = "") {
    std::ofstream file(path, std::ios::__noreplace);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << path << std::endl;
        return;
    }
    file << content;
    file.close();
}

std::string file_read_line(const std::string& file_path, const int line) {
    int cline = 0;
    std::string text;
    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << file_path << std::endl;
        return ""; // Return an empty string or handle the error as needed
    }

    while (std::getline(file, text)) {
        if (cline == line) {
            return text;
        }
        ++cline;
    }
    file.close();
    return "";
}

std::string file_read(const std::string& file_path) {
    std::stringstream content;
    std::string text;
    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << file_path << std::endl;
        return ""; // Return an empty string or handle the error as needed
    }

    content << file.rdbuf();

    file.close();
    return content.str();
}

void file_overwrite(const std::string& file_path, const std::string& content) {
    std::ofstream file(file_path);
    file << content;
    file.close();
}

void file_write(const std::string& file_path, const std::string& content) {
    std::ofstream file(file_path, std::ios::app);
    if (file.is_open()) {
        file << content;
    } else {
        std::cerr << "Error: Could not open " << file_path << std::endl;
    }
    file.close();
}

using namespace std;

int main() {
    const string content = R"(=== RegenXIO TEXT OUTPUT TEST ===

If you are reading this file, ALL of the following are working:

1. File creation
2. File write access
3. Text encoding (ASCII)
4. Line breaks (\n)
5. File close / flush

Subsystem : RegenXIO
Operation : file_create + write
Result    : SUCCESS

This file was generated intentionally.
It is NOT game data.

End of test.
)";
    const auto *test = new file("test.txt", content);
    cout << test->read() << endl;
    cout << "==================================" << endl;
    test->overwrite("hello ");
    test->write("world");
    cout << test->read() << endl;
    cout << "==================================" << endl;
    test->overwrite("hello \n");
    test->write("world");
    cout << test->read_line(0) << endl;
    cout << test->read_line(1) << endl;
    delete test;
    return 0;
}
