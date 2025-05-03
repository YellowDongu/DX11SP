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

HRESULT Polygon::CreateIndexBuffer(ID3D11Device* dxDevice, ID3D11Buffer*& buffer, UINT& indexCount, std::vector<UINT>& indices)
{
	UINT indexStride = sizeof(UINT);
	indexCount = static_cast<UINT>(indices.size());

	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = (UINT)(indexStride * indices.size());
	indexBufferDesc.StructureByteStride = indexStride;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA indexData = {};
	ZeroMemory(&indexData, sizeof(indexData));
	indexData.pSysMem = indices.data();

	if (FAILED(dxDevice->CreateBuffer(&indexBufferDesc, &indexData, &buffer)))
		return E_FAIL;

	return S_OK;
}

HRESULT Polygon::CreateDefaultVertexBuffer(ID3D11Device* dxDevice, ID3D11Buffer*& buffer, UINT& vertexStride, std::vector<StaticModelVertex>& vertices)
{
	vertexStride = static_cast<UINT>(sizeof(StaticModelVertex));
	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = vertexStride * static_cast<UINT>(vertices.size());
	vertexBufferDesc.StructureByteStride = vertexStride;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexData = {};
	ZeroMemory(&vertexData, sizeof(vertexData));
	vertexData.pSysMem = vertices.data();

	if (FAILED(dxDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &buffer)))
		return E_FAIL;
	return S_OK;
}
