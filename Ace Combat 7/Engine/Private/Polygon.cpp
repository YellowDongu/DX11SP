#include "Foundation.h"
#include "Polygon.h"
#include "GameInstance.h"

using namespace Engine;

Polygon::Polygon(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Component(dxDevice, dxDeviceContext)
{
}

Polygon::Polygon(const Polygon& other) : Component(other), indexBuffer(other.indexBuffer), vertexBuffer(other.vertexBuffer), stride(other.stride), offset(other.offset), indexCount(other.indexCount)
{
	if (indexBuffer != nullptr)
		indexBuffer->AddRef();
	if (vertexBuffer != nullptr)
		vertexBuffer->AddRef();
}

void Polygon::Free(void)
{
	//if (!original)
	//	return;
	
	if (indexBuffer != nullptr)
		indexBuffer->Release();
	if (vertexBuffer != nullptr)
		vertexBuffer->Release();
}

Component* Polygon::Clone(void)
{
	return new Polygon(*this);
}

void Polygon::Render(void)
{
	::Render(indexBuffer, vertexBuffer, stride, offset, DXGI_FORMAT_R32_UINT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	dxDeviceContext->DrawIndexed(indexCount, 0, 0);
}
