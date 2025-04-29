#include "Foundation.h"
#include "Component.h"


using namespace Engine;


Component::Component(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : BaseObject(dxDevice, dxDeviceContext), gameObject(nullptr)
{
}

Component::Component(const Component& other) : BaseObject(other), gameObject(nullptr)
{

}