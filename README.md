# Threads
A Windows desktop application written in C++ using Microsoft Foundation Classes (MFC).

Overview

The application provides bidirectional conversion between arbitrary binary files and C++ source files containing byte array definitions.

It generates C++ source code from binary data and can reconstruct the original binary file from a compatible C++ byte array definition.

Features

Binary-to-C++ conversion (BIN → CPP)

C++-to-binary conversion (CPP → BIN)

Hexadecimal representation of binary data

16 byte values per generated source line

Token-based parsing of C++ array definitions

Background conversion using a worker thread

Progress bar and operation status information

Automatic output filename generation

Persistent file paths using the Windows Registry

MFC graphical user interface

Technologies

C++

Microsoft Foundation Classes (MFC)

Win32 API

Windows Registry

File I/O

Multithreading

Implementation

The conversion logic is implemented using procedural C-style functions and MFC file handling.

The C++ input parser processes tokens separated by delimiters, allowing different whitespace and line-break arrangements in the input file.
