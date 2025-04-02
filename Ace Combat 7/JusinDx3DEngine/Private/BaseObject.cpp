#include "Foundation.h"
#include "BaseObject.h"

using namespace Engine;


BaseObject::BaseObject(ID3D11Device* _dxDevice, ID3D11DeviceContext* _dxDeviceContext) : dxDevice(_dxDevice), dxDeviceContext(_dxDeviceContext)
{
	dxDevice->AddRef();
	dxDeviceContext->AddRef();
}

BaseObject::BaseObject(const BaseObject& other) : dxDevice(other.dxDevice), dxDeviceContext(other.dxDeviceContext)
{
	dxDevice->AddRef();
	dxDeviceContext->AddRef();
}

BaseObject::~BaseObject(void)
{
	if (dxDevice)
		dxDevice->Release();
	if (dxDeviceContext)
		dxDeviceContext->Release();
}

