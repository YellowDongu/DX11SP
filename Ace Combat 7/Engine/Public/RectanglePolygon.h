#pragma once
#include "Polygon.h"


namespace Engine
{
	class ENGINEDLL RectanglePolygon : public Polygon
	{
	private:
		RectanglePolygon(void) = delete;
		RectanglePolygon(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
		RectanglePolygon(const RectanglePolygon& other);
		virtual ~RectanglePolygon(void) = default;
		virtual void Free(void) override;
	public:
		static RectanglePolygon* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, bool horizontal = false, bool saveVertex = false);
		virtual Engine::Component* Clone(void) override;

		HRESULT Start(void);
		virtual void Render(void) override;
		void Render(class Shader* otherShader);

		HRESULT CreateVertex(std::vector<SimpleVertex>& vertex, std::vector<UINT>& index, bool horizontal = false);
		HRESULT CreateVertexBuffer(ID3D11Device* dxDevice, ID3D11Buffer*& buffer, UINT& vertexStride, std::vector<SimpleVertex>& vertex);

	private:
		bool horizontal;
		std::vector<SimpleVertex> savedVertices;

	};
}