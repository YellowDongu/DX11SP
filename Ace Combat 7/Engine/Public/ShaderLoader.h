#pragma once

#include "Base.h"
#include "Shader.h"

namespace Engine
{
	class ShaderLoader final : public Base
	{
    private:
		ShaderLoader(void) = delete;
		ShaderLoader(ID3D11Device*& device, ID3D11DeviceContext*& context);
		virtual ~ShaderLoader(void) = default;

		virtual void Free(void) override;
    public:
		static ShaderLoader* Create(ID3D11Device*& device, ID3D11DeviceContext*& context);


        Shader* LoadShader(const std::wstring& filePath, D3D11_INPUT_ELEMENT_DESC** elements, UINT elementsSize);
        Shader* LoadShader(const std::wstring& filePath, std::vector<D3D11_INPUT_ELEMENT_DESC>& elements);
        Shader* LoadShader(ID3D11Device*& device, ID3D11DeviceContext*& context, const std::wstring& filePath, D3D11_INPUT_ELEMENT_DESC** elements, UINT elementsSize);
        Shader* LoadShader(ID3D11Device*& device, ID3D11DeviceContext*& context, const std::wstring& filePath, std::vector<D3D11_INPUT_ELEMENT_DESC>& elements);

        Shader* GetShader(const std::wstring& key);
		Shader* CurrentShader(void) { return currentShader; }
		const std::wstring& CurrentShaderName(void) const { return currentShaderName; }
		HRESULT SetShader(const std::wstring& key);
		HRESULT SetShader(Shader* shader);
		HRESULT AddShader(const std::wstring& key, Shader* newShader);
		void SetViewProjectionMatrix(const Matrix& matrix);

	private:
        ID3D11Device* device;
        ID3D11DeviceContext* context;
		Shader* currentShader;
		std::wstring currentShaderName;
		std::map<std::wstring, Shader*> shaders;
	};

}