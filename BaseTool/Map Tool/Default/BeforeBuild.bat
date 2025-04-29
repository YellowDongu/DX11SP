@echo off
set SOURCE_FILE=..\..\..\.\Ace Combat 7\Engine\Bin\Engine.dll
set OTHER_SOURCE_FILE=..\..\..\Ace Combat 7\Engine\Bin\Shaders\*.*
set DEST_DIR=..\Bin
set OTHER_DEST_DIR=..\Bin\Shaders

if not exist "%SOURCE_FILE%" (
    echo err: no file found %SOURCE_FILE%
)
if not exist "%OTHER_SOURCE_FILE%" (
    echo err: no file found %OTHER_SOURCE_FILE%
)

copy "%SOURCE_FILE%" "%DEST_DIR%\"
if %errorlevel% neq 0 (
    echo copy failed!
    pause
)

copy "%OTHER_SOURCE_FILE%" "%OTHER_DEST_DIR%\"
if %errorlevel% neq 0 (
    echo copy failed!
    pause
)
    pause