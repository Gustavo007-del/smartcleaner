//==============================================================================
// FileClassifier.cpp - File Classification Implementation
//==============================================================================

#include "FileClassifier.h"
#include "Logger.h"
#include <algorithm>

namespace DesktopCleaner {

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
FileClassifier::FileClassifier(Logger& logger) 
    : logger_(logger), extensionMap_(buildExtensionMap()) {
}

//------------------------------------------------------------------------------
// Classify Files
//------------------------------------------------------------------------------
void FileClassifier::classifyFiles(const std::vector<FileInfo>& files) {
    categorizedFiles_.clear();
    
    // Initialize categories with empty vectors
    for (const auto& category : getAllCategories()) {
        categorizedFiles_[category] = std::vector<FileInfo>();
    }
    
    logger_.info("Classifying " + std::to_string(files.size()) + " files...");
    
    // Classify each file
    for (const auto& file : files) {
        std::string category = classifyFile(file);
        categorizedFiles_[category].push_back(file);
    }
    
    // Log classification results
    logClassificationResults();
}

//------------------------------------------------------------------------------
// Get Categorized Files
//------------------------------------------------------------------------------
const std::map<std::string, std::vector<FileInfo>>& 
FileClassifier::getCategorizedFiles() const {
    return categorizedFiles_;
}

//------------------------------------------------------------------------------
// Get Files in Category
//------------------------------------------------------------------------------
std::vector<FileInfo> FileClassifier::getFilesInCategory(const std::string& category) const {
    auto it = categorizedFiles_.find(category);
    if (it != categorizedFiles_.end()) {
        return it->second;
    }
    return std::vector<FileInfo>();
}

//------------------------------------------------------------------------------
// Helper: Classify Single File
//------------------------------------------------------------------------------
std::string FileClassifier::classifyFile(const FileInfo& fileInfo) const {
    // Look up extension in map
    auto it = extensionMap_.find(fileInfo.extension);
    
    if (it != extensionMap_.end()) {
        // Found in map
        return it->second;
    } else {
        // Unknown extension -> Others category
        return CATEGORY_OTHERS;
    }
}

//------------------------------------------------------------------------------
// Helper: Log Classification Results
//------------------------------------------------------------------------------
void FileClassifier::logClassificationResults() const {
    logger_.info("Classification results:");
    
    for (const auto& category : getAllCategories()) {
        auto it = categorizedFiles_.find(category);
        if (it != categorizedFiles_.end() && !it->second.empty()) {
            logger_.info("  " + category + ": " + 
                        std::to_string(it->second.size()) + " files");
        }
    }
}

} // namespace DesktopCleaner
