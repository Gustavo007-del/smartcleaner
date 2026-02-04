//==============================================================================
// Logger.h - Logging System Interface
//==============================================================================

#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>

namespace DesktopCleaner {

//------------------------------------------------------------------------------
// Log Level Enumeration
//------------------------------------------------------------------------------
enum class LogLevel {
    INFO,      // General information
    SUCCESS,   // Successful operation
    WARNING,   // Warning message
    ERROR,     // Error message
    DEBUG      // Debug information
};

//------------------------------------------------------------------------------
// Logger Class
// Handles all logging operations to file and console
//------------------------------------------------------------------------------
class Logger {
public:
    // Constructor & Destructor
    Logger();
    ~Logger();
    
    // Prevent copying
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
    // Main logging method
    void log(LogLevel level, const std::string& message);
    
    // Convenience methods
    void info(const std::string& message);
    void success(const std::string& message);
    void warning(const std::string& message);
    void error(const std::string& message);
    void debug(const std::string& message);
    
    // Utility methods
    void logSeparator();
    void logSummary(int totalFiles, int successCount, int failCount, int warningCount);
    
    // Status methods
    bool isOpen() const;
    std::string getLogFilePath() const;
    
private:
    std::ofstream logFile_;        // Log file stream
    std::string logFilePath_;      // Path to current log file
    bool consoleOutput_;           // Enable console output
    
    // Helper methods
    std::string generateLogFilePath() const;
    std::string getCurrentTimestamp() const;
    std::string levelToString(LogLevel level) const;
    void writeLog(const std::string& prefix, const std::string& message);
};

} // namespace DesktopCleaner

#endif // LOGGER_H
