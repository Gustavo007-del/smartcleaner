//==============================================================================
// FileScanner.h - Directory Scanning Interface
//==============================================================================

#ifndef FILE_SCANNER_H
#define FILE_SCANNER_H

#include "Config.h"
#include <string>
#include <vector>
#include <filesystem>
#include <ctime>

namespace DesktopCleaner {

// Forward declaration
class Logger;

//------------------------------------------------------------------------------
// FileInfo Structure
// Contains metadata about a single file
//------------------------------------------------------------------------------
struct FileInfo {
    std::filesystem::path path;     // Full path to file
    std::string name;               // File name with extension
    std::string extension;          // File extension (lowercase)
    long long sizeBytes;            // File size in bytes
    std::time_t lastModified;       // Last modification time
};

//------------------------------------------------------------------------------
// FileScanner Class
// Responsible for scanning directories and collecting file information
//------------------------------------------------------------------------------
class FileScanner {
public:
    // Constructor
    explicit FileScanner(Logger& logger);
    
    // Main scanning method
    bool scanDirectory(const std::string& directoryPath);
    
    // Get scan results
    const std::vector<FileInfo>& getFiles() const;
    const std::vector<FileInfo>& getLargeFiles() const;
    const std::vector<FileInfo>& getOldFiles() const;
    
    // Configuration setters
    void setLargeFileSizeMB(long long sizeMB);
    void setOldFileAgeDays(int ageDays);
    
private:
    Logger& logger_;                        // Reference to logger
    std::vector<FileInfo> files_;           // All scanned files
    std::vector<FileInfo> largeFiles_;      // Files exceeding size threshold
    std::vector<FileInfo> oldFiles_;        // Files exceeding age threshold
    
    // Configuration
    long long largeFileSizeMB_;             // Large file threshold (MB)
    int oldFileAgeDays_;                    // Old file threshold (days)
    
    // Helper methods
    FileInfo extractFileInfo(const std::filesystem::directory_entry& entry) const;
    bool isLargeFile(const FileInfo& fileInfo) const;
    bool isOldFile(const FileInfo& fileInfo) const;
};

} // namespace DesktopCleaner

#endif // FILE_SCANNER_H
