rmdir /s /q "ipch"
rmdir /s /q "Debug"
rmdir /s /q "Release"

rmdir /s /q .\Client\Include\Debug
rmdir /s /q .\Client\Include\ipch
rmdir /s /q .\Client\Include\Release
rmdir /s /q .\Client\Include\x64

rmdir /s /q .\Engine\System\Include\x64\Debug
rmdir /s /q .\Engine\Utility\Include\x64\Debug

del /f /q .\.vs\Frame150\v14\
del /f /q .\.vs\Frame150\v17\ipch
del /f /q .\Client\Bin\Client.exe
del /f /q .\Client\Bin\Client.ilk
del /f /q .\Client\Bin\Client.pdb
del /f /q Frame150.VC.db
