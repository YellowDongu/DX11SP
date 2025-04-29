#pragma once

#include "Polygon.h"

namespace Engine
{
	class ENGINEDLL CubePolygon : public Polygon
	{
	private:
		CubePolygon(void) = delete;
		CubePolygon(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
		CubePolygon(const CubePolygon& other);
		virtual ~CubePolygon(void) = default;
		virtual void Free(void);
	public:
		static CubePolygon* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, bool saveVertex = false);
		static CubePolygon* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, std::vector<CubeVertex>& vertices, std::vector<UINT>& indices, bool saveVertex = false);
		virtual Component* Clone(void);

		void CreateDefaultPolygon(std::vector<CubeVertex>& vertices, std::vector<UINT>& indices);
		HRESULT CreateBuffer(std::vector<CubeVertex>& vertices, std::vector<UINT>& indices);

	private:
		std::vector<CubeVertex> vertices;
		std::vector<UINT> indices;

	};

}