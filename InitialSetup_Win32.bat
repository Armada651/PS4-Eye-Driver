@echo off
setlocal

::Clean up the old build folder
IF EXIST build (
del /f /s /q build > nul
rmdir /s /q build
)

::Clean up the old deps folder
IF EXIST deps (
del /f /s /q deps > nul
rmdir /s /q deps
)

:: Add MSVC build tools to the path
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x86

:: Compile the DEBUG|Win32 and RELEASE|Win32 builds of libusb
pushd thirdparty\libusb\msvc\
echo "Building libusb DEBUG|Win32..."
MSBuild.exe libusbx_2015.sln /p:configuration=DEBUG /p:Platform="Win32" /t:Clean;Build 
echo "Building libusb RELEASE|Win32..."
MSBuild.exe libusbx_2015.sln /p:configuration=RELEASE /p:Platform="Win32" /t:Clean;Build
popd

:: Generate the project files
call GenerateProjectFiles_Win32.bat
pause
goto exit

:failure
goto exit

:exit
endlocal
