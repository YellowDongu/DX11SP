#include "pch.h"
#include "SkyBox.h"
#include "DefaultCubeShader.h"
#include "MainCamera.h"

SkyBox::SkyBox(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::GameObject(dxDevice, dxDeviceContext)
{
}

void SkyBox::Free(void)
{
	vertexBuffer->Release();
	indexBuffer->Release();
	Base::DestroyInstance(transformComponent);
}

SkyBox* SkyBox::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, Engine::GameObject* mainCamera)
{
	SkyBox* newInstance = new SkyBox(dxDevice, dxDeviceContext);
	newInstance->mainCamera = mainCamera;
	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
    return newInstance;
}

Engine::GameObject* SkyBox::Clone(void)
{
    return nullptr;
}

HRESULT SkyBox::Start(void)
{
	CreateTransform();
	//transformComponent->Scale() *= 50.0f;
	shader = Engine::DefaultCubeShader::Create(dxDevice, dxDeviceContext);
    if (FAILED(AddShader(shader->shaderFile, shader)))
        Base::DestroyInstance(shader);

    if (FAILED(CreateMesh(dxDevice, vertexBuffer, indexBuffer, offset, stride, indexCount)))
        return E_FAIL;

	skyColorEnd = float4{ 85.0f / 256.0f, 113.0f / 256.0f, 131.0f / 256.0f, 1.0f };
	skyColorBegin = float4{ 213.0f / 256.0f, 210.0f / 256.0f, 216.0f / 256.0f, 1.0f };
    return S_OK;
}

HRESULT SkyBox::Awake(void)
{
    return E_NOTIMPL;
}

void SkyBox::Update(void)
{
}

void SkyBox::LateUpdate(void)
{
	transformComponent->Position() = mainCamera->transform()->Position();
	AddRenderObject(RenderType::Priority, this);
}

void SkyBox::FixedUpdate(void)
{
}

void SkyBox::Render(void)
{
	SetShader(shader->shaderFile);
	shader->PassNumber(1);
	Matrix matrix = static_cast<MainCamera*>(mainCamera)->ViewProjectionMatrix();
	shader->BindMatrix(shader->viewProjectionMatrixA, matrix);
	transformComponent->UpdateWorldMatrix();
	SetMatrix(GetCurrentShader()->worldMatrixA, transformComponent->WorldMatrix());
	shader->BindVariable("skyBoxStartColor", &skyColorBegin, static_cast<UINT>(sizeof(FLOAT) * 4));
	shader->BindVariable("skyBoxEndColor", &skyColorEnd, static_cast<UINT>(sizeof(FLOAT) * 4));
	shader->Render(indexBuffer, vertexBuffer,stride);
	dxDeviceContext->DrawIndexed(indexCount, 0, 0);

	SetShader(L"DefaultModelVSShader.hlsl");
}

HRESULT SkyBox::CreateMesh(ID3D11Device* dxDevice, ID3D11Buffer*& vertexBuffer, ID3D11Buffer*& indexBuffer, UINT& offset, UINT& stride, UINT& indexCount)
{
    std::vector<CubeVertex> vertices;
    std::vector<UINT> indices;

    CreateMeshData(vertices, indices);

    offset = 0;
    stride = sizeof(CubeVertex);
    indexCount = static_cast<UINT>(indices.size());
    return CreateBuffer(dxDevice, vertexBuffer, indexBuffer, vertices, indices);
}

void SkyBox::CreateMeshData(std::vector<CubeVertex>& vertices, std::vector<UINT>& indices)
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

HRESULT SkyBox::CreateBuffer(ID3D11Device* dxDevice, ID3D11Buffer*& vertexBuffer, ID3D11Buffer*& indexBuffer, std::vector<CubeVertex>& vertices, std::vector<UINT>& indices)
{
    // 버텍스 버퍼 생성
    D3D11_BUFFER_DESC vertexBufferDesc = {};
	ZeroMemory(&vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = (UINT)(sizeof(CubeVertex) * vertices.size());
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.StructureByteStride = (UINT)(sizeof(CubeVertex));
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA vertexData = {};
    ZeroMemory(&vertexData, sizeof(vertexData));
    vertexData.pSysMem = vertices.data();

    if (FAILED(dxDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer)))
        return E_FAIL;

    // 인덱스 버퍼 생성
    D3D11_BUFFER_DESC indexBufferDesc = {};
	ZeroMemory(&indexBufferDesc, sizeof(D3D11_BUFFER_DESC));
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = (UINT)(sizeof(UINT) * indices.size());
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	vertexBufferDesc.StructureByteStride = (UINT)(sizeof(UINT));
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA indexData = {};
    ZeroMemory(&indexData, sizeof(indexData));
    indexData.pSysMem = indices.data();

    if (FAILED(dxDevice->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer)))
        return E_FAIL;

    return S_OK;
}
