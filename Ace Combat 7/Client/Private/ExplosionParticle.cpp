#include "pch.h"
#include "ExplosionParticle.h"
#include "DefaultParticleShader.h"
ExplosionParticle::ExplosionParticle(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::MultiObjectBuffer(dxDevice, dxDeviceContext)
{
}

ExplosionParticle::ExplosionParticle(const ExplosionParticle& other) : Engine::MultiObjectBuffer(other)
{
}

void ExplosionParticle::Free(void)
{
	Engine::MultiObjectBuffer::Free();
}

ExplosionParticle* ExplosionParticle::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, UINT instanceNumber)
{
	ExplosionParticle* newInstance = new ExplosionParticle(dxDevice, dxDeviceContext);
	newInstance->instanceNumber = instanceNumber;
	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
	return newInstance;
}

Engine::Component* ExplosionParticle::Clone(void)
{
	return new ExplosionParticle(*this);
}

HRESULT ExplosionParticle::Start(void)
{
	std::vector<UINT> indices = { 0 };
	std::vector<StaticModelVertex> vertices = { {{ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f }} };

	if (FAILED(CreatePointIndexBuffer(dxDevice, indexBuffer, instanceNumber, indexCount, wholeIndexCount, indices)))
		return E_FAIL;
	if (FAILED(CreateVertexBuffer(dxDevice, vertexBuffer, vertexStride, vertices)))
		return E_FAIL;
	if (FAILED(CreateAdditionalVertexInfomationBuffer(dxDevice, additionalBuffer, additionalInfoStride, instanceNumber)))
		return E_FAIL;
	if (FAILED(CreateWorldBuffer(dxDevice, worldBuffer, instanceNumber, worldStride)))
		return E_FAIL;

	LoadTexture(L"../Bin/Resources/VFX/Common/Textures/Fire/T_Explosion07_8x8.dds", L"../Bin/Resources/VFX/Common/Textures/Fire/T_Explosion07_8x8.dds", explosionAltasTexture);
	textureInfomation = DirectX::XMFLOAT3{ 8.0f, 8.0f, 0.0f };

	return S_OK;
}

HRESULT ExplosionParticle::Awake(void)
{
	shader = GetShader(L"DefaultParticleShader");
	if (shader == nullptr)
	{
		shader = Engine::DefaultParticleShader::Create(dxDevice, dxDeviceContext);
		AddShader(L"DefaultParticleShader", shader);
	}
	return S_OK;
}

void ExplosionParticle::Update(void)
{
}

void ExplosionParticle::LateUpdate(void)
{
}

void ExplosionParticle::FixedUpdate(void)
{
}

void ExplosionParticle::Render(void)
{
	if (targetPoints == nullptr) return;
	if (additionalInfomations == nullptr) return;

	Engine::Shader* currentShader = ::GetCurrentShader();
	SetShader(shader);
	shader->BindVariable("additionalInfomation", &textureInfomation, sizeof(DirectX::XMFLOAT3));

	Matrix inversedViewMatrix;
	DirectX::XMStoreFloat4x4(&inversedViewMatrix, DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&EngineInstance()->RenderManager()->ViewMatrix())));
	SetViewProjectionMatrix();
	shader->BindMatrix("inversedViewMatrix", inversedViewMatrix);
	shader->BindTexture("diffuseTexture", explosionAltasTexture);
	shader->PassNumber(2);
	SetViewProjectionMatrix();
	if (FAILED(BindWorldBuffer(*targetPoints)))
		return;
	if (FAILED(BindAdditionalBuffer(*additionalInfomations)))
		return;

	__super::Render();
	SetShader(currentShader);
}


HRESULT ExplosionParticle::BindWorldBuffer(std::vector<Engine::VertexMatrix>& worldMatrix)
{
	D3D11_MAPPED_SUBRESOURCE		worldSubResource{};

	if (FAILED(dxDeviceContext->Map(worldBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &worldSubResource)))
	{
		return E_FAIL;
	}
	Engine::VertexMatrix* matrixBuffer = static_cast<Engine::VertexMatrix*>(worldSubResource.pData);

	if (targetPoints->size() > instanceNumber)
		memcpy(matrixBuffer, worldMatrix.data(), sizeof(Engine::VertexMatrix) * instanceNumber);
	else
		memcpy(matrixBuffer, worldMatrix.data(), sizeof(Engine::VertexMatrix) * worldMatrix.size());

	dxDeviceContext->Unmap(worldBuffer, 0);
	return S_OK;
}

HRESULT ExplosionParticle::BindAdditionalBuffer(std::vector<Engine::AdditionalVertexInfomation>& additionalInfomation)
{
	D3D11_MAPPED_SUBRESOURCE		subResource{};
	Engine::AdditionalVertexInfomation defaultStructForTemp;
	defaultStructForTemp.infoFirst = 1.0f;
	defaultStructForTemp.infoSecond = { 1.0f,1.0f };
	if (FAILED(dxDeviceContext->Map(additionalBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource)))
		return E_FAIL;
	Engine::AdditionalVertexInfomation* buffer = static_cast<Engine::AdditionalVertexInfomation*>(subResource.pData);

	if (targetPoints->size() > instanceNumber)
		memcpy(buffer, additionalInfomation.data(), sizeof(Engine::AdditionalVertexInfomation) * instanceNumber);
	else
		memcpy(buffer, additionalInfomation.data(), sizeof(Engine::AdditionalVertexInfomation) * additionalInfomation.size());

	dxDeviceContext->Unmap(additionalBuffer, 0);
	return S_OK;
}
