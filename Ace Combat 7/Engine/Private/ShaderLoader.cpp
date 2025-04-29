#include "Foundation.h"
#include "ShaderLoader.h"


using namespace Engine;



ShaderLoader::ShaderLoader(ID3D11Device*& _device, ID3D11DeviceContext*& _context) : device(_device), context(_context), currentShader(nullptr)
{
}

void ShaderLoader::Free(void)
{
	for (auto shader : shaders)
	{
		Base::Destroy(shader.second);
	}


}

ShaderLoader* ShaderLoader::Create(ID3D11Device*& device, ID3D11DeviceContext*& context)
{
	ShaderLoader* newInstance = new ShaderLoader(device, context);
	return newInstance;
}


Shader* ShaderLoader::GetShader(const std::wstring& key)
{
	auto shader = shaders.find(key);
	if (shader == shaders.end())
		return nullptr;

	return shader->second;
}

HRESULT ShaderLoader::SetShader(const std::wstring& key)
{
	auto shader = shaders.find(key);
	if (shader == shaders.end())
		return E_FAIL;

	if (currentShader == shader->second)
		return S_OK;

	currentShader = shader->second;
	currentShaderName = shader->first;
	currentShader->SetShader();
	currentShader->ApplyShader();
	return S_OK;
}

HRESULT ShaderLoader::SetShader(Shader* shader)
{
	currentShader = shader;
	currentShaderName = shader->shaderFile;
	currentShader->SetShader();
	currentShader->ApplyShader();
	return S_OK;
}

HRESULT ShaderLoader::AddShader(const std::wstring& key, Shader* newShader)
{
	auto shader = shaders.find(key);
	if (shader != shaders.end())
		return E_FAIL;

	shaders[key] = newShader;
	return S_OK;
}

void ShaderLoader::SetViewProjectionMatrix(const Matrix& matrix)
{
	for (auto& shader : shaders)
	{
		shader.second->BindMatrix(shader.second->viewProjectionMatrixA, matrix);
	}
}

Shader* ShaderLoader::LoadShader(const std::wstring& filePath, D3D11_INPUT_ELEMENT_DESC** elements, UINT elementsSize)
{
	return LoadShader(device, context, filePath, elements, elementsSize);
}
Shader* ShaderLoader::LoadShader(const std::wstring& filePath, std::vector<D3D11_INPUT_ELEMENT_DESC>& elements)
{
	return LoadShader(device, context, filePath, elements);
}

Shader* ShaderLoader::LoadShader(ID3D11Device*& device, ID3D11DeviceContext*& context, const std::wstring& filePath, D3D11_INPUT_ELEMENT_DESC** elements, UINT elementsSize)
{
	auto shader = shaders.find(filePath);
	if (shader != shaders.end())
		return shader->second;


	Shader* newShader = Shader::Create(device, context, filePath, elements, elementsSize);
	if (newShader == nullptr)
		return nullptr;

	shaders[filePath] = newShader;
	return newShader;
}

Shader* ShaderLoader::LoadShader(ID3D11Device*& device, ID3D11DeviceContext*& context, const std::wstring& filePath, std::vector<D3D11_INPUT_ELEMENT_DESC>& elements)
{
	auto shader = shaders.find(filePath);
	if (shader != shaders.end())
		return shader->second;

	Shader* newShader = Shader::Create(device, context, filePath, elements);
	if (newShader == nullptr)
		return nullptr;

	shaders[filePath] = newShader;
	return newShader;
}
