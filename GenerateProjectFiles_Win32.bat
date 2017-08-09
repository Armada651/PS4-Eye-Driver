@echo off

IF NOT EXIST build mkdir build
cd build

echo "Rebuilding PS4-Eye-Driver Win32 Project files..."
cmake .. -G "Visual Studio 14 2015"
pause