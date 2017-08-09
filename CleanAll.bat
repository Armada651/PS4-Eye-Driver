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

::Clean up the old install folder
IF EXIST Win32 (
del /f /s /q Win32 > nul
rmdir /s /q Win32
)

IF EXIST Win32 (
del /f /s /q x64 > nul
rmdir /s /q x64
)

:: Add MSVC build tools to the path
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x86

:: Compile the DEBUG|Win32 and RELEASE|Win32 builds of libusb
pushd thirdparty\libusb\msvc\
echo "Cleaning libusb DEBUG|Win32..."
MSBuild.exe libusbx_2015.sln /p:configuration=DEBUG /p:Platform="Win32" /t:Clean
echo "Cleaning libusb RELEASE|Win32..."
MSBuild.exe libusbx_2015.sln /p:configuration=RELEASE /p:Platform="Win32" /t:Clean
echo "Cleaning libusb DEBUG|x64..."
MSBuild.exe libusbx_2015.sln /p:configuration=DEBUG /p:Platform="x64" /t:Clean
echo "Cleaning libusb RELEASE|x64..."
MSBuild.exe libusbx_2015.sln /p:configuration=RELEASE /p:Platform="x64" /t:Clean
popd

:failure
goto exit

:exit
endlocal
