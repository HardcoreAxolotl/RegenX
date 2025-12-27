//
// Created by liad on 16/12/2025.
//

#include "IO.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <lauxlib.h>
#include <sstream>
#include "Log.hpp"

namespace filesystem = std::filesystem;

// C++
bool file_exists(const std::string& file_path) {
    return (filesystem::exists(file_path));
}

bool file_delete(const std::string& file_path) {
    if (std::remove(file_path.c_str()) == 0) {
        log(file_path + " was successfully deleted");
        return true;
    } else {
        error("Error deleting file: " + file_path);
        return false;
    }
}

bool file_create(const std::string& file_path, const std::string& content) {
    if (!filesystem::exists(file_path)) {
        std::ofstream file(file_path, std::ios::__noreplace);
        if (!file.is_open()) {
            error("Could not create " + file_path);
            return false;
        }
        if (!content.empty()) file << content;
        file.close();
        return true;
    }
    else {
        error("Could not create " + file_path);
        return false;
    }
}

std::string file_read_line(const std::string& file_path, const int line) {
    int cline = 0;
    std::string text;
    std::ifstream file(file_path);
    if (!file.is_open()) {
        error("Error: Could not open " + file_path);
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
        error("Error: Could not open " + file_path);
        return ""; // Return an empty string or handle the error as needed
    }

    content << file.rdbuf();

    file.close();
    return content.str();
}

bool file_overwrite(const std::string& file_path, const std::string& content) {
    if (content.empty()) {
        error("Refusing to overwrite with empty content" );
        return false;
    }

    std::ofstream file(file_path);
    if (!file) {
        error("Can't overwrite file" + file_path);
        return false;
    }

    file << content;
    file.close();
    return true;
}

bool file_write(const std::string& file_path, const std::string& content) {
    std::ofstream file(file_path, std::ios::app);
    if (file.is_open()) {
        if (content.empty()) {
            error("Refusing to write empty content" );
            return false;
        }
        file << content;
    } else {
        error("Error: Could not open " + file_path);
        return false;
    }
    file.close();
    return true;
}

