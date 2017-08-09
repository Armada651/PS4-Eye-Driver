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
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" amd64

:: Compile the DEBUG|x64 and RELEASE|x64 builds of libusb
pushd thirdparty\libusb\msvc\
echo "Building libusb DEBUG|x64..."
MSBuild.exe libusbx_2015.sln /tv:14.0 /p:configuration=DEBUG /p:Platform="x64" /t:Clean;Build 
echo "Building libusb RELEASE|x64..."
MSBuild.exe libusbx_2015.sln /tv:14.0 /p:configuration=RELEASE /p:Platform="x64" /t:Clean;Build
popd

:: Generate the project files
call GenerateProjectFiles_X64.bat
pause
goto exit

:failure
goto exit

:exit
endlocal
