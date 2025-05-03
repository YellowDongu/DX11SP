#pragma once

#include "Component.h"

namespace Engine
{
	class ENGINEDLL Polygon : public Component
	{
	protected:
		Polygon(void) = delete;
		Polygon(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
		Polygon(const Polygon& other);
		virtual ~Polygon(void) = default;
		virtual void Free(void) override;
	public:
		virtual Component* Clone(void) override;

		virtual void Render(void);

		void BorrowBuffer(ID3D11Buffer** index, ID3D11Buffer** vertex) { index = &indexBuffer; vertex = &vertexBuffer; }

		HRESULT CreateIndexBuffer(ID3D11Device* dxDevice, ID3D11Buffer*& buffer, UINT& indexCount, std::vector<UINT>& indices);
		HRESULT CreateDefaultVertexBuffer(ID3D11Device* dxDevice, ID3D11Buffer*& buffer, UINT& vertexStride, std::vector<StaticModelVertex>& vertex);

	protected:
		bool saveVertex{ false };
		bool original{ false };
		ID3D11Buffer* indexBuffer{ nullptr };
		ID3D11Buffer* vertexBuffer{ nullptr };

		UINT stride{ 0 };
		UINT offset{ 0 };
		UINT indexCount{ 0 };

		std::vector<UINT> savedIndices;
	};
}