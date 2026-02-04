#pragma once
#include <string>

namespace regenx::io {

    /// Check if a file exists at the given path.
    /// @param path Path to the file
    /// @return true if the file exists, false otherwise
    bool file_exist(const std::string& path);

    /// Remove a file from the filesystem.
    /// @param path Path to the file
    /// @return true if the file was removed, false otherwise
    bool file_remove(const std::string& path);

    /// Create a new file at the given path with optional initial contents.
    /// Does nothing if the file already exists.
    /// @param path Path to the file
    /// @param contents Initial contents to write (optional)
    /// @return true if the file was created successfully, false otherwise
    bool file_create(const std::string& path, const std::string& contents = "");

    /// Append contents to an existing file.
    /// @param path Path to the file
    /// @param contents Contents to append
    /// @return true if write succeeded, false otherwise
    bool file_write(const std::string& path, const std::string& contents = "");

    /// Overwrite the entire contents of a file.
    /// @param path Path to the file
    /// @param contents Contents to write
    /// @return true if the file was overwritten successfully, false otherwise
    bool file_overwrite(const std::string& path, const std::string& contents);

    /// Write a specific line in a file.
    /// - Overwrites that line if it exists.
    /// - Appends empty lines if the file is shorter than the specified line index.
    /// @param path Path to the file
    /// @param line Zero-based line index
    /// @param contents Line contents to write
    /// @return true if write succeeded, false otherwise
    bool file_write_line(const std::string& path, const int line, const std::string& contents);

    /// Read the entire contents of a file.
    /// @param path Path to the file
    /// @return File contents as a string, or empty string if file cannot be opened
    std::string file_read(const std::string& path);

    /// Read a specific line from a file.
    /// @param path Path to the file
    /// @param line Zero-based line index to read
    /// @return The requested line including newline, or empty string if file cannot be opened
    std::string file_read_line(const std::string& path, const int line);
}
