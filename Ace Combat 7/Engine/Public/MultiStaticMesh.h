#pragma once
#include "MultiObjectBuffer.h"


namespace Engine
{

	class MultiStaticMesh : public MultiObjectBuffer
	{
	private:
		MultiStaticMesh(void) = delete;
		MultiStaticMesh(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
		MultiStaticMesh(const MultiStaticMesh& other);
		virtual ~MultiStaticMesh(void) = default;
		virtual void Free(void) override;
	public:
		static MultiStaticMesh* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, std::wstring modelFilePath);
		Component* Clone(void);



	private:

	};
}