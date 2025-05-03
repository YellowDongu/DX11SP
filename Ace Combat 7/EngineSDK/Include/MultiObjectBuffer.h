#pragma once

#include "Component.h"
namespace Engine
{
	struct VertexMatrix
	{
		float4 right, up, look, position;
	};
	struct AdditionalVertexInfomation
	{
		FLOAT infoFirst;
		float2 infoSecond;
	};

	class ENGINEDLL MultiObjectBuffer abstract : public Component
	{
	protected:
		MultiObjectBuffer(void) = delete;
		MultiObjectBuffer(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
		MultiObjectBuffer(const MultiObjectBuffer& other);
		virtual ~MultiObjectBuffer(void) = default;
		virtual void Free(void) override;
	public:

		HRESULT BindWorldBuffer(const std::vector<Matrix>& matrix);
		HRESULT BindWorldBuffer(const std::vector<Engine::VertexMatrix>& matrixData);

		virtual void BindAssembler(void);
		virtual void Render(void);

		HRESULT CreatePointIndexBuffer(ID3D11Device* dxDevice, ID3D11Buffer*& indexBuffer, UINT instanceNumber, UINT& indexCount, UINT& wholeIndexCount, std::vector<UINT>& indices);
		HRESULT CreateIndexBuffer(ID3D11Device* dxDevice, ID3D11Buffer*& indexBuffer, UINT instanceNumber, UINT& indexCount, UINT& wholeIndexCount, std::vector<UINT>& indices);
		HRESULT CreateVertexBuffer(ID3D11Device* dxDevice, ID3D11Buffer*& indexBuffer, UINT& vertexStride, std::vector<StaticModelVertex>& vertex);
		HRESULT CreateAdditionalVertexInfomationBuffer(ID3D11Device* dxDevice, ID3D11Buffer*& indexBuffer, UINT& vertexStride, UINT instanceNumber);
		HRESULT CreateWorldBuffer(ID3D11Device* dxDevice, ID3D11Buffer*& worldBuffer, UINT instanceNumber, UINT& worldStride);

	protected:
		bool clone;
		UINT instanceNumber{0};
		ID3D11Buffer* vertexBuffer{nullptr};
		ID3D11Buffer* indexBuffer{nullptr};
		ID3D11Buffer* worldBuffer{nullptr};
		ID3D11Buffer* additionalBuffer{nullptr};
		std::vector<Matrix> matrixData;
		UINT vertexStride{0};
		UINT worldStride{0};
		UINT additionalInfoStride{0};
		UINT indexCount{0};
		UINT wholeIndexCount{0};
	};
}