#pragma once
#include "Base.h"


enum MatrixType
{
	DXWorld,
	DXProjection,
	End
};

namespace Engine
{
	class BufferSender final : public Base
	{
	private:
		BufferSender(void) = delete;
		BufferSender(const BufferSender& other) = delete;
		virtual ~BufferSender(void) = default;
		BufferSender(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
		HRESULT Initialize(void);
		virtual void Free(void) override;
	public:
		static BufferSender* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);

		void RenderPreset(void);

        HRESULT CompileShader(const std::wstring& filePath, const std::string& entryMethod, const std::string& shaderModel, ID3DBlob*& ShaderBlob, ID3DBlob*& errorBlob);
        HRESULT LoadVertexShader(ID3D11Device*& device, ID3DBlob*& vertexShaderBlob, ID3D11VertexShader*& vertexShader);
        HRESULT InputLayoutSetting(ID3D11Device*& device, ID3DBlob*& vertexShaderBlob, ID3D11InputLayout*& inputLayout);
        HRESULT LoadPixelShader(ID3D11Device*& device, ID3DBlob*& pixelShaderBlob, ID3D11PixelShader*& pixelShader);
        HRESULT CreateSampler(ID3D11Device*& pDevice, ID3D11SamplerState*& g_pSamplerState);
        HRESULT LoadVertexShader(ID3D11Device*& device, const std::wstring& filePath, ID3D11VertexShader*& vertexShader, ID3D11InputLayout*& inputLayout);
        HRESULT LoadPixelShader(ID3D11Device*& device, const std::wstring& filePath, ID3D11PixelShader*& pixelShader, ID3D11SamplerState*& samplerState);
        HRESULT CreateRasterizerState(ID3D11Device*& device, ID3D11RasterizerState*& rasterizerState);
		HRESULT CreateMatrixBuffer(ID3D11Device*& device, ID3D11Buffer*& matrixBuffer);
		HRESULT CreateMaterialBuffer(ID3D11Device*& device, ID3D11Buffer*& materialBuffer);

		HRESULT SetTransform(MatrixType type, const DirectX::XMMATRIX& matrix);
		HRESULT SetMaterial(MaterialBufferType bufferType);
		HRESULT SetTexture(ID3D11ShaderResourceView* diffuseTexture, ID3D11ShaderResourceView* specularTexture);
	private:

		ID3D11Device* device = nullptr;
		ID3D11DeviceContext* context = nullptr;

		ID3D11SamplerState* samplerState = nullptr;
		ID3D11VertexShader* vertexShader = nullptr;
		ID3D11PixelShader* pixelShader = nullptr;
		ID3D11InputLayout* inputLayout = nullptr; // vertexInputLayout

		ID3D11RasterizerState* g_pRasterizerState = nullptr;
		ID3D11Buffer* materialBuffer = nullptr;

		std::vector<ID3D11Buffer*> matrixBuffer;
	};

}

