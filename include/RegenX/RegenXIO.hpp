#pragma once
#include <string>

namespace regenx::io {
    /// Check if a file exists at the given path.
        /// @param path Path to the file
        /// @return true if the file exists, false otherwise
    bool file_exist(const std::string& path);

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

    /// Read the entire contents of a file.
        /// @param path Path to the file
        /// @return File contents as a string, or empty string if file cannot be opened
    std::string file_read(const std::string& path);

    /// Read a specific line from a file.
        /// @param path Path to the file
        /// @param line Zero-based line index to read
        /// @return The requested line including newline, or empty string if file cannot be opened
    std::string file_read_line(const std::string& path, const int line);

    /// Remove a file from the filesystem.
        /// @param path Path to the file
        /// @return true if the file was removed, false otherwise
    bool file_remove(const std::string& path);

}
