//==============================================================================
// Logger.cpp - Logging System Implementation
//==============================================================================

#include "Logger.h"
#include "Config.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;

namespace DesktopCleaner {

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
Logger::Logger() : consoleOutput_(true) {
    // Create logs directory if it doesn't exist
    try {
        if (!fs::exists(LOG_DIRECTORY)) {
            fs::create_directory(LOG_DIRECTORY);
        }
        
        // Generate log file path with timestamp
        logFilePath_ = generateLogFilePath();
        
        // Open log file
        logFile_.open(logFilePath_, std::ios::out | std::ios::app);
        
        if (logFile_.is_open()) {
            logSeparator();
            log(LogLevel::INFO, APP_NAME + " v" + APP_VERSION + " - Session Started");
            logSeparator();
        } else {
            std::cerr << "Warning: Could not open log file: " << logFilePath_ << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error initializing logger: " << e.what() << std::endl;
    }
}

//------------------------------------------------------------------------------
// Destructor
//------------------------------------------------------------------------------
Logger::~Logger() {
    if (logFile_.is_open()) {
        logSeparator();
        log(LogLevel::INFO, "Session Ended");
        logSeparator();
        logFile_.close();
    }
}

//------------------------------------------------------------------------------
// Main Logging Method
//------------------------------------------------------------------------------
void Logger::log(LogLevel level, const std::string& message) {
    std::string timestamp = getCurrentTimestamp();
    std::string levelStr = levelToString(level);
    std::string logEntry = "[" + timestamp + "] " + levelStr + ": " + message;
    
    writeLog(levelStr, logEntry);
}

//------------------------------------------------------------------------------
// Convenience Logging Methods
//------------------------------------------------------------------------------
void Logger::info(const std::string& message) {
    log(LogLevel::INFO, message);
}

void Logger::success(const std::string& message) {
    log(LogLevel::SUCCESS, message);
}

void Logger::warning(const std::string& message) {
    log(LogLevel::WARNING, message);
}

void Logger::error(const std::string& message) {
    log(LogLevel::ERROR, message);
}

void Logger::debug(const std::string& message) {
    log(LogLevel::DEBUG, message);
}

//------------------------------------------------------------------------------
// Log Separator
//------------------------------------------------------------------------------
void Logger::logSeparator() {
    std::string separator(70, '=');
    if (logFile_.is_open()) {
        logFile_ << separator << std::endl;
    }
}

//------------------------------------------------------------------------------
// Log Summary
//------------------------------------------------------------------------------
void Logger::logSummary(int totalFiles, int successCount, int failCount, int warningCount) {
    logSeparator();
    info("Operation Summary:");
    info("  Total files processed: " + std::to_string(totalFiles));
    info("  Successfully moved: " + std::to_string(successCount));
    info("  Failed: " + std::to_string(failCount));
    info("  Warnings: " + std::to_string(warningCount));
    logSeparator();
}

//------------------------------------------------------------------------------
// Status Methods
//------------------------------------------------------------------------------
bool Logger::isOpen() const {
    return logFile_.is_open();
}

std::string Logger::getLogFilePath() const {
    return logFilePath_;
}

//------------------------------------------------------------------------------
// Helper: Generate Log File Path
//------------------------------------------------------------------------------
std::string Logger::generateLogFilePath() const {
    // Get current time for filename
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    std::tm tm_buf;
    
    // Thread-safe time conversion
#ifdef _WIN32
    localtime_s(&tm_buf, &time_t);
#else
    localtime_r(&time_t, &tm_buf);
#endif
    
    // Format: cleaner_YYYYMMDD_HHMMSS.log
    std::ostringstream oss;
    oss << LOG_DIRECTORY << "/" << LOG_FILE_PREFIX
        << std::put_time(&tm_buf, "%Y%m%d_%H%M%S")
        << ".log";
    
    return oss.str();
}

//------------------------------------------------------------------------------
// Helper: Get Current Timestamp
//------------------------------------------------------------------------------
std::string Logger::getCurrentTimestamp() const {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    std::tm tm_buf;
    
#ifdef _WIN32
    localtime_s(&tm_buf, &time_t);
#else
    localtime_r(&time_t, &tm_buf);
#endif
    
    std::ostringstream oss;
    oss << std::put_time(&tm_buf, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

//------------------------------------------------------------------------------
// Helper: Convert Log Level to String
//------------------------------------------------------------------------------
std::string Logger::levelToString(LogLevel level) const {
    switch (level) {
        case LogLevel::INFO:    return "INFO";
        case LogLevel::SUCCESS: return "SUCCESS";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR:   return "ERROR";
        case LogLevel::DEBUG:   return "DEBUG";
        default:                return "UNKNOWN";
    }
}

//------------------------------------------------------------------------------
// Helper: Write to File and Console
//------------------------------------------------------------------------------
void Logger::writeLog(const std::string& prefix, const std::string& message) {
    // Write to file
    if (logFile_.is_open()) {
        logFile_ << message << std::endl;
        logFile_.flush(); // Ensure immediate write
    }
    
    // Write to console with color coding (simple version)
    if (consoleOutput_) {
        if (prefix == "ERROR") {
            std::cerr << message << std::endl;
        } else {
            std::cout << message << std::endl;
        }
    }
}

} // namespace DesktopCleaner
