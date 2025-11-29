@echo off
setlocal

REM --- Configuration ---
REM Set this to your Unreal Engine installation directory
set "UE_PATH=D:\GameDev\UE5"
set "PROJECT_NAME=StateTreePlayground"
set "UPROJECT_PATH=%~dp0%PROJECT_NAME%.uproject"

REM Check if UE_PATH exists
if not exist "%UE_PATH%" (
    echo Error: Unreal Engine path not found at "%UE_PATH%"
    echo Please edit this script and set the correct UE_PATH.
    pause
    exit /b 1
)

REM --- Build ---
echo Building %PROJECT_NAME%...
set "UBT_PATH=%UE_PATH%\Engine\Build\BatchFiles\Build.bat"

if not exist "%UBT_PATH%" (
    echo Error: Build.bat not found at "%UBT_PATH%"
    pause
    exit /b 1
)

call "%UBT_PATH%" %PROJECT_NAME%Editor Win64 Development -Project="%UPROJECT_PATH%" -WaitMutex -FromMsBuild
if %ERRORLEVEL% NEQ 0 (
    echo Build failed!
    pause
    exit /b %ERRORLEVEL%
)

REM --- Run ---
echo Starting Unreal Editor...
set "EDITOR_PATH=%UE_PATH%\Engine\Binaries\Win64\UnrealEditor.exe"

if not exist "%EDITOR_PATH%" (
    echo Error: UnrealEditor.exe not found at "%EDITOR_PATH%"
    pause
    exit /b 1
)

start "" "%EDITOR_PATH%" "%UPROJECT_PATH%"

echo Done.
endlocal
