#!/usr/bin/env python3

import os
import shutil
import time
from pathlib import Path
import logging
from datetime import datetime

# Configure logging
logging.basicConfig(
    level=logging.INFO,
    format='%(asctime)s - %(message)s',
    datefmt='%Y-%m-%d %H:%M:%S'
)

# Configuration
DOWNLOADS_DIR = str(Path.home() / "Downloads")
DEST_DIRS = {
    "Images": [".jpg", ".jpeg", ".png", ".gif", ".bmp", ".tiff", ".webp", ".svg"],
    "Documents": [".pdf", ".doc", ".docx", ".txt", ".odt", ".rtf", ".tex", ".ppt", ".pptx", ".xls", ".xlsx", ".csv"],
    "Audio": [".mp3", ".wav", ".flac", ".aac", ".ogg", ".m4a"],
    "Video": [".mp4", ".mkv", ".avi", ".mov", ".wmv", ".flv", ".webm"],
    "Archives": [".zip", ".rar", ".tar", ".gz", ".7z", ".bz2", ".xz"],
    "Code": [".py", ".java", ".js", ".html", ".css", ".cpp", ".c", ".h", ".php", ".rb", ".go", ".rs", ".sh", ".json", ".xml"],
    "Executables": [".deb", ".exe", ".msi", ".app", ".AppImage", ".run", ".bin"],
    "Misc": []  # For any other file types
}

def setup_directories():
    """Create destination directories if they don't exist"""
    for dir_name in DEST_DIRS.keys():
        dir_path = os.path.join(DOWNLOADS_DIR, dir_name)
        if not os.path.exists(dir_path):
            os.makedirs(dir_path)
            logging.info(f"Created directory: {dir_path}")

def get_destination_folder(file_extension):
    """Determine the appropriate destination folder based on file extension"""
    for folder, extensions in DEST_DIRS.items():
        if file_extension.lower() in extensions:
            return folder
    return "Misc"  # Default destination if extension doesn't match any category

def clean_downloads():
    """Move files from Downloads to their appropriate destinations"""
    setup_directories()
    
    # Get all files in the Downloads directory
    files = [f for f in os.listdir(DOWNLOADS_DIR) if os.path.isfile(os.path.join(DOWNLOADS_DIR, f))]
    
    if not files:
        logging.info("No files to organize in Downloads directory")
        return
    
    files_moved = 0
    
    for filename in files:
        # Skip hidden files and the script itself
        if filename.startswith('.') or filename == os.path.basename(__file__):
            continue
            
        source_path = os.path.join(DOWNLOADS_DIR, filename)
        
        # Determine file extension
        file_extension = os.path.splitext(filename)[1]
        
        # Get destination folder
        dest_folder = get_destination_folder(file_extension)
        dest_path = os.path.join(DOWNLOADS_DIR, dest_folder, filename)
        
        # Handle file name conflicts
        if os.path.exists(dest_path):
            name, ext = os.path.splitext(filename)
            timestamp = datetime.now().strftime("%Y%m%d%H%M%S")
            new_filename = f"{name}_{timestamp}{ext}"
            dest_path = os.path.join(DOWNLOADS_DIR, dest_folder, new_filename)
        
        try:
            shutil.move(source_path, dest_path)
            logging.info(f"Moved: {filename} → {dest_folder}/")
            files_moved += 1
        except Exception as e:
            logging.error(f"Error moving {filename}: {str(e)}")
    
    logging.info(f"Organization complete. {files_moved} files moved.")

def run_as_daemon(interval_minutes=30):
    """Run the cleaner periodically"""
    logging.info(f"Smart Desktop Cleaner started. Running every {interval_minutes} minutes.")
    try:
        while True:
            clean_downloads()
            time.sleep(interval_minutes * 60)
    except KeyboardInterrupt:
        logging.info("Smart Desktop Cleaner stopped manually.")

if __name__ == "__main__":
    import argparse
    
    parser = argparse.ArgumentParser(description="Smart Desktop Cleaner for organizing Downloads folder")
    parser.add_argument("--daemon", action="store_true", help="Run as a background process")
    parser.add_argument("--interval", type=int, default=30, help="Interval in minutes for daemon mode")
    parser.add_argument("--run-once", action="store_true", help="Run once and exit")
    
    args = parser.parse_args()
    
    if args.run_once:
        clean_downloads()
    elif args.daemon:
        run_as_daemon(args.interval)
    else:
        clean_downloads()
        logging.info("Tip: Use --daemon flag to run continuously in the background")