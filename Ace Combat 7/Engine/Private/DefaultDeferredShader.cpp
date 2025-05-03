#include "Foundation.h"
#include "DefaultDeferredShader.h"

using namespace Engine;

DefaultDeferredShader::DefaultDeferredShader(ID3D11Device*& device, ID3D11DeviceContext*& context) : Shader(device, context)
{
}

void DefaultDeferredShader::Free(void)
{
	Shader::Free();
}

DefaultDeferredShader* DefaultDeferredShader::Create(ID3D11Device*& device, ID3D11DeviceContext*& context)
{
	DefaultDeferredShader* newInstance = new DefaultDeferredShader(device, context);

	if (FAILED(newInstance->LoadFXshader(L"../Bin/Shaders/DefaultDeferredShader.hlsl", SimpleVertex::inputLayout)))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}

	return newInstance;
}

void DefaultDeferredShader::SetNamePreset(void)
{
	shaderFile = L"DefaultDeferredShader.hlsl";
	shaderFileA = "DefaultDeferredShader.hlsl";
	boneMatrixBuffer = L"";
	boneMatrixBufferA = "";
	MaterialConstantBuffer = L"";
	MaterialConstantBufferA = "";
}
