@echo off

IF NOT EXIST build mkdir build
cd build

echo "Rebuilding PS4-Eye-Driver x64 Project files..."
cmake .. -G "Visual Studio 14 2015 Win64"
pause