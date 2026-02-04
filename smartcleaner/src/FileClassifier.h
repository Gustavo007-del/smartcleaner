//==============================================================================
// FileClassifier.h - File Classification Interface
//==============================================================================

#ifndef FILE_CLASSIFIER_H
#define FILE_CLASSIFIER_H

#include "Config.h"
#include "FileScanner.h"
#include <string>
#include <vector>
#include <map>
#include <unordered_map>

namespace DesktopCleaner {

// Forward declaration
class Logger;

//------------------------------------------------------------------------------
// FileClassifier Class
// Categorizes files based on extension rules
//------------------------------------------------------------------------------
class FileClassifier {
public:
    // Constructor
    explicit FileClassifier(Logger& logger);
    
    // Main classification method
    void classifyFiles(const std::vector<FileInfo>& files);
    
    // Get classification results
    const std::map<std::string, std::vector<FileInfo>>& getCategorizedFiles() const;
    std::vector<FileInfo> getFilesInCategory(const std::string& category) const;
    
private:
    Logger& logger_;                                                // Reference to logger
    std::unordered_map<std::string, std::string> extensionMap_;     // Extension -> Category mapping
    std::map<std::string, std::vector<FileInfo>> categorizedFiles_; // Category -> Files mapping
    
    // Helper methods
    std::string classifyFile(const FileInfo& fileInfo) const;
    void logClassificationResults() const;
};

} // namespace DesktopCleaner

#endif // FILE_CLASSIFIER_H
