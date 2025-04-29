#include "Foundation.h"
#include "DefaultCubeShader.h"

using namespace Engine;


DefaultCubeShader::DefaultCubeShader(ID3D11Device*& _device, ID3D11DeviceContext*& _context) : Shader(_device, _context)
{
}

void DefaultCubeShader::Free(void)
{
	Shader::Free();
}

DefaultCubeShader* DefaultCubeShader::Create(ID3D11Device*& device, ID3D11DeviceContext*& context)
{
	DefaultCubeShader* newInstance = new DefaultCubeShader(device, context);

	if (FAILED(newInstance->LoadFXshader(L"../Bin/Shaders/DefaultCubeShader.hlsl", CubeVertex::inputLayout)))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}

	return newInstance;
}

HRESULT DefaultCubeShader::CreateBuffers(ID3D11Device* device, std::map<std::wstring, ID3D11Buffer*>& constantBuffers)
{
	return S_OK;
}

void DefaultCubeShader::SetNamePreset(void)
{
	shaderFile = L"DefaultCubeShader.hlsl";
	shaderFileA = "DefaultCubeShader.hlsl";

}
