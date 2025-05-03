#include "Foundation.h"
#include "MultiObjectBuffer.h"
#include "GameInstance.h"
using namespace Engine;

MultiObjectBuffer::MultiObjectBuffer(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Component(dxDevice, dxDeviceContext), clone(false)
{
}

MultiObjectBuffer::MultiObjectBuffer(const MultiObjectBuffer& other) : Component(other), clone(true)
{
}

void MultiObjectBuffer::Free(void)
{
	if (clone)
		return;
	if (vertexBuffer != nullptr)
	{
		vertexBuffer->Release();
		vertexBuffer = nullptr;
	}
	if (indexBuffer != nullptr)
	{
		indexBuffer->Release();
		indexBuffer = nullptr;
	}
	if (worldBuffer != nullptr)
	{
		worldBuffer->Release();
		worldBuffer = nullptr;
	}
	if (additionalBuffer != nullptr)
	{
		additionalBuffer->Release();
		additionalBuffer = nullptr;
	}
}

HRESULT MultiObjectBuffer::BindWorldBuffer(const std::vector<Matrix>& matrix)
{
	D3D11_MAPPED_SUBRESOURCE		worldSubResource{};

	if (FAILED(dxDeviceContext->Map(worldBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &worldSubResource)))
		return E_FAIL;

	Engine::VertexMatrix* matrixBuffer = static_cast<Engine::VertexMatrix*>(worldSubResource.pData);
	//std::vector<Engine::VertexMatrix> matrixData(matrix.size());
	//for (size_t i = 0; i < matrix.size(); i++)
	//{
	//	memcpy(&matrixData[i].right, &matrix[i].m[0], sizeof(DirectX::XMFLOAT4));
	//	memcpy(&matrixData[i].up, &matrix[i].m[1], sizeof(DirectX::XMFLOAT4));
	//	memcpy(&matrixData[i].look, &matrix[i].m[2], sizeof(DirectX::XMFLOAT4));
	//	memcpy(&matrixData[i].position, &matrix[i].m[3], sizeof(DirectX::XMFLOAT4));
	//}
	if (matrix.size() > instanceNumber)
	{
		//memcpy(matrixBuffer, matrixData.data(), sizeof(Matrix) * instanceNumber);
		for (size_t i = 0; i < instanceNumber; i++)
		{
			memcpy(&matrixBuffer[i].right, &matrix[i].m[0], sizeof(float4));
			memcpy(&matrixBuffer[i].up, &matrix[i].m[1], sizeof(float4));
			memcpy(&matrixBuffer[i].look, &matrix[i].m[2], sizeof(float4));
			memcpy(&matrixBuffer[i].position, &matrix[i].m[3], sizeof(float4));
		}
	}
	else
	{
		//memcpy(matrixBuffer, matrixData.data(), sizeof(Matrix) * matrixData.size());
		for (size_t i = 0; i < matrix.size(); i++)
		{
			memcpy(&matrixBuffer[i].right, &matrix[i].m[0], sizeof(float4));
			memcpy(&matrixBuffer[i].up, &matrix[i].m[1], sizeof(float4));
			memcpy(&matrixBuffer[i].look, &matrix[i].m[2], sizeof(float4));
			memcpy(&matrixBuffer[i].position, &matrix[i].m[3], sizeof(float4));
		}
	}

	dxDeviceContext->Unmap(worldBuffer, 0);
	return S_OK;
}

HRESULT MultiObjectBuffer::BindWorldBuffer(const std::vector<Engine::VertexMatrix>& matrixData)
{
	D3D11_MAPPED_SUBRESOURCE		worldSubResource{};

	if (FAILED(dxDeviceContext->Map(worldBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &worldSubResource)))
		return E_FAIL;
	Engine::VertexMatrix* matrixBuffer = static_cast<Engine::VertexMatrix*>(worldSubResource.pData);

	if (matrixData.size() > instanceNumber)
	{
		memcpy(matrixBuffer, matrixData.data(), sizeof(Engine::VertexMatrix) * instanceNumber);
	}
	else
	{
		memcpy(matrixBuffer, matrixData.data(), sizeof(Engine::VertexMatrix) * matrixData.size());
	}

	dxDeviceContext->Unmap(worldBuffer, 0);

	return S_OK;
}

void MultiObjectBuffer::BindAssembler(void)
{
	/* 그리고자하는 자원들을 장치에 바인딩한다. */
	std::vector<ID3D11Buffer*> vertexBuffers = { vertexBuffer, worldBuffer, additionalBuffer };

	std::vector<UINT> vertexStrides = { vertexStride, worldStride, additionalInfoStride };
	std::vector<UINT> offsets = { 0, 0, 0 };

	GetCurrentShader()->Render(indexBuffer, vertexBuffers, vertexStrides, static_cast<UINT>(vertexBuffers.size()), offsets, DXGI_FORMAT_R32_UINT, D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
}

void Engine::MultiObjectBuffer::Render(void)
{
	BindAssembler();
	dxDeviceContext->DrawIndexedInstanced(indexCount, instanceNumber, 0, 0, 0);
}

HRESULT MultiObjectBuffer::CreatePointIndexBuffer(ID3D11Device* dxDevice, ID3D11Buffer*& indexBuffer, UINT instanceNumber, UINT& indexCount, UINT& wholeIndexCount, std::vector<UINT>& indices)
{
	UINT indexStride = sizeof(UINT);
	indexCount = static_cast<UINT>(indices.size());
	wholeIndexCount = indexCount * instanceNumber;

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

	if (FAILED(dxDevice->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer)))
		return E_FAIL;

	return S_OK;
}

HRESULT MultiObjectBuffer::CreateIndexBuffer(ID3D11Device* dxDevice, ID3D11Buffer*& buffer, UINT instanceNumber, UINT& indexCount, UINT& wholeIndexCount, std::vector<UINT>& indices)
{
	UINT indexStride = sizeof(UINT);
	std::vector<UINT> indicesForBuffer;
	indexCount = static_cast<UINT>(indices.size());
	wholeIndexCount = indexCount * instanceNumber;
	indicesForBuffer.resize(indexCount * instanceNumber);

	for (UINT i = 0; i < instanceNumber; i++)
	{
		memcpy(&indicesForBuffer[i * indexCount], indices.data(), indexStride * indexCount);
	}

    D3D11_BUFFER_DESC indexBufferDesc = {};
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = (UINT)(indexStride * indicesForBuffer.size());
	indexBufferDesc.StructureByteStride = indexStride;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA indexData = {};
    ZeroMemory(&indexData, sizeof(indexData));
    indexData.pSysMem = indicesForBuffer.data();

    if (FAILED(dxDevice->CreateBuffer(&indexBufferDesc, &indexData, &buffer)))
        return E_FAIL;

	return S_OK;
}

HRESULT MultiObjectBuffer::CreateVertexBuffer(ID3D11Device* dxDevice, ID3D11Buffer*& buffer, UINT& vertexStride, std::vector<StaticModelVertex>& vertices)
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

HRESULT MultiObjectBuffer::CreateAdditionalVertexInfomationBuffer(ID3D11Device* dxDevice, ID3D11Buffer*& buffer, UINT& stride, UINT instanceNumber)
{
	stride = sizeof(AdditionalVertexInfomation);
	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = stride * instanceNumber;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.StructureByteStride = stride;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;

	AdditionalVertexInfomation nullInfomation;
	ZeroMemory(&nullInfomation, sizeof(AdditionalVertexInfomation));
	std::vector<AdditionalVertexInfomation> vertices(instanceNumber);

	for (UINT i = 0; i < instanceNumber; i++)
	{
		memcpy(reinterpret_cast<FLOAT*>(&vertices[i]), &nullInfomation, sizeof(AdditionalVertexInfomation));
	}

	D3D11_SUBRESOURCE_DATA vertexData = {};
	ZeroMemory(&vertexData, sizeof(vertexData));
	vertexData.pSysMem = vertices.data();

	if (FAILED(dxDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &buffer)))
		return E_FAIL;

	return S_OK;
}

HRESULT MultiObjectBuffer::CreateWorldBuffer(ID3D11Device* dxDevice, ID3D11Buffer*& buffer, UINT instanceNumber, UINT& worldStride)
{
	worldStride = sizeof(VertexMatrix);
	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = worldStride * instanceNumber;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.StructureByteStride = worldStride;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	xmMatrix identity = DirectX::XMMatrixIdentity();
	VertexMatrix identityVertexMatrix;
	DirectX::XMStoreFloat4(&identityVertexMatrix.right, identity.r[0]);
	DirectX::XMStoreFloat4(&identityVertexMatrix.up, identity.r[1]);
	DirectX::XMStoreFloat4(&identityVertexMatrix.look, identity.r[2]);
	DirectX::XMStoreFloat4(&identityVertexMatrix.position, identity.r[3]);



	std::vector<VertexMatrix> vertices(instanceNumber);
	for (UINT i = 0; i < instanceNumber; i++)
	{
		memcpy(reinterpret_cast<FLOAT*>(&vertices[i]), &identityVertexMatrix, sizeof(VertexMatrix));
	}
	
	D3D11_SUBRESOURCE_DATA vertexData = {};
	ZeroMemory(&vertexData, sizeof(vertexData));
	vertexData.pSysMem = vertices.data();

	if (FAILED(dxDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &buffer)))
		return E_FAIL;

	return S_OK;
}
