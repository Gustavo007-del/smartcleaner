//==============================================================================
// FileScanner.cpp - Directory Scanning Implementation
//==============================================================================

#include "FileScanner.h"
#include "Logger.h"
#include <iostream>
#include <chrono>

namespace fs = std::filesystem;

namespace DesktopCleaner {

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
FileScanner::FileScanner(Logger& logger) 
    : logger_(logger), 
      largeFileSizeMB_(DEFAULT_LARGE_FILE_SIZE_MB),
      oldFileAgeDays_(DEFAULT_OLD_FILE_AGE_DAYS) {
}

//------------------------------------------------------------------------------
// Scan Directory
//------------------------------------------------------------------------------
bool FileScanner::scanDirectory(const std::string& directoryPath) {
    files_.clear();
    largeFiles_.clear();
    oldFiles_.clear();
    
    try {
        // Verify directory exists
        if (!fs::exists(directoryPath)) {
            logger_.error("Directory does not exist: " + directoryPath);
            return false;
        }
        
        if (!fs::is_directory(directoryPath)) {
            logger_.error("Path is not a directory: " + directoryPath);
            return false;
        }
        
        logger_.info("Scanning directory: " + directoryPath);
        
        // Iterate through directory entries
        for (const auto& entry : fs::directory_iterator(directoryPath)) {
            try {
                // Only process regular files (skip directories, symlinks, etc.)
                if (entry.is_regular_file()) {
                    FileInfo fileInfo = extractFileInfo(entry);
                    files_.push_back(fileInfo);
                    
                    // Check if file is large
                    if (isLargeFile(fileInfo)) {
                        largeFiles_.push_back(fileInfo);
                    }
                    
                    // Check if file is old
                    if (isOldFile(fileInfo)) {
                        oldFiles_.push_back(fileInfo);
                    }
                }
            } catch (const std::exception& e) {
                // Log individual file errors but continue scanning
                logger_.warning("Error processing file: " + entry.path().string() + 
                              " - " + e.what());
            }
        }
        
        logger_.info("Found " + std::to_string(files_.size()) + " files");
        
        return true;
        
    } catch (const fs::filesystem_error& e) {
        logger_.error("Filesystem error during scan: " + std::string(e.what()));
        return false;
    } catch (const std::exception& e) {
        logger_.error("Unexpected error during scan: " + std::string(e.what()));
        return false;
    }
}

//------------------------------------------------------------------------------
// Get Scan Results
//------------------------------------------------------------------------------
const std::vector<FileInfo>& FileScanner::getFiles() const {
    return files_;
}

const std::vector<FileInfo>& FileScanner::getLargeFiles() const {
    return largeFiles_;
}

const std::vector<FileInfo>& FileScanner::getOldFiles() const {
    return oldFiles_;
}

//------------------------------------------------------------------------------
// Configuration Setters
//------------------------------------------------------------------------------
void FileScanner::setLargeFileSizeMB(long long sizeMB) {
    largeFileSizeMB_ = sizeMB;
    logger_.info("Large file threshold set to: " + std::to_string(sizeMB) + " MB");
}

void FileScanner::setOldFileAgeDays(int ageDays) {
    oldFileAgeDays_ = ageDays;
    logger_.info("Old file threshold set to: " + std::to_string(ageDays) + " days");
}

//------------------------------------------------------------------------------
// Helper: Extract File Information
//------------------------------------------------------------------------------
FileInfo FileScanner::extractFileInfo(const fs::directory_entry& entry) const {
    FileInfo info;
    
    try {
        info.path = entry.path();
        info.name = entry.path().filename().string();
        info.extension = entry.path().extension().string();
        
        // Convert extension to lowercase for consistent matching
        std::transform(info.extension.begin(), info.extension.end(), 
                      info.extension.begin(), ::tolower);
        
        // Get file size
        info.sizeBytes = fs::file_size(entry.path());
        
        // Get last write time
        auto ftime = fs::last_write_time(entry.path());
        
        // Convert filesystem time to system time
        // Note: C++20 has better methods, but for C++17 we use this approach
        auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
            ftime - fs::file_time_type::clock::now() + std::chrono::system_clock::now()
        );
        info.lastModified = std::chrono::system_clock::to_time_t(sctp);
        
    } catch (const std::exception& e) {
        logger_.warning("Error extracting info for: " + entry.path().string());
        throw; // Re-throw to be handled by caller
    }
    
    return info;
}

//------------------------------------------------------------------------------
// Helper: Check if File is Large
//------------------------------------------------------------------------------
bool FileScanner::isLargeFile(const FileInfo& fileInfo) const {
    long long sizeMB = fileInfo.sizeBytes / (1024 * 1024);
    return sizeMB >= largeFileSizeMB_;
}

//------------------------------------------------------------------------------
// Helper: Check if File is Old
//------------------------------------------------------------------------------
bool FileScanner::isOldFile(const FileInfo& fileInfo) const {
    auto now = std::chrono::system_clock::now();
    auto nowTimeT = std::chrono::system_clock::to_time_t(now);
    
    // Calculate age in days
    long long ageSeconds = nowTimeT - fileInfo.lastModified;
    int ageDays = static_cast<int>(ageSeconds / (60 * 60 * 24));
    
    return ageDays >= oldFileAgeDays_;
}

} // namespace DesktopCleaner
