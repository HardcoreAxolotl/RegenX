//
// Created by liad on 1/12/26.
//
#include <RegenX/RegenXLog.hpp>

#include <iostream>
#include <chrono>
#include <format>
#include "RegenXIO.hpp"

std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
const std::string LOG_PATH = "logs/" + std::format("{:%Y-%m-%d %H:%M:%S}\n", now);

namespace regenx::log {
    bool init() {
        if (!regenx::io::file_create(LOG_PATH)) {
            std::cerr << "Failed to create log file\n";
            return false;
        }
        return true;
    }
    void print(const std::string& msg) {
        std::cout << "\033[0m[PRINT] " << msg << "\033[0m\n";
    }

    void info(const std::string& msg) {
        if (regenx::io::file_exist(LOG_PATH) && !regenx::io::file_write(LOG_PATH, "[INFO] " + msg)) {
            std::cerr << "\033[33m[WARNING] Failed to write INFO log to file\033[0m\n";
        }
        std::cout << "\033[32m[INFO] " << msg << "\033[0m\n";
    }

    void debug(const std::string& msg) {
        if (regenx::io::file_exist(LOG_PATH) && !regenx::io::file_write(LOG_PATH, "[DEBUG] " + msg)) {
            std::cerr << "\033[33m[WARNING] Failed to write DEBUG log to file\033[0m\n";
        }
        std::cout << "\033[0m[DEBUG] " << msg << "\033[0m\n";
    }

    void trace(const std::string& msg) {
        if (regenx::io::file_exist(LOG_PATH) && !regenx::io::file_write(LOG_PATH, "[TRACE] " + msg)) {
            std::cerr << "\033[33m[WARNING] Failed to write TRACE log to file\033[0m\n";
        }
        std::cout << "\033[36m[TRACE] " << msg << "\033[0m\n";
    }

    void warning(const std::string& msg) {
        if (regenx::io::file_exist(LOG_PATH) && !regenx::io::file_write(LOG_PATH, "[WARNING] " + msg)) {
            std::cerr << "\033[33m[WARNING] Failed to write WARNING log to file\033[0m\n";
        }
        std::cout << "\033[33m[WARNING] " << msg << "\033[0m\n";
    }

    void error(const std::string& msg) {
        if (regenx::io::file_exist(LOG_PATH) && !regenx::io::file_write(LOG_PATH, "[ERROR] " + msg)) {
            std::cerr << "\033[33m[WARNING] Failed to write ERROR log to file\033[0m\n";
        }
        std::cout << "\033[31m[ERROR] " << msg << "\033[0m\n";
    }

    void fatal(const std::string& msg) {
        if (regenx::io::file_exist(LOG_PATH) && !regenx::io::file_write(LOG_PATH, "[FATAL] " + msg)) {
            std::cerr << "\033[33m[WARNING] Failed to write FATAL log to file\033[0m\n";
        }
        std::cout << "\033[31m[FATAL] " << msg << "\033[0m\n";
        std::abort();
    }

    void system(const std::string& msg) {
        if (regenx::io::file_exist(LOG_PATH) && !regenx::io::file_write(LOG_PATH, "[SYSTEM] " + msg)) {
            std::cerr << "\033[33m[WARNING] Failed to write SYSTEM log to file\033[0m\n";
        }
        std::cout << "\033[34m[SYSTEM] " << msg << "\033[0m\n";
    }
}
