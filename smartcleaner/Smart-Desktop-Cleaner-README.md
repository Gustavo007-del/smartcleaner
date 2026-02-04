# Smart Desktop Cleaner (C++)

## Project Description

A command-line utility written in C++17 that automatically organizes and cleans desktop files using intelligent rule-based file classification. This tool scans directories, categorizes files by extension, identifies large/old files, and safely moves them into organized category folders with comprehensive logging.

Built as a production-ready system programming project demonstrating file system operations, defensive programming, and modular C++ design.

---

## Features

✅ **Automatic File Categorization**
- Documents (pdf, docx, txt, xlsx, pptx, odt)
- Images (jpg, jpeg, png, gif, bmp, svg, webp)
- Videos (mp4, mkv, avi, mov, wmv, flv)
- Archives (zip, rar, 7z, tar, gz)
- Code files (cpp, h, py, js, java, html, css)
- Others (uncategorized files)

✅ **Smart File Analysis**
- Identifies large files exceeding size threshold (default: 100MB)
- Detects old files based on last modified date (default: 90 days)
- Displays file statistics before operations

✅ **Safety Features**
- Dry-run mode: Preview actions without making changes
- Comprehensive error handling (permissions, file locks, invalid paths)
- Detailed logging of all operations
- Non-destructive: Files are moved, not deleted

✅ **Configurable Parameters**
- Custom directory path
- Adjustable size threshold for "large files"
- Adjustable age threshold for "old files"
- Extensible category rules

✅ **Comprehensive Logging**
- Operation timestamps
- Success/failure records
- Error details with file paths
- Log rotation with timestamps

---

## Project Structure

```
SmartDesktopCleaner/
│
├── src/
│   ├── main.cpp                 # Entry point, CLI argument parsing
│   ├── FileScanner.h            # Directory scanning declarations
│   ├── FileScanner.cpp          # File enumeration implementation
│   ├── FileClassifier.h         # File categorization declarations
│   ├── FileClassifier.cpp       # Extension-based classification logic
│   ├── FileMover.h              # File moving operations declarations
│   ├── FileMover.cpp            # Safe file moving with error handling
│   ├── Logger.h                 # Logging system declarations
│   ├── Logger.cpp               # File-based logging implementation
│   └── Config.h                 # Configuration constants & rules
│
├── logs/                        # Generated log files (created at runtime)
├── README.md                    # This file
└── build.sh                     # Build script (optional)
```

---

## Build Requirements

### Prerequisites
- **Compiler**: g++ 7.0+ or clang++ 5.0+ (C++17 support required)
- **Operating System**: Windows 10+, Linux (Ubuntu 18.04+), macOS 10.13+
- **Standard Library**: C++17 `<filesystem>` support

### Required Headers
- `<filesystem>` (std::filesystem)
- `<vector>`, `<map>`, `<string>`
- `<fstream>`, `<iostream>`
- `<chrono>`, `<ctime>`

---

## Compilation Steps

### Linux / macOS

**Option 1: Single Command**
```bash
g++ -std=c++17 -Wall -Wextra -O2 \
    src/main.cpp \
    src/FileScanner.cpp \
    src/FileClassifier.cpp \
    src/FileMover.cpp \
    src/Logger.cpp \
    -o desktop_cleaner
```

**Option 2: With Filesystem Linking (if needed)**
```bash
g++ -std=c++17 -Wall -Wextra -O2 \
    src/main.cpp \
    src/FileScanner.cpp \
    src/FileClassifier.cpp \
    src/FileMover.cpp \
    src/Logger.cpp \
    -lstdc++fs -o desktop_cleaner
```

### Windows (MinGW/MSYS2)
```cmd
g++ -std=c++17 -Wall -Wextra -O2 ^
    src\main.cpp ^
    src\FileScanner.cpp ^
    src\FileClassifier.cpp ^
    src\FileMover.cpp ^
    src\Logger.cpp ^
    -o desktop_cleaner.exe
```

### Windows (Visual Studio)
```cmd
cl /EHsc /std:c++17 /W4 /O2 ^
    src\main.cpp ^
    src\FileScanner.cpp ^
    src\FileClassifier.cpp ^
    src\FileMover.cpp ^
    src\Logger.cpp ^
    /Fe:desktop_cleaner.exe
```

---

## How to Run

### Basic Usage
```bash
# Run with default settings (current directory, real mode)
./desktop_cleaner

# Specify custom directory
./desktop_cleaner /path/to/desktop

# Windows example
desktop_cleaner.exe C:\Users\YourName\Desktop
```

### Command-Line Options

```bash
./desktop_cleaner [OPTIONS] [DIRECTORY]
```

| Option | Description | Default |
|--------|-------------|---------|
| `--dry-run` | Preview actions without moving files | Off |
| `--size=<MB>` | Large file threshold in MB | 100 |
| `--age=<DAYS>` | Old file threshold in days | 90 |
| `--help` | Display help message | - |

### Examples

**Dry-Run Mode** (Safe Preview)
```bash
./desktop_cleaner --dry-run ~/Desktop
```

**Custom Thresholds**
```bash
# Flag files larger than 50MB and older than 30 days
./desktop_cleaner --size=50 --age=30 ~/Desktop
```

**Production Run**
```bash
# Organize files with custom settings
./desktop_cleaner --size=200 --age=60 ~/Desktop
```

---

## Dry-Run Mode Explanation

**What is Dry-Run Mode?**
Dry-run mode simulates all file operations without actually moving any files. It allows you to:
- Preview which files will be categorized
- See where files will be moved
- Identify potential errors (permissions, locks)
- Verify configuration before committing changes

**How It Works:**
```bash
./desktop_cleaner --dry-run ~/Desktop
```

**Sample Dry-Run Output:**
```
[DRY-RUN] Would create directory: ~/Desktop/Documents
[DRY-RUN] Would move: report.pdf → Documents/
[DRY-RUN] Would move: vacation.jpg → Images/
[DRY-RUN] Large file detected: movie.mp4 (250 MB)
[DRY-RUN] Old file detected: notes.txt (last modified 120 days ago)

Total files scanned: 45
Files to be organized: 42
Files to be skipped: 3 (subdirectories/special files)
```

**Recommendation:** Always run in dry-run mode first to review actions!

---

## Safety Considerations

### Built-in Safety Features

1. **Non-Destructive Operations**
   - Files are moved, never deleted
   - Original directory structure preserved for subdirectories
   - No modification of file contents

2. **Error Recovery**
   - Graceful handling of permission errors
   - Skips locked/in-use files
   - Continues processing on individual failures

3. **Collision Handling**
   - Detects filename conflicts in target directories
   - Appends timestamp suffix to prevent overwrites
   - Logs all renaming operations

4. **Dry-Run First**
   - Strongly recommended before production runs
   - Zero risk of unintended changes
   - Full preview of operations

### User Responsibilities

⚠️ **Before Running:**
- Backup important files
- Close applications using desktop files
- Verify directory path is correct
- Test with dry-run mode first

⚠️ **Do Not:**
- Run on system directories (C:\Windows, /etc, /usr)
- Interrupt during file operations
- Run simultaneously on same directory

---

## Sample Output

### Console Output
```
========================================
  Smart Desktop Cleaner v1.0
========================================

Scanning directory: /home/user/Desktop
Dry-run mode: OFF
Large file threshold: 100 MB
Old file threshold: 90 days

[SCAN] Found 67 files

[CLASSIFY] Categorizing files...
  Documents: 15 files
  Images: 23 files
  Videos: 5 files
  Archives: 3 files
  Code: 8 files
  Others: 13 files

[ANALYZE] File Analysis:
  Large files (>100MB): 2
    - movie_1080p.mp4 (1.2 GB)
    - backup.zip (250 MB)
  
  Old files (>90 days): 7
    - old_report.pdf (modified 145 days ago)
    - notes_2023.txt (modified 312 days ago)

[CREATE] Creating category folders...
  ✓ Documents/
  ✓ Images/
  ✓ Videos/
  ✓ Archives/
  ✓ Code/
  ✓ Others/

[MOVE] Organizing files...
  ✓ report.pdf → Documents/
  ✓ vacation.jpg → Images/
  ✓ tutorial.mp4 → Videos/
  ! error_log.txt → Others/ [RENAMED: error_log_20260204_1649.txt]
  ✗ locked_file.docx [ERROR: Permission denied]

========================================
Operation Summary:
  Total files processed: 67
  Successfully moved: 64
  Failed: 3
  Warnings: 1 (filename collision)
  
  Log file: logs/cleaner_20260204_164532.log
========================================
```

### Log File (logs/cleaner_20260204_164532.log)
```
[2026-02-04 16:45:32] ===== Smart Desktop Cleaner Started =====
[2026-02-04 16:45:32] Directory: /home/user/Desktop
[2026-02-04 16:45:32] Dry-run: false
[2026-02-04 16:45:32] Size threshold: 100 MB
[2026-02-04 16:45:32] Age threshold: 90 days

[2026-02-04 16:45:33] SCAN: Found 67 files
[2026-02-04 16:45:33] CLASSIFY: report.pdf → Documents
[2026-02-04 16:45:34] MOVE SUCCESS: report.pdf → Documents/report.pdf
[2026-02-04 16:45:35] MOVE WARNING: error_log.txt renamed to error_log_20260204_1649.txt
[2026-02-04 16:45:36] MOVE FAILED: locked_file.docx [Permission denied]

[2026-02-04 16:45:40] ===== Operation Completed =====
[2026-02-04 16:45:40] Success: 64 | Failed: 3 | Warnings: 1
```

---

## Limitations

### Current Limitations

1. **Flat Directory Only**
   - Only processes files in root of target directory
   - Subdirectories are skipped (preserved as-is)
   - Future: Add recursive scanning option

2. **Extension-Based Only**
   - Classification relies on file extensions
   - Cannot analyze file contents (e.g., magic numbers)
   - Future: Add MIME type detection

3. **No Undo Functionality**
   - File moves are immediate (non-transactional)
   - Manual reversal required
   - Future: Add operation history and rollback

4. **Single Directory per Run**
   - Cannot process multiple directories simultaneously
   - Future: Add batch directory processing

5. **Basic Duplicate Detection**
   - Timestamp suffix for filename collisions
   - No content-based duplicate detection
   - Future: Add hash-based duplicate finder

### Known Issues

- **Windows Long Paths**: Paths exceeding 260 characters may fail (use `\\?\` prefix workaround)
- **Network Drives**: Performance degraded on network shares
- **Symbolic Links**: Followed (not treated as special)

---

## Future Enhancements

### Planned Features (v2.0)

1. **Advanced Categorization**
   - Content-based classification (magic numbers)
   - Custom user-defined rules (regex patterns)
   - Smart categorization using metadata

2. **Enhanced Safety**
   - Undo/rollback functionality
   - Transaction-based operations
   - Backup creation before moves

3. **Performance Optimization**
   - Multi-threaded scanning
   - Progress bar for large directories
   - Incremental organization

4. **Extended Analysis**
   - Duplicate file detection (hash-based)
   - Disk space usage reports
   - File type statistics

5. **Configuration System**
   - External config file (YAML/JSON)
   - User profiles (work, personal, minimal)
   - Persistent settings

6. **Recursive Mode**
   - Process subdirectories
   - Maintain or flatten structure
   - Depth limit controls

7. **Interactive Mode**
   - Confirm before each operation
   - Skip/rename prompts
   - Category selection UI

### Contribute Ideas
This is a learning project. Suggestions welcome!

---

## Technical Details

### Design Patterns Used
- **Single Responsibility Principle**: Each class handles one concern
- **Dependency Injection**: Logger passed to components
- **Strategy Pattern**: File classification rules
- **Defensive Programming**: Extensive error checking

### Memory Management
- RAII principles throughout
- Smart pointers where applicable
- No manual memory allocation
- Automatic resource cleanup

### Error Handling Strategy
- Try-catch blocks around filesystem operations
- Graceful degradation (continue on errors)
- Comprehensive error logging
- User-friendly error messages

---

## Interview Talking Points

When presenting this project:

1. **System Programming Concepts**
   - File I/O and filesystem operations
   - Error handling in system calls
   - Cross-platform compatibility considerations

2. **C++ Language Features**
   - C++17 `<filesystem>` library usage
   - STL containers (vector, map, unordered_set)
   - RAII and resource management
   - Exception safety

3. **Software Engineering**
   - Modular design with clear separation of concerns
   - Defensive programming practices
   - Comprehensive logging for debugging
   - Dry-run mode for safe testing

4. **Problem Solving**
   - Handling edge cases (permissions, locks, collisions)
   - Performance considerations (large directories)
   - User experience (clear output, helpful messages)

---

## License

Educational/Portfolio Project - Free to use and modify

---

## Author

Created as a fresher-level system programming portfolio project demonstrating:
- Production-quality C++ code
- File system manipulation
- Error handling and logging
- Modular software design

**Version:** 1.0  
**Date:** February 2026
