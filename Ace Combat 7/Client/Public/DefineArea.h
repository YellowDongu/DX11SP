#pragma once


extern int windowSizeX;
extern int windowSizeY;
extern FLOAT worldReSizeAmount;
extern HWND hWnd;
extern HINSTANCE hInstance;



#include "Shader.h"
extern Engine::Shader* shader;


inline FLOAT ConvertFeetToWorld(FLOAT feet) { return feet / 30.2f * worldReSizeAmount; }
inline FLOAT ConvertWorldToFeet(FLOAT world) { return world * 30.2f / worldReSizeAmount; }

inline xmMatrix AircraftGlobalXMMatrix(void) { return DirectX::XMMatrixAffineTransformation(DirectX::XMVectorSet(0.01f, 0.01f, 0.01f, 0.0f), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(-90.0f), DirectX::XMConvertToRadians(-90.0f), DirectX::XMConvertToRadians(0.0f)), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f)); }
inline void AircraftGlobalMatrix(Matrix& out) { DirectX::XMStoreFloat4x4(&out, DirectX::XMMatrixAffineTransformation(DirectX::XMVectorSet(0.01f, 0.01f, 0.01f, 0.0f), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(-90.0f), DirectX::XMConvertToRadians(-90.0f), DirectX::XMConvertToRadians(0.0f)), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f))); }
inline xmMatrix GearGlobalMatrix(void) { return DirectX::XMMatrixAffineTransformation(DirectX::XMVectorSet(0.01f, 0.01f, 0.01f, 0.0f), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(0.0f), DirectX::XMConvertToRadians(-90.0f), DirectX::XMConvertToRadians(0.0f)), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f)); }
inline void GearGlobalMatrix(Matrix& out) { DirectX::XMStoreFloat4x4(&out, DirectX::XMMatrixAffineTransformation(DirectX::XMVectorSet(0.01f, 0.01f, 0.01f, 0.0f), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(0.0f), DirectX::XMConvertToRadians(-90.0f), DirectX::XMConvertToRadians(0.0f)), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f))); }


inline HRESULT ConvertModel(std::string loadPath, std::wstring savePath, Matrix offsestMatrix)
{
	ModelConverter aircraftConvert;
	return aircraftConvert.ConvertFBX(loadPath, savePath, offsestMatrix);
}

inline HRESULT ConvertModel(std::string loadPath, std::wstring savePath, fxmMatrix offsetMatrix, bool staticModel = false)
{
	ModelConverter aircraftConvert;
	Matrix aircraftMatrix;
	DirectX::XMStoreFloat4x4(&aircraftMatrix, offsetMatrix);
	return aircraftConvert.ConvertFBX(loadPath, savePath, aircraftMatrix, staticModel);
}