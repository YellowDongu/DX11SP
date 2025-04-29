#include "pch.h"
#include "UIShader.h"


UIShader::UIShader(ID3D11Device* device, ID3D11DeviceContext* context) : Shader(device, context)
{
}

void UIShader::Free(void)
{
	Engine::Shader::Free();
}

UIShader* UIShader::Create(ID3D11Device* device, ID3D11DeviceContext* context)
{
	UIShader* newInstance = new UIShader(device, context);
	//newInstance->SetNamePreset();
	if (FAILED(newInstance->LoadFXshader(L"UIShader.hlsl", UIVertex::inputLayout)))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
	return newInstance;
}

void UIShader::SetNamePreset(void)
{
	shaderFile = L"UIShader.hlsl";
	shaderFileA = "UIShader.hlsl";
	boneMatrixBuffer = L"BoneBuffer";
	boneMatrixBufferA = "BoneBuffer";
	MaterialConstantBuffer = L"MaterialBufferInput";
	MaterialConstantBufferA = "MaterialBufferInput";
	viewProjectionMatrix = L"viewProjectionMatrix";
	viewProjectionMatrixA = "viewProjectionMatrix";
	worldMatrix = L"worldMatrix";
	worldMatrixA = "worldMatrix";
}
