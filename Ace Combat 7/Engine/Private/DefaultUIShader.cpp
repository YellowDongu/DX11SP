#include "Foundation.h"
#include "DefaultUIShader.h"


using namespace Engine;

DefaultUIShader::DefaultUIShader(ID3D11Device* device, ID3D11DeviceContext* context) : Shader(device, context)
{
}

void DefaultUIShader::Free(void)
{
	Shader::Free();
}

DefaultUIShader* DefaultUIShader::Create(ID3D11Device* device, ID3D11DeviceContext* context)
{
	DefaultUIShader* newInstance = new DefaultUIShader(device, context);

	if (FAILED(newInstance->LoadFXshader(L"../Bin/Shaders/DefaultUIShader.hlsl", UIVertex::inputLayout)))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}

	return newInstance;
}

void DefaultUIShader::SetNamePreset(void)
{
	shaderFile = L"DefaultUIShader.hlsl";
	shaderFileA = "DefaultUIShader.hlsl";
}
