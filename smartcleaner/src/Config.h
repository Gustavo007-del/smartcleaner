//==============================================================================
// Config.h - Configuration Constants and Rules
//==============================================================================

#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace DesktopCleaner {

//------------------------------------------------------------------------------
// Application Metadata
//------------------------------------------------------------------------------
const std::string APP_NAME = "Smart Desktop Cleaner";
const std::string APP_VERSION = "1.0";

//------------------------------------------------------------------------------
// Default Configuration Values
//------------------------------------------------------------------------------
const long long DEFAULT_LARGE_FILE_SIZE_MB = 100;     // Files larger than 100MB
const int DEFAULT_OLD_FILE_AGE_DAYS = 90;             // Files older than 90 days
const bool DEFAULT_DRY_RUN = false;                   // Actual move operations

//------------------------------------------------------------------------------
// Logging Configuration
//------------------------------------------------------------------------------
const std::string LOG_DIRECTORY = "logs";
const std::string LOG_FILE_PREFIX = "cleaner_";

//------------------------------------------------------------------------------
// File Category Definitions
//------------------------------------------------------------------------------
const std::string CATEGORY_DOCUMENTS = "Documents";
const std::string CATEGORY_IMAGES = "Images";
const std::string CATEGORY_VIDEOS = "Videos";
const std::string CATEGORY_ARCHIVES = "Archives";
const std::string CATEGORY_CODE = "Code";
const std::string CATEGORY_OTHERS = "Others";

//------------------------------------------------------------------------------
// File Extension to Category Mapping
// Organized by category for easy maintenance
//------------------------------------------------------------------------------

// Document extensions
const std::unordered_set<std::string> EXTENSIONS_DOCUMENTS = {
    ".pdf", ".doc", ".docx", ".txt", ".rtf", ".odt",
    ".xlsx", ".xls", ".pptx", ".ppt", ".csv"
};

// Image extensions
const std::unordered_set<std::string> EXTENSIONS_IMAGES = {
    ".jpg", ".jpeg", ".png", ".gif", ".bmp", ".svg",
    ".webp", ".ico", ".tiff", ".tif", ".raw"
};

// Video extensions
const std::unordered_set<std::string> EXTENSIONS_VIDEOS = {
    ".mp4", ".mkv", ".avi", ".mov", ".wmv", ".flv",
    ".webm", ".mpeg", ".mpg", ".3gp", ".m4v"
};

// Archive extensions
const std::unordered_set<std::string> EXTENSIONS_ARCHIVES = {
    ".zip", ".rar", ".7z", ".tar", ".gz", ".bz2",
    ".xz", ".tgz", ".tar.gz", ".iso"
};

// Code file extensions
const std::unordered_set<std::string> EXTENSIONS_CODE = {
    ".cpp", ".c", ".h", ".hpp", ".py", ".java",
    ".js", ".ts", ".jsx", ".tsx", ".html", ".css",
    ".scss", ".php", ".rb", ".go", ".rs", ".swift",
    ".sh", ".bat", ".json", ".xml", ".yaml", ".yml"
};

//------------------------------------------------------------------------------
// Helper: Get All Categories
//------------------------------------------------------------------------------
inline std::vector<std::string> getAllCategories() {
    return {
        CATEGORY_DOCUMENTS,
        CATEGORY_IMAGES,
        CATEGORY_VIDEOS,
        CATEGORY_ARCHIVES,
        CATEGORY_CODE,
        CATEGORY_OTHERS
    };
}

//------------------------------------------------------------------------------
// Helper: Build Extension-to-Category Map
// This function creates a lookup table for fast classification
//------------------------------------------------------------------------------
inline std::unordered_map<std::string, std::string> buildExtensionMap() {
    std::unordered_map<std::string, std::string> extensionMap;
    
    // Add documents
    for (const auto& ext : EXTENSIONS_DOCUMENTS) {
        extensionMap[ext] = CATEGORY_DOCUMENTS;
    }
    
    // Add images
    for (const auto& ext : EXTENSIONS_IMAGES) {
        extensionMap[ext] = CATEGORY_IMAGES;
    }
    
    // Add videos
    for (const auto& ext : EXTENSIONS_VIDEOS) {
        extensionMap[ext] = CATEGORY_VIDEOS;
    }
    
    // Add archives
    for (const auto& ext : EXTENSIONS_ARCHIVES) {
        extensionMap[ext] = CATEGORY_ARCHIVES;
    }
    
    // Add code files
    for (const auto& ext : EXTENSIONS_CODE) {
        extensionMap[ext] = CATEGORY_CODE;
    }
    
    return extensionMap;
}

//------------------------------------------------------------------------------
// Display Configuration
//------------------------------------------------------------------------------
const std::string CONSOLE_SEPARATOR = "========================================";
const int CONSOLE_WIDTH = 40;

} // namespace DesktopCleaner

#endif // CONFIG_H
