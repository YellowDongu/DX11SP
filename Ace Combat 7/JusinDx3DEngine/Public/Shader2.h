#pragma once
#include "Base.h"


namespace Engine
{
	class ENGINEDLL Shader final : public Base
	{
	private:
		Shader(void) = default;
		Shader(ID3D11Device*& device, ID3D11DeviceContext*& context);
		~Shader(void) = default;
		virtual void Free(void) override;
	public:
		static Shader* Create(ID3D11Device*& device, ID3D11DeviceContext*& context, const std::wstring& fileName, D3D11_INPUT_ELEMENT_DESC** elements, UINT elementsSize);



		HRESULT LoadFXshader(const std::wstring& fileName, const D3D11_INPUT_ELEMENT_DESC*& elements, UINT elementsSize);
		HRESULT Render(void);

		HRESULT BindMatrix(const std::string& variableName, const Matrix& matrix);
		HRESULT LoadShader(ID3D11Device*& device, const std::wstring& fileName, const D3D11_INPUT_ELEMENT_DESC*& elements, UINT elementsSize, ID3DX11Effect*& effect, UINT& passNumber, std::vector<ID3D11InputLayout*>& inputLayouts);

	private:
		ID3D11Device* device;
		ID3D11DeviceContext* context;
		ID3DX11Effect* effect;
		UINT passNumber, layoutIndex, passIndex;
		UINT techniqueNumber = 1, techniqueIndex = 0;
		std::vector<ID3D11InputLayout*> inputLayouts;

		ID3DX11Effect* g_pEffect = nullptr;
		ID3DX11EffectTechnique* technique = nullptr;
		ID3DX11EffectPass* pass = nullptr;
	};

}