#include "Foundation.h"
#include "UIObject.h"

using namespace Engine;



HRESULT UIObject::CreateScale(ID3D11ShaderResourceView* textureView, Vector2& scale)
{
	ID3D11Resource* textureResource = nullptr;
	ID3D11Texture2D* texture2D = nullptr;
	D3D11_TEXTURE2D_DESC desc;

	textureView->GetResource(&textureResource);
	if (FAILED(textureResource->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&texture2D)))
	{
		scale = { 1.0f, 1.0f };
		textureResource->Release();
		return E_FAIL;
	}

	texture2D->GetDesc(&desc);
	scale = { static_cast<FLOAT>(desc.Width), static_cast<FLOAT>(desc.Height) };

	texture2D->Release();
	textureResource->Release();
	return S_OK;
}

HRESULT UIObject::CreateVertex(ID3D11Buffer*& vertexBuffer, ID3D11Buffer*& indexBuffer, UINT& vertexCount, const Vector2& scale)
{
	std::vector<UIVertex> vertex;
	std::vector<UINT> index;
	CreateVertex(vertex, index, scale);
	vertexCount = static_cast<UINT>(index.size());
	return CreateBuffer(dxDevice, vertexBuffer, indexBuffer, vertex, index);
}

DirectX::XMMATRIX UIObject::CreateMatrix(const Vector2& position, const Vector2& scale, FLOAT angle)
{
	DirectX::XMFLOAT3 tempPosition = DirectX::XMFLOAT3(position.x, position.y, 0.1f);
	DirectX::XMVECTOR translation = DirectX::XMLoadFloat3(&tempPosition);

	DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixScaling(scale.x, scale.y, 1.0f) * DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(angle));
	memcpy(&worldMatrix.r[3], &translation, sizeof(DirectX::XMVECTOR));
	return worldMatrix;
}

HRESULT UIObject::CreateBuffer(ID3D11Device* dxDevice, ID3D11Buffer*& vertexBuffer, ID3D11Buffer*& indexBuffer, std::vector<UIVertex>& vertices, std::vector<UINT>& indices)
{
	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = (UINT)(sizeof(AdvencedVertex) * vertices.size());
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertexData = {};
	ZeroMemory(&vertexData, sizeof(vertexData));
	vertexData.pSysMem = vertices.data();

	if (FAILED(dxDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer)))
		return E_FAIL;

	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = (UINT)(sizeof(UINT) * indices.size());
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexData = {};
	ZeroMemory(&indexData, sizeof(indexData));
	indexData.pSysMem = indices.data();

	if (FAILED(dxDevice->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer)))
		return E_FAIL;

	return S_OK;
}


void UIObject::CreateVertex(std::vector<UIVertex>& vertex, std::vector<UINT>& index, const Vector2& scale)
{
	vertex.clear();
	UIVertex newVertex;

	newVertex.position = { scale.x / -2.0f, scale.y / -2.0f };
	newVertex.texturePosition = { 0.0f, 0.0f };
	vertex.push_back(newVertex);

	newVertex.position = { scale.x / 2.0f, scale.y / -2.0f };
	newVertex.texturePosition = { 1.0f, 0.0f };
	vertex.push_back(newVertex);

	newVertex.position = { scale.x / 2.0f, scale.y / 2.0f };
	newVertex.texturePosition = { 1.0f, 1.0f };
	vertex.push_back(newVertex);

	newVertex.position = { scale.x / -2.0f, scale.y / 2.0f };
	newVertex.texturePosition = { 0.0f, 1.0f };
	vertex.push_back(newVertex);

	index.push_back(0);
	index.push_back(2);
	index.push_back(1);

	index.push_back(0);
	index.push_back(3);
	index.push_back(2);
}

#include "GameInstance.h"

HRESULT UIParts::LoadUITexture(const std::wstring filePath)
{
	HRESULT result = LoadTexture(filePath, filePath, texture);
	if (SUCCEEDED(result))
		texture->AddRef();
	return result;
}
