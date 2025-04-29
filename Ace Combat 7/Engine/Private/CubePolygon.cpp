#include "Foundation.h"
#include "CubePolygon.h"

using namespace Engine;

CubePolygon::CubePolygon(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Polygon(dxDevice, dxDeviceContext)
{
}

CubePolygon::CubePolygon(const CubePolygon& other) : Polygon(other)
{
}

void CubePolygon::Free(void)
{
	Polygon::Free();
}

CubePolygon* CubePolygon::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, bool saveVertex)
{
	CubePolygon* newInstance = new CubePolygon(dxDevice, dxDeviceContext);
	newInstance->saveVertex = saveVertex;
	std::vector<CubeVertex> vertices; std::vector<UINT> indices;
	newInstance->CreateDefaultPolygon(vertices, indices);
	if (FAILED(newInstance->CreateBuffer(vertices, indices)))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
	if (saveVertex)
	{
		newInstance->vertices.resize(vertices.size());
		newInstance->indices.resize(indices.size());

		memcpy(newInstance->vertices.data(), vertices.data(), sizeof(CubeVertex) * vertices.size());
		memcpy(newInstance->indices.data(), indices.data(), sizeof(UINT) * indices.size());
	}
	
	return newInstance;
}

CubePolygon* CubePolygon::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, std::vector<CubeVertex>& vertices, std::vector<UINT>& indices, bool saveVertex)
{
	CubePolygon* newInstance = new CubePolygon(dxDevice, dxDeviceContext);
	newInstance->saveVertex = saveVertex;

	if (FAILED(newInstance->CreateBuffer(vertices, indices)))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
	if (saveVertex)
	{
		newInstance->vertices.resize(vertices.size());
		newInstance->indices.resize(indices.size());

		memcpy(newInstance->vertices.data(), vertices.data(), sizeof(CubeVertex) * vertices.size());
		memcpy(newInstance->indices.data(), indices.data(), sizeof(UINT) * indices.size());
	}

	return newInstance;
}

Component* CubePolygon::Clone(void)
{
	return new CubePolygon(*this);
}

void CubePolygon::CreateDefaultPolygon(std::vector<CubeVertex>& vertices, std::vector<UINT>& indices)
{
	vertices.clear();
	vertices.resize(8);


	vertices[0].position = { -0.5f, 0.5f,-0.5f };
	vertices[1].position = { 0.5f, 0.5f,-0.5f };
	vertices[2].position = { 0.5f,-0.5f,-0.5f };
	vertices[3].position = { -0.5f,-0.5f,-0.5f };
	vertices[4].position = { -0.5f, 0.5f, 0.5f };
	vertices[5].position = { 0.5f, 0.5f, 0.5f };
	vertices[6].position = { 0.5f,-0.5f, 0.5f };
	vertices[7].position = { -0.5f,-0.5f, 0.5f };

	vertices[0].texturePosition = vertices[0].position;
	vertices[1].texturePosition = vertices[1].position;
	vertices[2].texturePosition = vertices[2].position;
	vertices[3].texturePosition = vertices[3].position;
	vertices[4].texturePosition = vertices[4].position;
	vertices[5].texturePosition = vertices[5].position;
	vertices[6].texturePosition = vertices[6].position;
	vertices[7].texturePosition = vertices[7].position;


	indices.clear();
	indices.resize(36);
	/* +X */
	indices[0] = 1; indices[1] = 5; indices[2] = 6;
	indices[3] = 1; indices[4] = 6; indices[5] = 2;

	/* -X */
	indices[6] = 4; indices[7] = 0; indices[8] = 3;
	indices[9] = 4; indices[10] = 3; indices[11] = 7;

	/* +Y */
	indices[12] = 4; indices[13] = 5; indices[14] = 1;
	indices[15] = 4; indices[16] = 1; indices[17] = 0;

	/* -Y */
	indices[18] = 3; indices[19] = 2; indices[20] = 6;
	indices[21] = 3; indices[22] = 6; indices[23] = 7;

	/* +Z */
	indices[24] = 5; indices[25] = 4; indices[26] = 7;
	indices[27] = 5; indices[28] = 7; indices[29] = 6;

	/* -Z */
	indices[30] = 0; indices[31] = 1; indices[32] = 2;
	indices[33] = 0; indices[34] = 2; indices[35] = 3;
}

HRESULT CubePolygon::CreateBuffer(std::vector<CubeVertex>& vertices, std::vector<UINT>& indices)
{
	// 버텍스 버퍼 생성
	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = (UINT)(sizeof(CubeVertex) * vertices.size());
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertexData = {};
	ZeroMemory(&vertexData, sizeof(vertexData));
	vertexData.pSysMem = vertices.data();

	if (FAILED(dxDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer)))
		return E_FAIL;

	// 인덱스 버퍼 생성
	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = (UINT)(sizeof(UINT) * indices.size());
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexData = {};
	ZeroMemory(&indexData, sizeof(indexData));
	indexData.pSysMem = indices.data();

	if (FAILED(dxDevice->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer)))
		return E_FAIL;

	stride = sizeof(CubeVertex);
	offset = 0;
	indexCount = indices.size();
	return S_OK;
}
