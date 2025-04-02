#include "Foundation.h"
#include "Shader.h"

using namespace Engine;


extern "C"


Shader::Shader(ID3D11Device*& _device, ID3D11DeviceContext*& _context) : device(_device), context(_context)
{
}

void Shader::Free(void)
{
}

Shader* Shader::Create(ID3D11Device*& device, ID3D11DeviceContext*& context, const std::wstring& fileName, D3D11_INPUT_ELEMENT_DESC** elements, UINT elementsSize)
{
	Shader* newInstance = new Shader(device, context);

	if (FAILED(newInstance->LoadFXshader(fileName, *elements, elementsSize)))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}

	return newInstance;
}

HRESULT Shader::LoadFXshader(const std::wstring& fileName, const D3D11_INPUT_ELEMENT_DESC*& elements, UINT elementsSize)
{
	return LoadShader(device, fileName, elements, elementsSize, effect, passNumber, inputLayouts);
}

HRESULT Shader::Render(void)
{
	if (layoutIndex >= inputLayouts.size())
		return E_FAIL;

	context->IASetInputLayout(inputLayouts[layoutIndex]);

	ID3DX11EffectPass* currentPass = effect->GetTechniqueByIndex(0)->GetPassByIndex(passIndex);
	if (currentPass == nullptr)
		return E_FAIL;

	return currentPass->Apply(0, context);
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

HRESULT Shader::LoadShader(ID3D11Device*& device, const std::wstring& fileName, const D3D11_INPUT_ELEMENT_DESC*& elements, UINT elementsSize, ID3DX11Effect*& effect, UINT& passNumber, std::vector<ID3D11InputLayout*>& inputLayouts)
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

	ID3DX11EffectTechnique* technique = effect->GetTechniqueByIndex(0);
	if (nullptr == technique)
		return E_FAIL;

	D3DX11_TECHNIQUE_DESC TechniqueDesc;
	technique->GetDesc(&TechniqueDesc);

	passNumber = TechniqueDesc.Passes;

	for (size_t i = 0; i < passNumber; i++)
	{
		ID3DX11EffectPass* effectPass = technique->GetPassByIndex(i);
		if (effectPass == nullptr)
			return E_FAIL;

		D3DX11_PASS_DESC PassDesc;
		effectPass->GetDesc(&PassDesc);

		ID3D11InputLayout* newLayout = nullptr;

		if (FAILED(device->CreateInputLayout(elements, elementsSize, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &newLayout)))
			return E_FAIL;

		inputLayouts.push_back(newLayout);
	}
	return S_OK;
}