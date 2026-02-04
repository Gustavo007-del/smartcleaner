//==============================================================================
// main.cpp - Smart Desktop Cleaner Entry Point
//==============================================================================

#include "Logger.h"
#include "FileScanner.h"
#include "FileClassifier.h"
#include "FileMover.h"
#include "Config.h"
#include <iostream>
#include <filesystem>
#include <string>
#include <cstdlib>

namespace fs = std::filesystem;
using namespace DesktopCleaner;

//------------------------------------------------------------------------------
// Function Prototypes
//------------------------------------------------------------------------------
void printHeader();
void printUsage();
void printSeparator();
bool parseArguments(int argc, char* argv[], std::string& directory, 
                   bool& dryRun, long long& sizeThreshold, int& ageThreshold);
std::string getDefaultDesktopPath();
void displayAnalysis(const FileScanner& scanner);

//------------------------------------------------------------------------------
// Main Function
//------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    // Parse command-line arguments
    std::string targetDirectory;
    bool dryRun = DEFAULT_DRY_RUN;
    long long sizeThresholdMB = DEFAULT_LARGE_FILE_SIZE_MB;
    int ageThresholdDays = DEFAULT_OLD_FILE_AGE_DAYS;
    
    if (!parseArguments(argc, argv, targetDirectory, dryRun, 
                       sizeThresholdMB, ageThresholdDays)) {
        return 1;
    }
    
    // Use current directory if no path specified
    if (targetDirectory.empty()) {
        targetDirectory = fs::current_path().string();
    }
    
    // Verify directory exists
    if (!fs::exists(targetDirectory)) {
        std::cerr << "Error: Directory does not exist: " << targetDirectory << std::endl;
        return 1;
    }
    
    // Print application header
    printHeader();
    
    // Initialize Logger
    Logger logger;
    if (!logger.isOpen()) {
        std::cerr << "Warning: Logging may not work properly" << std::endl;
    }
    
    // Log configuration
    logger.info("Target directory: " + targetDirectory);
    logger.info("Dry-run mode: " + std::string(dryRun ? "true" : "false"));
    logger.info("Large file threshold: " + std::to_string(sizeThresholdMB) + " MB");
    logger.info("Old file threshold: " + std::to_string(ageThresholdDays) + " days");
    
    std::cout << "\nScanning directory: " << targetDirectory << std::endl;
    std::cout << "Dry-run mode: " << (dryRun ? "ON" : "OFF") << std::endl;
    std::cout << "Large file threshold: " << sizeThresholdMB << " MB" << std::endl;
    std::cout << "Old file threshold: " << ageThresholdDays << " days" << std::endl;
    
    try {
        // Step 1: Scan Directory
        printSeparator();
        std::cout << "[SCAN] Scanning files..." << std::endl;
        
        FileScanner scanner(logger);
        scanner.setLargeFileSizeMB(sizeThresholdMB);
        scanner.setOldFileAgeDays(ageThresholdDays);
        
        if (!scanner.scanDirectory(targetDirectory)) {
            logger.error("Failed to scan directory");
            std::cerr << "Error: Failed to scan directory" << std::endl;
            return 1;
        }
        
        const auto& files = scanner.getFiles();
        std::cout << "[SCAN] Found " << files.size() << " files" << std::endl;
        
        if (files.empty()) {
            std::cout << "\nNo files to organize. Exiting." << std::endl;
            return 0;
        }
        
        // Step 2: Classify Files
        printSeparator();
        std::cout << "[CLASSIFY] Categorizing files..." << std::endl;
        
        FileClassifier classifier(logger);
        classifier.classifyFiles(files);
        
        const auto& categorizedFiles = classifier.getCategorizedFiles();
        
        // Display classification results
        for (const auto& category : getAllCategories()) {
            auto filesInCategory = classifier.getFilesInCategory(category);
            if (!filesInCategory.empty()) {
                std::cout << "  " << category << ": " 
                         << filesInCategory.size() << " files" << std::endl;
            }
        }
        
        // Step 3: Analyze Files (Large & Old)
        printSeparator();
        displayAnalysis(scanner);
        
        // Step 4: Move Files
        printSeparator();
        std::cout << "[ORGANIZE] " << (dryRun ? "[DRY-RUN] " : "") 
                  << "Organizing files..." << std::endl;
        
        FileMover mover(logger, dryRun);
        
        if (!mover.organizeFiles(targetDirectory, categorizedFiles)) {
            logger.error("File organization failed");
            std::cerr << "Error: File organization failed" << std::endl;
            return 1;
        }
        
        // Step 5: Display Summary
        printSeparator();
        std::cout << "\n✓ Operation completed successfully!\n" << std::endl;
        
        std::cout << "Summary:" << std::endl;
        std::cout << "  Total files: " << files.size() << std::endl;
        std::cout << "  Successfully moved: " << mover.getSuccessCount() << std::endl;
        std::cout << "  Failed: " << mover.getFailCount() << std::endl;
        std::cout << "  Warnings: " << mover.getWarningCount() << std::endl;
        
        std::cout << "\nLog file: " << logger.getLogFilePath() << std::endl;
        
        printSeparator();
        
        return 0;
        
    } catch (const std::exception& e) {
        logger.error("Fatal error: " + std::string(e.what()));
        std::cerr << "\nFatal error: " << e.what() << std::endl;
        return 1;
    }
}

//------------------------------------------------------------------------------
// Print Application Header
//------------------------------------------------------------------------------
void printHeader() {
    printSeparator();
    std::cout << "  " << APP_NAME << " v" << APP_VERSION << std::endl;
    printSeparator();
}

//------------------------------------------------------------------------------
// Print Usage Information
//------------------------------------------------------------------------------
void printUsage() {
    std::cout << "Usage: desktop_cleaner [OPTIONS] [DIRECTORY]\n" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  --dry-run           Preview actions without moving files" << std::endl;
    std::cout << "  --size=<MB>         Large file threshold in MB (default: 100)" << std::endl;
    std::cout << "  --age=<DAYS>        Old file threshold in days (default: 90)" << std::endl;
    std::cout << "  --help              Display this help message" << std::endl;
    std::cout << "\nExamples:" << std::endl;
    std::cout << "  desktop_cleaner --dry-run ~/Desktop" << std::endl;
    std::cout << "  desktop_cleaner --size=50 --age=30 /path/to/folder" << std::endl;
    std::cout << "  desktop_cleaner C:\\Users\\YourName\\Desktop" << std::endl;
}

//------------------------------------------------------------------------------
// Print Separator Line
//------------------------------------------------------------------------------
void printSeparator() {
    std::cout << CONSOLE_SEPARATOR << std::endl;
}

//------------------------------------------------------------------------------
// Parse Command-Line Arguments
//------------------------------------------------------------------------------
bool parseArguments(int argc, char* argv[], std::string& directory, 
                   bool& dryRun, long long& sizeThreshold, int& ageThreshold) {
    directory = "";
    
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "--help" || arg == "-h") {
            printUsage();
            return false;
        }
        else if (arg == "--dry-run") {
            dryRun = true;
        }
        else if (arg.find("--size=") == 0) {
            try {
                sizeThreshold = std::stoll(arg.substr(7));
                if (sizeThreshold <= 0) {
                    std::cerr << "Error: Size threshold must be positive" << std::endl;
                    return false;
                }
            } catch (const std::exception& e) {
                std::cerr << "Error: Invalid size value: " << arg << std::endl;
                return false;
            }
        }
        else if (arg.find("--age=") == 0) {
            try {
                ageThreshold = std::stoi(arg.substr(6));
                if (ageThreshold <= 0) {
                    std::cerr << "Error: Age threshold must be positive" << std::endl;
                    return false;
                }
            } catch (const std::exception& e) {
                std::cerr << "Error: Invalid age value: " << arg << std::endl;
                return false;
            }
        }
        else if (arg[0] == '-') {
            std::cerr << "Error: Unknown option: " << arg << std::endl;
            std::cerr << "Use --help for usage information" << std::endl;
            return false;
        }
        else {
            // Assume it's a directory path
            directory = arg;
        }
    }
    
    return true;
}

//------------------------------------------------------------------------------
// Display File Analysis (Large and Old Files)
//------------------------------------------------------------------------------
void displayAnalysis(const FileScanner& scanner) {
    const auto& largeFiles = scanner.getLargeFiles();
    const auto& oldFiles = scanner.getOldFiles();
    
    std::cout << "[ANALYZE] File analysis:" << std::endl;
    
    // Display large files
    if (!largeFiles.empty()) {
        std::cout << "  Large files (" << largeFiles.size() << "):" << std::endl;
        for (size_t i = 0; i < std::min(size_t(5), largeFiles.size()); ++i) {
            const auto& file = largeFiles[i];
            double sizeMB = static_cast<double>(file.sizeBytes) / (1024.0 * 1024.0);
            std::cout << "    - " << file.name << " (" 
                     << std::fixed << std::setprecision(1) << sizeMB << " MB)" 
                     << std::endl;
        }
        if (largeFiles.size() > 5) {
            std::cout << "    ... and " << (largeFiles.size() - 5) << " more" << std::endl;
        }
    } else {
        std::cout << "  No large files detected" << std::endl;
    }
    
    // Display old files
    if (!oldFiles.empty()) {
        std::cout << "  Old files (" << oldFiles.size() << "):" << std::endl;
        for (size_t i = 0; i < std::min(size_t(5), oldFiles.size()); ++i) {
            const auto& file = oldFiles[i];
            auto now = std::chrono::system_clock::now();
            auto nowTimeT = std::chrono::system_clock::to_time_t(now);
            int ageDays = static_cast<int>((nowTimeT - file.lastModified) / (60 * 60 * 24));
            
            std::cout << "    - " << file.name << " (" 
                     << ageDays << " days old)" << std::endl;
        }
        if (oldFiles.size() > 5) {
            std::cout << "    ... and " << (oldFiles.size() - 5) << " more" << std::endl;
        }
    } else {
        std::cout << "  No old files detected" << std::endl;
    }
}
