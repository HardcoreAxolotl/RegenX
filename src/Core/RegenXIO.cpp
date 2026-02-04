//
// Created by liad on 1/12/26.
//
#include <RegenX/Core/RegenXIO.hpp>
#include <RegenX/Core/RegenXLog.hpp>

#include <filesystem>
#include <fstream>
#include <vector>

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
            log::info("Created \"" + path + "\"");
            return true;
        }
        log::error("Failed to create \"" + path + "\", file already exist");
        return false;
    }

    bool file_write(const std::string& path, const std::string& contents) {
        if (file_exist(path)) {
            std::ofstream file(path, std::ios::app);
            if (!file.is_open()) {
                log::error("Failed to open \"" + path + "\"");
                return false;
            }
            file << contents << std::endl;
            file.close();
            log::info("Written to \"" + path + "\"");
            return true;
        }
        log::error("Failed to write to \"" + path + "\", file doesnt exist");
        return false;
    }

    std::string file_read(const std::string& path) {
        if (file_exist(path)) {
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
            log::info("Read from \"" + path + "\"");
            return content;
        }
        log::error("Failed to read \"" + path + "\", file doesn't exist");
        return "";
    }

    std::string file_read_line(const std::string& path, const int line) {
        if (line < 0)
        {
            log::error("Failed to read \"" + path + "\", invalid index");
            return "";
        }
        if (!file_exist(path)) {
            log::error("Failed to read \"" + path + "\", file doesnt exist");
            return "";
        }

        std::ifstream file(path);
        if (!file.is_open()) {
            log::error("Failed to open \"" + path + "\"");
            return "";
        }
        std::string text;
        std::string content;
        int i = 0;
        while (getline(file, text)) {
            if (i == line)
            {
                content = (text + '\n');
                break;
            }
            i++;
        }
        file.close();
        log::info("Read line from \"" + path + "\"");
        return content;
    }

    bool file_overwrite(const std::string& path, const std::string& contents)
    {
        if (!file_exist(path)) {
            log::error("Failed to write \"" + path + "\", file doesnt exist");
            return false;
        }

        std::ofstream file(path);
        if (!file.is_open()) {
            log::error("Failed to open \"" + path + "\"");
            return false;
        }
        file << contents << std::endl;
        file.close();
        log::info("Overwritten \"" + path + "\"");
        return true;
    }

    bool file_write_line(const std::string& path, int line, const std::string& contents) {
        if (line < 0) {
            log::error("Failed to write to \"" + path + "\", invalid index");
            return false;
        }

        if (!file_exist(path)) {
            log::error("Failed to write to \"" + path + "\", file doesn't exist");
            return false;
        }

        std::ifstream infile(path);
        if (!infile.is_open()) {
            log::error("Failed to open \"" + path + "\" for reading");
            return false;
        }

        // Read all lines
        std::vector<std::string> lines;
        std::string text;
        while (std::getline(infile, text)) {
            lines.push_back(text);
        }
        infile.close();

        // Insert or replace the line
        if (line < lines.size()) {
            lines[line] = contents; // overwrite
        } else {
            // pad with empty lines if needed
            lines.resize(line);
            lines.push_back(contents);
        }

        // Write back
        std::ofstream outfile(path, std::ios::trunc);
        if (!outfile.is_open()) {
            log::error("Failed to open \"" + path + "\" for writing");
            return false;
        }

        for (const auto& l : lines) {
            outfile << l << '\n';
        }

        outfile.close();
        log::info("Wrote to line to " + std::to_string(line) + " in \"" + path + "\"");
        return true;
    }

    bool file_remove(const std::string& path) {
        if (!std::filesystem::remove(path)) {
            log::error("Failed to remove \"" + path + "\"");
            return false;
        }

        log::info("Removed \"" + path + "\"");
        return true;
    }
}
