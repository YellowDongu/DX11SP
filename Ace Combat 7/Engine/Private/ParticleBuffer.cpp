#include "Foundation.h"
#include "ParticleBuffer.h"

using namespace Engine;



const std::vector<D3D11_INPUT_ELEMENT_DESC> ParticleMatrix::inputLayout = {
   { "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
   { "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT,    0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0 },

   { "TEXCOORD",  1, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0,   D3D11_INPUT_PER_INSTANCE_DATA, 1 },
   { "TEXCOORD",  2, DXGI_FORMAT_R32G32B32_FLOAT, 1, 16,  D3D11_INPUT_PER_INSTANCE_DATA, 1 },
   { "TEXCOORD",  3, DXGI_FORMAT_R32G32B32_FLOAT, 1, 32,  D3D11_INPUT_PER_INSTANCE_DATA, 1 },
   { "TEXCOORD",  4, DXGI_FORMAT_R32G32B32_FLOAT, 1, 48,  D3D11_INPUT_PER_INSTANCE_DATA, 1 }
};

Engine::ParticleBuffer::ParticleBuffer(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : MultiObjectBuffer(dxDevice, dxDeviceContext)
{
}

Engine::ParticleBuffer::ParticleBuffer(const ParticleBuffer& other) : MultiObjectBuffer(other)
{
}

void Engine::ParticleBuffer::Free(void)
{
}

ParticleBuffer* Engine::ParticleBuffer::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
	return nullptr;
}

Component* Engine::ParticleBuffer::Clone(void)
{
	return nullptr;
}

HRESULT Engine::ParticleBuffer::Start(void)
{
	return E_NOTIMPL;
}

void Engine::ParticleBuffer::Update(void)
{
}

void Engine::ParticleBuffer::LateUpdate(void)
{
}

void Engine::ParticleBuffer::FixedUpdate(void)
{
}

HRESULT Engine::ParticleBuffer::CreateBuffers(void)
{
	//if (FAILED(CreateIndexBuffer(dxDevice, indexBuffer, instanceNumber))
	//	{
	//	}
	//if (FAILED(CreateVertexBuffer(dxDevice, indexBuffer))
	//	{
	//	}
	//if (FAILED(CreateWorldBuffer(dxDevice, worldBuffer, instanceNumber))
	//{
	//}

	return E_NOTIMPL;
}

HRESULT Engine::ParticleBuffer::CreateWorldBuffer(ID3D11Device* dxDevice, ID3D11Buffer*& vertexBuffer, ID3D11Buffer*& indexBuffer, std::vector<ParticleMatrix>& data)
{
    return E_NOTIMPL;
}

HRESULT Engine::ParticleBuffer::CreateIndexBuffer(ID3D11Device* dxDevice, ID3D11Buffer*& indexBuffer, UINT instanceNumber, std::vector<UINT>& indices)
{
    return E_NOTIMPL;
}


HRESULT Engine::ParticleBuffer::CreateVertexBuffer(ID3D11Device* dxDevice, ID3D11Buffer*& indexBuffer)
{
	return E_NOTIMPL;
}

HRESULT Engine::ParticleBuffer::CreateWorldBuffer(ID3D11Device* dxDevice, ID3D11Buffer*& worldBuffer, UINT instanceNumber, std::vector<ModelVertex>& vertices)
{
    //// 버텍스 버퍼 생성
    //D3D11_BUFFER_DESC vertexBufferDesc = {};
    //vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    //vertexBufferDesc.ByteWidth = (UINT)(sizeof(ModelVertex) * vertices.size());
    //vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    //
    //D3D11_SUBRESOURCE_DATA vertexData = {};
    //ZeroMemory(&vertexData, sizeof(vertexData));
    //vertexData.pSysMem = vertices.data();
    //
    //if (FAILED(dxDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer)))
    //    return E_FAIL;
    //
    //// 인덱스 버퍼 생성
    //D3D11_BUFFER_DESC indexBufferDesc = {};
    //indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    //indexBufferDesc.ByteWidth = (UINT)(sizeof(UINT) * indices.size());
    //indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    //
    //D3D11_SUBRESOURCE_DATA indexData = {};
    //ZeroMemory(&indexData, sizeof(indexData));
    //indexData.pSysMem = indices.data();
    //
    //if (FAILED(dxDevice->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer)))
    //    return E_FAIL;
    //
    //return S_OK;
    return E_NOTIMPL;
}
