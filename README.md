# The Yoyo Project

The Yoyo project has initially been started as an idle project in 2022 to visualize some 
some software engineering principles and architectural principles.

With this software the user should be able to design their own Graphical User Interface, which
is able to connect to a connected (wire/usb) embedded device, display and control said device
using that GUI.

This version of the software includes some basic element types and exposes an SDK with which
the user can create their own elements (GUI, connectivity and otherwise) and load them as plugins.

It contains a command-framework (for undo functionality) and an IO-framework built for extensibility.

This software follows the SOLID principles. It contains unit-tests (using GTest), mainly using
Test-Driven-Development. See the `doc/` directory for doxygen documentation for developers, plugin-
developers and users.

Development time to reach this state: roughly 3 months.

**Addendum 2026**

This project was originally developed on Windows using mingw-8.1, C++-17 and Qt 5.15.

The current state was ported to Linux, using gcc/clang, conan and Qt 6.10

## Build 

Yoyo uses CMake as meta build system.

Language is C++-17.

### Dependencies

You need to have the following libraries on your system to build Yoyo:

| Lib        | Version (original 2022)                | Comment |
|:-----------|:--------------------------------------:|:--------|
|CMake       | 3.21+                                  | tested with 4.4 |
|Qt          | 5.15.2                                 | currently using 6.10 |
|Boost       | 1.78.0                                 | Components: uuids, signals2, filesystem|
|Google Test |16f637fbf4ffc3f7a01fa4eceb7906634565242f| see tests/CMakeLists.txt|
|Doxygen     |                                        ||
|clang-format| 12.0.0                                 | Formatting tool (currently 22.1) |
|clang-tidy  |                                        | Static code analysis|

#### Boost libraries

Please note, that currently the filesystem library from Boost is used, even though in C++-17, the
filesystem library is already in the standard library. Unfortunately this is necessary, since the
compiler used on Windows is minGW 8.1. In this version, there is still a bug with 
std::filesystem::path, which is resolved with minGW 9. Until then we need to use the filesystem
library from Boost. This makes it necessary to compile the Boost libraries.

Here is a step by step guide how to build the boost libraries for Windows:
https://gist.github.com/sim642/29caef3cc8afaa273ce6

On Linuxoid systems, you should be able to install them with your package manager.

### IDE

Even though everybody is free to use their prefered IDE, we recommend using Qt-Creator.
(https://doc.qt.io/qtcreator/creator-project-cmake.html)

Please configure the following CMake variables (the rest will be handled by Qt-Creator):

| Variable | Value | Comment |
|:---------|:------|:--------|
|CMAKE_MAKE_PROGRAM|C:/path/to/ninja/ninja.exe| Optional, for using ninja-build, which is much faster than ordinary make|
|CMAKE_PREFIX_PATH|%{Qt:QT_INSTALL_PREFIX};C:\path\to\install\of\boost\boost_1_78| Path to the boost installation folder|

Again, this is only necessary for Windows builds, on Linuxoid systems, the installation paths are
predetermined and CMake is bound to find it without help.

**Addendum (2026)**
`CMakePresets.json` was added setting the required build variables. If it doesn't work for you, please
consult this file to see which variables to set and add your own presets in `CMakeUserPresets.json`.

Since using conan as package manager, Qt-Creator is no more the IDE of choice (is seeking for Kits
conan actually takes care of), but we recommend VS-Code with extensions for C++, CMake, clangd, clang-format, etc.
