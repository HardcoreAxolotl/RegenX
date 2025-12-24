//
// Created by liad on 16/12/2025.
//

#include "RegenXIO.h"
#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <sstream>

namespace filesystem = std::filesystem;

bool file_exists(const std::string& path) {
    return (filesystem::exists(path));
}

void file_delete(const std::string& path) {
    if (std::remove(path.c_str()) == 0) {
        std::cout << "File successfully deleted: " << path << std::endl;
    } else {
        std::cerr << "Error deleting file: " << path << std::endl;
    }
}

void file_create(const std::string& path, const std::string& content) {
    if (!filesystem::exists(path)) {
        std::ofstream file(path, std::ios::__noreplace);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open " << path << std::endl;
            return;
        }
        if (!content.empty()) file << content;
        file.close();
    }
    else {
        std::cerr << "Error: Could not create " << path << std::endl;
    }
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
    if (content.empty()) {
        std::cerr << "Refusing to overwrite with empty content" << std::endl;
        return;
    }

    std::ofstream file(file_path);
    if (!file) {
        std::cerr << "Can't overwrite file: " << file_path << std::endl;
        return;
    }

    file << content;
}

void file_write(const std::string& file_path, const std::string& content) {
    std::ofstream file(file_path, std::ios::app);
    if (file.is_open()) {
        if (content.empty()) {
            std::cerr << "Refusing to write empty content" << std::endl;
            return;
        }
        file << content;
    } else {
        std::cerr << "Error: Could not open " << file_path << std::endl;
    }
    file.close();
}