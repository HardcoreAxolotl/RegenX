//
// Created by liad on 16/12/2025.
//
#pragma once

#ifndef REGENX_REGENXIO_H
#define REGENX_REGENXIO_H
#include <lua.h>
#include <string>
bool file_exists(const std::string& file_path);

bool file_delete(const std::string& file_path);

bool file_create(const std::string& file_path, const std::string& content = "");

std::string file_read_line(const std::string& file_path, const int line);

std::string file_read(const std::string& file_path);

bool file_overwrite(const std::string& file_path, const std::string& content);

bool file_write(const std::string& file_path, const std::string& content);
#endif //REGENX_REGENXIO_H