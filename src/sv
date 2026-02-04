#include <iostream>
#include <ostream>
#include <RegenX/RegenXLog.hpp>
#include <RegenX/RegenXIO.hpp>

using namespace regenx;

int main()
{
    // Initialize the logger
    //log::init();

    log::info("Creating file");
    io::file_create("test.log");
    std::cout << std::endl;

    log::info("Writing to file");
    io::file_write("test.log", "Testing something 1");
    std::cout << std::endl;

    log::info("Reading file");
    std::string contents = io::file_read("test.log");
    log::info("File contents: " + contents);
    std::cout << std::endl;

    log::info("Overwriting file");
    io::file_overwrite("test.log", "Testing something 2");
    std::cout << std::endl;

    log::info("Appending a line to file");
    io::file_write_line("test.log", 1, "Random Text");
    std::cout << std::endl;

    log::info("Reading entire file again");
    contents = io::file_read("test.log");
    log::info("File contents: " + contents);
    std::cout << std::endl;

    log::info("Reading first line of file");
    std::string first_line = io::file_read_line("test.log", 0);
    log::info("First line: " + first_line);
    std::cout << std::endl;

    log::info("Checking if file exists 1");
    bool exists = io::file_exist("test.log");
    log::info(std::string("File exists: ") + (exists ? "true" : "false"));
    std::cout << std::endl;

    io::file_remove("test.log");
    log::info("Checking if file exists 2");
    exists = io::file_exist("test.log");
    log::info(std::string("File exists: ") + (exists ? "true" : "false"));


    return 0;
}
