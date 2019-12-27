#!/usr/bin/env python
"""
Run clang-format on all source and header files in directories that enforce clang formatting
"""

from __future__ import print_function
import os

SOURCE_FOLDERS = ['include', 'src', 'test']
SUFFIXES = ('.cc', '.cpp', '.h', '.hh', '.hpp')

def find_source_files(clang_dirs):
    """
    Find all source files and headers in the clang-enforced directories
    Args:
        clang_dirs (list): List of directories to search
    Returns:
        list: The list of source and header files in the given directories
    """
    source_files = []
    for dirname in clang_dirs:
        for root, _, files in os.walk(dirname):
            source_files.extend([os.path.join(root, filename) for filename in files if filename.endswith(SUFFIXES)])
    return sorted(source_files)

def run_clang(clang_files):
    """
    Run clang-format, overwrite existing files if formatting changes
    Args:
        clang_files (list): List of files to format
    """
    for filename in clang_files:
        with open(filename, 'r') as infile:
            indata = infile.read()
        outdata = os.popen('clang-format-3.9 -style=file '+filename).read()
        if indata != outdata:
            print('\033[1m\033[33mReformatting {}\033[0m'.format(filename[2:]))
            with open(filename, 'w') as outfile:
                outfile.write(outdata)

def main():
    """Find files and format them"""
    source_files = find_source_files(SOURCE_FOLDERS)
    run_clang(source_files)

if __name__ == '__main__':
    main()
