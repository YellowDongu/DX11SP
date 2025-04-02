#pragma once
#include "Base.h"

namespace Engine
{
	class ENGINEDLL BaseObject abstract : public Base
	{
	public:
		BaseObject(void) = delete;
		BaseObject(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
		BaseObject(const BaseObject& other);
		virtual ~BaseObject(void);


		void SetNonDestroyObject(void) { nonDestroy = true; }
		void NonDestroyObject(void) { nonDestroy = true; }
	protected:
		bool nonDestroy = false;

		ID3D11Device* dxDevice = nullptr;
		ID3D11DeviceContext* dxDeviceContext = nullptr;
	};

}
