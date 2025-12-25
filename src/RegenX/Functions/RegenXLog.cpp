//
// Created by liad on 24/12/2025.
//

#include "RegenXLog.h"

#include <iostream>
#include <ostream>

void log(const std::string& text) {
    std::cout << text << std::endl;
}

void warning(const std::string& text) {
    std::cout << "\033[33m[Warning]\033[0m " << text << std::endl;
}

void error(const std::string& text) {
    std::cerr << "[Error] " << text << std::endl;
}