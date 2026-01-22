//
// Created by liad on 1/12/26.
//
#include <RegenXIO.hpp>

#include <filesystem>
#include <fstream>

// LATER: Return here when RegenX has some more meat to its bone I feel like it can be improved
namespace regenx::io {
    bool file_exist(const std::string& path) {
        return std::filesystem::exists(path);
    }

    bool file_create(const std::string& path, const std::string& contents) {
        if (!file_exist(path)) {
            std::ofstream file(path);
            if (!file.is_open()) {
                return false;
            }
            file << contents;
            file.close();
            return true;
        }
        return false;
    }

    bool file_write(const std::string& path, const std::string& contents) {
        if (file_exist(path)) {
            std::ofstream file(path, std::ios::app);
            if (!file.is_open()) {
                return false;
            }
            file << contents << std::endl;
            file.close();
            return true;
        }
        return false;
    }

    std::string file_read(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            return "";
        }
        std::string text;
        std::string content;
        while (getline(file, text)) {
            content += (text + '\n');
        }
        file.close();
        return content;
    }

    std::string file_read_line(const std::string& path, const int line) {
        std::ifstream file(path);
        if (!file.is_open()) {
            return "";
        }
        std::string text;
        std::string content;
        int i = 0;
        while (getline(file, text)) {
            content = (text + '\n');
            if (i == line) break;
            i++;
        }
        file.close();
        return content;
    }

    bool file_remove(const std::string& path) {
        if (file_exist(path)) {
            return std::filesystem::remove(path);
        }
        return false;
    }
}
