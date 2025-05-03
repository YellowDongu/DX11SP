#include "Foundation.h"
#include "RectanglePolygon.h"
#include "GameInstance.h"

using namespace Engine;

RectanglePolygon::RectanglePolygon(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Polygon(dxDevice, dxDeviceContext), horizontal(false)
{
}

RectanglePolygon::RectanglePolygon(const RectanglePolygon& other) : Polygon(other), horizontal(other.horizontal)
{
}

void RectanglePolygon::Free(void)
{
	Polygon::Free();
}

RectanglePolygon* RectanglePolygon::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, bool horizontal, bool saveVertex)
{
	RectanglePolygon* newInstance = new RectanglePolygon(dxDevice, dxDeviceContext);
	newInstance->horizontal = horizontal;
	newInstance->saveVertex = saveVertex;
	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
	return newInstance;
}

Component* RectanglePolygon::Clone(void)
{
	return new RectanglePolygon(*this);
}

HRESULT RectanglePolygon::Start(void)
{
	std::vector<SimpleVertex> vertices;
	std::vector<UINT> indices;
	if(FAILED(CreateVertex(vertices, indices, horizontal)))
		return E_FAIL;

	if (FAILED(CreateIndexBuffer(dxDevice, indexBuffer, indexCount, indices)))
		return E_FAIL;

	if (FAILED(CreateVertexBuffer(dxDevice, vertexBuffer, stride, vertices)))
		return E_FAIL;

	if (saveVertex)
	{
		savedIndices.resize(indices.size());
		savedVertices.resize(vertices.size());
		memcpy(savedIndices.data(), indices.data(), sizeof(UINT) * indices.size());
		memcpy(savedVertices.data(), vertices.data(), sizeof(SimpleVertex) * vertices.size());
	}

	return S_OK;
}

void RectanglePolygon::Render(void)
{
	::Render(indexBuffer, vertexBuffer, stride, offset, DXGI_FORMAT_R32_UINT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	dxDeviceContext->DrawIndexed(indexCount, 0, 0);
}

void RectanglePolygon::Render(Shader* otherShader)
{
	dxDeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	dxDeviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	dxDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	otherShader->ApplyShader();

	dxDeviceContext->DrawIndexed(indexCount, 0, 0);
}

HRESULT RectanglePolygon::CreateVertex(std::vector<SimpleVertex>& vertices, std::vector<UINT>& indices, bool horizontal)
{
	vertices.clear();
	indices.clear();
	if (!horizontal)
	{
		vertices.push_back({ {-0.5f, 0.5f, 0.f }, { 0.f, 0.f } });
		vertices.push_back({ { 0.5f, 0.5f, 0.f }, { 1.f, 0.f } });
		vertices.push_back({ { 0.5f,-0.5f, 0.f }, { 1.f, 1.f } });
		vertices.push_back({ {-0.5f,-0.5f, 0.f }, { 0.f, 1.f } });
	}
	else
	{
		vertices.push_back({ {-0.5f, 0.f, 0.5f }, { 0.f, 0.f } });
		vertices.push_back({ { 0.5f, 0.f, 0.5f }, { 1.f, 0.f } });
		vertices.push_back({ { 0.5f, 0.f,-0.5f }, { 1.f, 1.f } });
		vertices.push_back({ {-0.5f, 0.f,-0.5f }, { 0.f, 1.f } });
	}

	indices = {
		 0 ,1 ,2
		,0 ,2 ,3
	};

	return S_OK;
}

HRESULT RectanglePolygon::CreateVertexBuffer(ID3D11Device* dxDevice, ID3D11Buffer*& buffer, UINT& vertexStride, std::vector<SimpleVertex>& vertices)
{
	vertexStride = static_cast<UINT>(sizeof(SimpleVertex));
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
