#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include "RegenX/RegenXIO.h"

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
    file_create("test.txt", content);
    cout << file_read("test.txt") << endl;
    cout << endl << endl << endl;
    file_write("test.txt", "Hello, World!\nGoodbye, World");
    cout << file_read("test.txt") << endl;
    cout << endl << endl << endl;
    cout << file_read_line("test.txt", 0) << endl;
    cout << file_read_line("test.txt", 1) << endl;
    return 0;
}
