#include "Foundation.h"
#include "Shader.h"

using namespace Engine;

Shader::Shader(ID3D11Device*& _device, ID3D11DeviceContext*& _context) : device(_device), context(_context)
{
	device->AddRef();
	context->AddRef();
}

void Shader::Free(void)
{
	for (auto& buffer : constantBuffers)
	{
		buffer.second->Release();
	}
	constantBuffers.clear();
	for (auto& inputLayout : inputLayouts)
	{
		inputLayout->Release();
	}
	inputLayouts.clear();

	if (effect)
		effect->Release();
	if (device)
		device->Release();
	if (context)
		context->Release();

	effect = nullptr;
	device = nullptr;
	context = nullptr;
}

Shader* Shader::Create(ID3D11Device*& device, ID3D11DeviceContext*& context, const std::wstring& fileName, D3D11_INPUT_ELEMENT_DESC** elements, UINT elementsSize)
{
	Shader* newInstance = new Shader(device, context);

	if (FAILED(newInstance->LoadFXshader(fileName, elements, elementsSize)))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}

	return newInstance;
}

Shader* Shader::Create(ID3D11Device*& device, ID3D11DeviceContext*& context, const std::wstring& fileName, std::vector<D3D11_INPUT_ELEMENT_DESC> elements)
{
	Shader* newInstance = new Shader(device, context);

	if (FAILED(newInstance->LoadFXshader(fileName, elements)))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}

	return newInstance;
}

HRESULT Shader::LoadFXshader(const std::wstring& fileName, D3D11_INPUT_ELEMENT_DESC** elements, UINT elementsSize)
{
	SetNamePreset();
	layoutIndex = 0;
	passIndex = 0;
	HRESULT result = S_OK;
	result = LoadShader(device, fileName, elements, elementsSize, effect, passNumber, inputLayouts);
	if (FAILED(result))
		return result;

	result = CreateBuffers(device, constantBuffers);
	return result;
}

HRESULT Shader::LoadFXshader(const std::wstring& fileName, std::vector<D3D11_INPUT_ELEMENT_DESC> elements)
{
	SetNamePreset();
	layoutIndex = 0;
	passIndex = 0;
	D3D11_INPUT_ELEMENT_DESC* elementsData = elements.data();
	HRESULT result = S_OK;
	result = LoadShader(device, fileName, &elementsData, (UINT)elements.size(), effect, passNumber, inputLayouts);
	if (FAILED(result))
		return result;

	result = CreateBuffers(device, constantBuffers);
	return result;
}

HRESULT Shader::Render(ID3D11Buffer* indexBuffer, ID3D11Buffer* vertexBuffer, UINT stride, UINT offset, DXGI_FORMAT indexFormat, D3D11_PRIMITIVE_TOPOLOGY topology)
{
	context->IASetInputLayout(inputLayouts[layoutIndex]);

	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(indexBuffer, indexFormat, 0);
	context->IASetPrimitiveTopology(topology);

	ID3DX11EffectPass* currentPass = effect->GetTechniqueByIndex(techniqueIndex)->GetPassByIndex(passIndex);
	if (currentPass == nullptr)
		return E_FAIL;

	return currentPass->Apply(0, context);
}

HRESULT Engine::Shader::Render(ID3D11Buffer* indexBuffer, ID3D11Buffer** vertexBuffer, UINT* stride, UINT vertexBufferCount, UINT* offset, DXGI_FORMAT indexFormat, D3D11_PRIMITIVE_TOPOLOGY topology)
{
	context->IASetInputLayout(inputLayouts[layoutIndex]);

	context->IASetVertexBuffers(0, vertexBufferCount, vertexBuffer, stride, offset);
	context->IASetIndexBuffer(indexBuffer, indexFormat, 0);
	context->IASetPrimitiveTopology(topology);

	ID3DX11EffectPass* currentPass = effect->GetTechniqueByIndex(techniqueIndex)->GetPassByIndex(passIndex);
	if (currentPass == nullptr)
		return E_FAIL;

	return currentPass->Apply(0, context);
}

HRESULT Engine::Shader::Render(ID3D11Buffer* indexBuffer, std::vector<ID3D11Buffer*>& vertexBuffer, std::vector<UINT>& stride, UINT vertexBufferCount, std::vector<UINT>& offset, DXGI_FORMAT indexFormat, D3D11_PRIMITIVE_TOPOLOGY topology)
{
	context->IASetInputLayout(inputLayouts[layoutIndex]);

	context->IASetVertexBuffers(0, vertexBufferCount, vertexBuffer.data(), stride.data(), offset.data());
	context->IASetIndexBuffer(indexBuffer, indexFormat, 0);
	context->IASetPrimitiveTopology(topology);

	ID3DX11EffectPass* currentPass = effect->GetTechniqueByIndex(techniqueIndex)->GetPassByIndex(passIndex);
	if (currentPass == nullptr)
		return E_FAIL;

	return currentPass->Apply(0, context);
}

HRESULT Shader::SetShader(void)
{
	if (layoutIndex >= inputLayouts.size())
		return E_FAIL;

	context->IASetInputLayout(inputLayouts[layoutIndex]);

	pass = effect->GetTechniqueByIndex(0)->GetPassByIndex(passIndex);
	if (pass == nullptr)
		return E_FAIL;

	return S_OK;
}

HRESULT Shader::ApplyShader(void)
{
	return pass->Apply(0, context);
}

HRESULT Shader::BindMatrix(const std::string& variableName, const Matrix& matrix)
{
	ID3DX11EffectVariable* variable = effect->GetVariableByName(variableName.c_str());
	if (variable == nullptr)
		return E_FAIL;

	ID3DX11EffectMatrixVariable* matrixVariable = variable->AsMatrix();
	if (matrixVariable == nullptr)
		return E_FAIL;

	return matrixVariable->SetMatrix(reinterpret_cast<const FLOAT*>(&matrix));
}

HRESULT Shader::BindVariable(const std::string& variableName, const void* pointer, UINT size)
{
	ID3DX11EffectVariable* variable = effect->GetVariableByName(variableName.c_str());
	if (nullptr == variable)
		return E_FAIL;

	return variable->SetRawValue(pointer, 0, size);
}

HRESULT Shader::BindConstantBuffer(const std::string& variableName, ID3D11Buffer* buffer)
{
	ID3DX11EffectConstantBuffer* constantBuffer = effect->GetConstantBufferByName(variableName.c_str());
	if (constantBuffer == nullptr)
		return E_FAIL;

	constantBuffer->SetConstantBuffer(buffer);

	return S_OK;
}

HRESULT Shader::BindTexture(const std::string& variableName, ID3D11ShaderResourceView* texture)
{
	ID3DX11EffectShaderResourceVariable* pTextureVar = effect->GetVariableByName(variableName.c_str())->AsShaderResource();

	if (pTextureVar == nullptr)
		return E_FAIL;

	if (pass == nullptr)
	{
		pass = effect->GetTechniqueByIndex(0)->GetPassByIndex(passIndex);
		if (pass == nullptr)
			return E_FAIL;
	}

	if (FAILED(pTextureVar->SetResource(texture)))
		return E_FAIL;
	return pass->Apply(0, context);
}

HRESULT Shader::SetConstantBuffer(const std::string& variableName, ID3D11Buffer* buffer)
{
	if (buffer == nullptr)
		return E_FAIL;

	if (pass == nullptr)
	{
		pass = effect->GetTechniqueByIndex(0)->GetPassByIndex(passIndex);
		if (pass == nullptr)
			return E_FAIL;
	}

	ID3DX11EffectConstantBuffer* constantBuffer = effect->GetConstantBufferByName(variableName.c_str());
	if (constantBuffer == nullptr)
		return E_FAIL;

	constantBuffer->SetConstantBuffer(buffer);

	pass->Apply(0, context);
	return S_OK;
}

HRESULT Shader::LoadShader(ID3D11Device*& device, const std::wstring& fileName, D3D11_INPUT_ELEMENT_DESC** elements, UINT elementsSize, ID3DX11Effect*& effect, UINT& passNumber, std::vector<ID3D11InputLayout*>& inputLayouts)
{
#ifdef _DEBUG
	UINT flag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	UINT flag = D3DCOMPILE_OPTIMIZATION_LEVEL1;
#endif
	ID3DBlob* ErrBlob = nullptr;

	if (FAILED(D3DX11CompileEffectFromFile(fileName.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, flag, 0, device, &effect, &ErrBlob)))
	{
		if (ErrBlob != nullptr)
		{
			MessageBoxA(nullptr, (char*)ErrBlob->GetBufferPointer(), "ERROR", MB_OK);
			ErrBlob->Release();
		}

		return E_FAIL;
	}

	technique = effect->GetTechniqueByIndex(0);
	if (nullptr == technique)
		return E_FAIL;

	D3DX11_TECHNIQUE_DESC TechniqueDesc;
	technique->GetDesc(&TechniqueDesc);

	passNumber = TechniqueDesc.Passes;

	for (UINT i = 0; i < passNumber; i++)
	{
		pass = technique->GetPassByIndex(i);
		if (pass == nullptr)
			return E_FAIL;

		D3DX11_PASS_DESC PassDesc;
		pass->GetDesc(&PassDesc);

		ID3D11InputLayout* newLayout = nullptr;

		if (FAILED(device->CreateInputLayout(*elements, elementsSize, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &newLayout)))
			return E_FAIL;

		inputLayouts.push_back(newLayout);
	}
	return S_OK;
}

ID3D11Buffer* Shader::ConstantBuffer(std::wstring bufferName)
{
	auto bufferIterator = constantBuffers.find(bufferName);
	if (bufferIterator == constantBuffers.end())
		return nullptr;
	return bufferIterator->second;
}

HRESULT Shader::BindConstantBuffer(ID3D11Buffer* buffer, char* bufferName)
{
	ID3DX11EffectConstantBuffer* constantBuffer = effect->GetConstantBufferByName(bufferName);
	if (constantBuffer == nullptr)
		return E_FAIL;

	return constantBuffer->AsConstantBuffer()->SetConstantBuffer(buffer);
}

HRESULT Shader::BindConstantBuffer(ID3D11Buffer* buffer, const std::string& bufferName)
{
	ID3DX11EffectConstantBuffer* constantBuffer = effect->GetConstantBufferByName(bufferName.c_str());
	if (constantBuffer == nullptr)
		return E_FAIL;

	return constantBuffer->AsConstantBuffer()->SetConstantBuffer(buffer);
}

HRESULT Shader::ConstantBuffer(std::wstring bufferName, ID3D11Buffer* buffer)
{
	if (buffer == nullptr)
		return E_FAIL;

	auto bufferIterator = constantBuffers.find(bufferName);
	if (bufferIterator != constantBuffers.end())
		return E_FAIL;

	constantBuffers[bufferName] = buffer;

	return S_OK;
}

void Shader::SetNamePreset(void)
{
	shaderFile = L"ModelVSShader.hlsl";
	shaderFileA = "ModelVSShader.hlsl";
	boneMatrixBuffer = L"BoneBuffer";
	boneMatrixBufferA = "BoneBuffer";
	MaterialConstantBuffer = L"MaterialBufferInput";
	MaterialConstantBufferA = "MaterialBufferInput";
	viewProjectionMatrix = L"viewProjectionMatrix";
	viewProjectionMatrixA = "viewProjectionMatrix";
	worldMatrix = L"worldMatrix";
	worldMatrixA = "worldMatrix";
}

void Shader::PassNumber(UINT value)
{
	//if (passNumber >= value)
	//	passIndex = passNumber - 1;
	//else
		passIndex = value;

	if (technique == nullptr)
	{
		technique = effect->GetTechniqueByIndex(0);
	}

	pass = technique->GetPassByIndex(passIndex);
	if(pass != nullptr)
		pass->Apply(0, context);
}
