#pragma once
#include "Base.h"


namespace Engine
{
	class ENGINEDLL Shader : public Base
	{
	protected:
		Shader(void) = default;
		Shader(ID3D11Device*& device, ID3D11DeviceContext*& context);
		Shader(const Shader& other) = delete;
		virtual ~Shader(void) = default;
		virtual void Free(void) override;
	public:
		static Shader* Create(ID3D11Device*& device, ID3D11DeviceContext*& context, const std::wstring& fileName, D3D11_INPUT_ELEMENT_DESC** elements, UINT elementsSize);
		static Shader* Create(ID3D11Device*& device, ID3D11DeviceContext*& context, const std::wstring& fileName, std::vector<D3D11_INPUT_ELEMENT_DESC> elements);

		virtual HRESULT CreateBuffers(ID3D11Device* device, std::map<std::wstring, ID3D11Buffer*>& constantBuffers) { return S_OK; }
		HRESULT LoadFXshader(const std::wstring& fileName, D3D11_INPUT_ELEMENT_DESC** elements, UINT elementsSize);
		HRESULT LoadFXshader(const std::wstring& fileName, std::vector< D3D11_INPUT_ELEMENT_DESC> elements);
		HRESULT Render(ID3D11Buffer* indexBuffer, ID3D11Buffer* vertexBuffer, UINT stride, UINT offset = 0, DXGI_FORMAT indexFormat = DXGI_FORMAT_R32_UINT, D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		HRESULT Render(ID3D11Buffer* indexBuffer, ID3D11Buffer** vertexBuffer, UINT* stride, UINT vertexBufferCount, UINT* offset, DXGI_FORMAT indexFormat = DXGI_FORMAT_R32_UINT, D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		HRESULT Render(ID3D11Buffer* indexBuffer, std::vector<ID3D11Buffer*>& vertexBuffer, std::vector<UINT>& stride, UINT vertexBufferCount, std::vector<UINT>& offset, DXGI_FORMAT indexFormat = DXGI_FORMAT_R32_UINT, D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		HRESULT SetShader(void);
		HRESULT ApplyShader(void);

		HRESULT BindMatrix(const std::string& variableName, const Matrix& matrix);
		HRESULT BindVariable(const std::string& variableName, const void* pointer, UINT size);
		HRESULT BindConstantBuffer(const std::string& variableName, ID3D11Buffer* buffer);
		HRESULT BindTexture(const std::string& variableName, ID3D11ShaderResourceView* texture);
		HRESULT SetConstantBuffer(const std::string& variableName, ID3D11Buffer* buffer);
		HRESULT LoadShader(ID3D11Device*& device, const std::wstring& fileName, D3D11_INPUT_ELEMENT_DESC** elements, UINT elementsSize, ID3DX11Effect*& effect, UINT& passNumber, std::vector<ID3D11InputLayout*>& inputLayouts);
		ID3D11Buffer* ConstantBuffer(std::wstring bufferName);
		HRESULT ConstantBuffer(std::wstring bufferName, ID3D11Buffer* buffer);
		virtual void SetNamePreset(void);

		UINT PassNumber(void) { return passNumber; }
		void PassNumber(UINT value);

		//HRESULT TechniqueNumber(UINT value);
		UINT TechniqueNumber(void) { return techniqueIndex; }


	protected:
		ID3D11Device* device = nullptr;
		ID3D11DeviceContext* context = nullptr;
		ID3DX11Effect* effect = nullptr;
		UINT passNumber = 1, layoutIndex = 0, passIndex = 0;
		UINT techniqueNumber = 1, techniqueIndex = 0;
		std::vector<ID3D11InputLayout*> inputLayouts = {};
		std::map<std::wstring, ID3D11Buffer*> constantBuffers = {};

		ID3DX11EffectTechnique* technique = nullptr;
		ID3DX11EffectPass* pass = nullptr;



	public: // keys
		std::wstring shaderFile = L"ModelVSShader.hlsl";
		std::string  shaderFileA = "ModelVSShader.hlsl";
		std::wstring boneMatrixBuffer = L"BoneBuffer";
		std::string  boneMatrixBufferA = "BoneBuffer";
		std::wstring MaterialConstantBuffer = L"MaterialBufferInput";
		std::string  MaterialConstantBufferA = "MaterialBufferInput";
		std::wstring viewProjectionMatrix = L"viewProjectionMatrix";
		std::string  viewProjectionMatrixA = "viewProjectionMatrix";
		std::wstring worldMatrix = L"worldMatrix";
		std::string  worldMatrixA = "worldMatrix";
		std::wstring diffuseTexture = L"diffuseTexture";
		std::string  diffuseTextureA = "diffuseTexture";
		std::wstring normalTexture = L"normalTexture";
		std::string  normalTextureA = "normalTexture";
		std::wstring specularTexture = L"specularTexture";
		std::string  specularTextureA = "specularTexture";
		//matrix viewProjectionMatrix;
	};

}