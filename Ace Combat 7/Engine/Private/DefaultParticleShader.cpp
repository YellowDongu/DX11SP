#include "Foundation.h"
#include "DefaultParticleShader.h"

using namespace Engine;

DefaultParticleShader::DefaultParticleShader(ID3D11Device*& device, ID3D11DeviceContext*& context) : Shader(device, context)
{
}

void DefaultParticleShader::Free(void)
{
	Shader::Free();
}

DefaultParticleShader* DefaultParticleShader::Create(ID3D11Device*& device, ID3D11DeviceContext*& context)
{
	DefaultParticleShader* newInstance = new DefaultParticleShader(device, context);

	std::vector<D3D11_INPUT_ELEMENT_DESC> defaultParticleBufferElements = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{"WORLD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{"WORLD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{"WORLD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{"WORLD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },

		{"TEXCOORD", 1, DXGI_FORMAT_R32_FLOAT,		2, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{"TEXCOORD", 2, DXGI_FORMAT_R32G32_FLOAT,	2, 4, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	};

	if (FAILED(newInstance->LoadFXshader(L"../Bin/Shaders/DefaultParticleShader.hlsl", defaultParticleBufferElements)))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}

	return newInstance;
}

void DefaultParticleShader::SetNamePreset(void)
{
	shaderFile = L"DefaultModelVSShader.hlsl";
	shaderFileA = "DefaultModelVSShader.hlsl";
	boneMatrixBuffer = L"BoneBuffer";
	boneMatrixBufferA = "BoneBuffer";
	MaterialConstantBuffer = L"MaterialBufferInput";
	MaterialConstantBufferA = "MaterialBufferInput";
}
