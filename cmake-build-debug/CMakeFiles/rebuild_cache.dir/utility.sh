set -e

cd /cygdrive/c/Users/nated/os-final-proj/cmake-build-debug
/cygdrive/c/Users/nated/AppData/Local/JetBrains/CLion2023.3/cygwin_cmake/bin/cmake.exe --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
