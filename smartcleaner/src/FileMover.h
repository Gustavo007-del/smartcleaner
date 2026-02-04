//==============================================================================
// FileMover.h - File Moving Operations Interface
//==============================================================================

#ifndef FILE_MOVER_H
#define FILE_MOVER_H

#include "FileScanner.h"
#include <string>
#include <map>
#include <vector>

namespace DesktopCleaner {

// Forward declaration
class Logger;

//------------------------------------------------------------------------------
// FileMover Class
// Handles safe file moving operations with error handling
//------------------------------------------------------------------------------
class FileMover {
public:
    // Constructor
    FileMover(Logger& logger, bool dryRun = false);
    
    // Main organization method
    bool organizeFiles(
        const std::string& baseDirectory,
        const std::map<std::string, std::vector<FileInfo>>& categorizedFiles
    );
    
    // Get operation statistics
    int getSuccessCount() const;
    int getFailCount() const;
    int getWarningCount() const;
    
private:
    Logger& logger_;          // Reference to logger
    bool dryRun_;            // Dry-run mode flag
    
    // Operation counters
    int successCount_;       // Successfully moved files
    int failCount_;          // Failed operations
    int warningCount_;       // Warnings (e.g., file collisions)
    
    // Helper methods
    bool createCategoryDirectories(
        const std::string& baseDirectory,
        const std::map<std::string, std::vector<FileInfo>>& categorizedFiles
    );
    
    bool moveFile(const FileInfo& fileInfo, const std::string& targetDirectory);
    
    std::string handleFileCollision(
        const std::string& targetDirectory,
        const std::string& fileName
    );
};

} // namespace DesktopCleaner

#endif // FILE_MOVER_H
