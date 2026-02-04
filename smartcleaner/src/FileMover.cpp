//==============================================================================
// FileMover.cpp - File Moving Operations Implementation
//==============================================================================

#include "FileMover.h"
#include "Logger.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>

namespace fs = std::filesystem;

namespace DesktopCleaner {

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
FileMover::FileMover(Logger& logger, bool dryRun) 
    : logger_(logger), 
      dryRun_(dryRun),
      successCount_(0),
      failCount_(0),
      warningCount_(0) {
}

//------------------------------------------------------------------------------
// Organize Files
//------------------------------------------------------------------------------
bool FileMover::organizeFiles(
    const std::string& baseDirectory,
    const std::map<std::string, std::vector<FileInfo>>& categorizedFiles) {
    
    logger_.info("Starting file organization...");
    
    if (dryRun_) {
        logger_.info("[DRY-RUN MODE] No files will be actually moved");
    }
    
    // Reset counters
    successCount_ = 0;
    failCount_ = 0;
    warningCount_ = 0;
    
    try {
        // Step 1: Create category directories
        if (!createCategoryDirectories(baseDirectory, categorizedFiles)) {
            logger_.error("Failed to create category directories");
            return false;
        }
        
        // Step 2: Move files to their categories
        for (const auto& [category, files] : categorizedFiles) {
            if (files.empty()) {
                continue; // Skip empty categories
            }
            
            std::string targetDir = baseDirectory + "/" + category;
            
            for (const auto& file : files) {
                moveFile(file, targetDir);
            }
        }
        
        // Log summary
        logger_.logSummary(
            successCount_ + failCount_,
            successCount_,
            failCount_,
            warningCount_
        );
        
        return true;
        
    } catch (const std::exception& e) {
        logger_.error("Unexpected error during organization: " + std::string(e.what()));
        return false;
    }
}

//------------------------------------------------------------------------------
// Get Operation Statistics
//------------------------------------------------------------------------------
int FileMover::getSuccessCount() const { return successCount_; }
int FileMover::getFailCount() const { return failCount_; }
int FileMover::getWarningCount() const { return warningCount_; }

//------------------------------------------------------------------------------
// Helper: Create Category Directories
//------------------------------------------------------------------------------
bool FileMover::createCategoryDirectories(
    const std::string& baseDirectory,
    const std::map<std::string, std::vector<FileInfo>>& categorizedFiles) {
    
    logger_.info("Creating category directories...");
    
    for (const auto& [category, files] : categorizedFiles) {
        // Skip empty categories
        if (files.empty()) {
            continue;
        }
        
        std::string categoryPath = baseDirectory + "/" + category;
        
        try {
            if (dryRun_) {
                if (!fs::exists(categoryPath)) {
                    logger_.info("[DRY-RUN] Would create directory: " + category);
                }
            } else {
                // Create directory if it doesn't exist
                if (!fs::exists(categoryPath)) {
                    fs::create_directory(categoryPath);
                    logger_.success("Created directory: " + category);
                } else {
                    logger_.info("Directory already exists: " + category);
                }
            }
        } catch (const fs::filesystem_error& e) {
            logger_.error("Failed to create directory: " + category + 
                         " - " + e.what());
            return false;
        }
    }
    
    return true;
}

//------------------------------------------------------------------------------
// Helper: Move Single File
//------------------------------------------------------------------------------
bool FileMover::moveFile(const FileInfo& fileInfo, const std::string& targetDirectory) {
    try {
        std::string targetPath = targetDirectory + "/" + fileInfo.name;
        
        // Check if target file already exists
        if (fs::exists(targetPath)) {
            // Handle collision: append timestamp
            targetPath = handleFileCollision(targetDirectory, fileInfo.name);
            warningCount_++;
        }
        
        if (dryRun_) {
            // Dry-run: just log what would happen
            logger_.info("[DRY-RUN] Would move: " + fileInfo.name + " → " + 
                        fs::path(targetDirectory).filename().string() + "/");
            successCount_++;
            return true;
        }
        
        // Actual move operation
        fs::rename(fileInfo.path, targetPath);
        
        logger_.success("Moved: " + fileInfo.name + " → " + 
                       fs::path(targetDirectory).filename().string() + "/");
        successCount_++;
        return true;
        
    } catch (const fs::filesystem_error& e) {
        logger_.error("Failed to move: " + fileInfo.name + " - " + e.what());
        failCount_++;
        return false;
    } catch (const std::exception& e) {
        logger_.error("Unexpected error moving: " + fileInfo.name + " - " + e.what());
        failCount_++;
        return false;
    }
}

//------------------------------------------------------------------------------
// Helper: Handle File Name Collision
//------------------------------------------------------------------------------
std::string FileMover::handleFileCollision(
    const std::string& targetDirectory, 
    const std::string& fileName) {
    
    // Extract file name and extension
    fs::path filePath(fileName);
    std::string stem = filePath.stem().string();
    std::string extension = filePath.extension().string();
    
    // Generate timestamp suffix
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    std::tm tm_buf;
    
#ifdef _WIN32
    localtime_s(&tm_buf, &time_t);
#else
    localtime_r(&time_t, &tm_buf);
#endif
    
    std::ostringstream oss;
    oss << std::put_time(&tm_buf, "%Y%m%d_%H%M%S");
    
    // Create new filename: original_stem_timestamp.extension
    std::string newFileName = stem + "_" + oss.str() + extension;
    std::string newPath = targetDirectory + "/" + newFileName;
    
    logger_.warning("File collision detected: " + fileName + 
                   " renamed to: " + newFileName);
    
    return newPath;
}

} // namespace DesktopCleaner
