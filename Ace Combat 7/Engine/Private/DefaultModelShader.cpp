#include "Foundation.h"
#include "DefaultModelShader.h"

using namespace Engine;


DefaultModelShader::DefaultModelShader(ID3D11Device*& device, ID3D11DeviceContext*& context) : Shader(device, context)
{
}

void DefaultModelShader::Free(void)
{
	Shader::Free();
}

DefaultModelShader* DefaultModelShader::Create(ID3D11Device*& device, ID3D11DeviceContext*& context)
{
	DefaultModelShader* newInstance = new DefaultModelShader(device, context);

	if (FAILED(newInstance->LoadFXshader(L"../Bin/Shaders/DefaultModelShader.hlsl", ModelVertex::inputLayout)))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}

	return newInstance;
}

HRESULT DefaultModelShader::CreateBuffers(ID3D11Device* device, std::map<std::wstring, ID3D11Buffer*>& constantBuffers)
{
	ID3D11Buffer* newBuffer = nullptr;
	HRESULT result;
	D3D11_BUFFER_DESC cbDesc = {};
	ZeroMemory(&cbDesc, sizeof(D3D11_BUFFER_DESC));

	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.ByteWidth = sizeof(BoneBuffer);
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	result = device->CreateBuffer(&cbDesc, nullptr, &newBuffer);
	ConstantBuffer(boneMatrixBuffer, newBuffer);

	ZeroMemory(&cbDesc, sizeof(D3D11_BUFFER_DESC));

	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.ByteWidth = sizeof(MaterialBufferType);
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	result = device->CreateBuffer(&cbDesc, nullptr, &newBuffer);
	ConstantBuffer(MaterialConstantBuffer, newBuffer);
	return S_OK;
}

void DefaultModelShader::SetNamePreset(void)
{
	shaderFile = L"DefaultModelVSShader.hlsl";
	shaderFileA = "DefaultModelVSShader.hlsl";
	boneMatrixBuffer = L"BoneBuffer";
	boneMatrixBufferA = "BoneBuffer";
	MaterialConstantBuffer = L"MaterialBufferInput";
	MaterialConstantBufferA = "MaterialBufferInput";
}
