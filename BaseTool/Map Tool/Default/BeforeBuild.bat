@echo off
set SOURCE_FILE=..\..\..\Zero Sievert\Client\Bin\System.dll
set OTHER_SOURCE_FILE=..\..\..\Zero Sievert\Client\Bin\Utility.dll
set DEST_DIR=..\Bin

if not exist "%SOURCE_FILE%" (
    echo err: no file found %SOURCE_FILE%
)
if not exist "%OTHER_SOURCE_FILE%" (
    echo err: no file found %OTHER_SOURCE_FILE%
)

copy "%SOURCE_FILE%" "%DEST_DIR%\"
if %errorlevel% neq 0 (
    echo copy failed!
)

copy "%OTHER_SOURCE_FILE%" "%DEST_DIR%\"
if %errorlevel% neq 0 (
    echo copy failed!
)