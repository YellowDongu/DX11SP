// 커맨드 명령어		옵션		원본 파일이 있는 위치		사본 파일을 저장할 위치

xcopy	/y		.\JusinDx3DEngine\Public\*.*					.\EngineSDK\Include\
xcopy	/y		.\JusinDx3DEngine\Bin\Shaders\*.*			.\JusinDx3DClient\Bin\Shaders\

xcopy	/y		.\JusinDx3DEngine\Bin\JusinDx3DEngine.dll		.\JusinDx3DClient\Bin\
xcopy	/y		.\JusinDx3DEngine\Bin\JusinDx3DEngine.lib		.\EngineSDK\Lib\