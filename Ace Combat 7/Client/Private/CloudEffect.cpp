#include "pch.h"
#include "CloudEffect.h"
#include "DefaultParticleShader.h"

CloudEffect::CloudEffect(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::MultiObjectBuffer(dxDevice, dxDeviceContext)
{
}

CloudEffect::CloudEffect(const CloudEffect& other) : Engine::MultiObjectBuffer(other)
{

}

void CloudEffect::Free(void)
{
	Engine::MultiObjectBuffer::Free();
}

CloudEffect* CloudEffect::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
	CloudEffect* newInstance = new CloudEffect(dxDevice, dxDeviceContext);
	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
	return newInstance;
}

Engine::Component* CloudEffect::Clone(void)
{
	return new CloudEffect(*this);
}

HRESULT CloudEffect::Start(void)
{

	std::vector<UINT> indices = { 0 };
	std::vector<StaticModelVertex> vertices = { {{ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f }} };

	instanceNumber = 500;

	if (FAILED(CreatePointIndexBuffer(dxDevice, indexBuffer, instanceNumber, indexCount, wholeIndexCount, indices)))
		return E_FAIL;
	if (FAILED(CreateVertexBuffer(dxDevice, vertexBuffer, vertexStride, vertices)))
		return E_FAIL;
	if (FAILED(CreateAdditionalVertexInfomationBuffer(dxDevice, additionalBuffer, additionalInfoStride, instanceNumber)))
		return E_FAIL;
	if (FAILED(CreateWorldBuffer(dxDevice, worldBuffer, instanceNumber, worldStride)))
		return E_FAIL;

	//if (FAILED(LoadTexture(L"../Bin/Resources/VFX/Common/Textures/Smoke/T_DensitySmoke01_5x5.dds", L"../Bin/Resources/VFX/Common/Textures/Smoke/T_DensitySmoke01_5x5.dds", cloudTexture)))
	//if (FAILED(LoadTexture(L"../Bin/Resources/VFX/Common/Textures/Smoke/T_DensitySmoke02_5x5.dds", L"../Bin/Resources/VFX/Common/Textures/Smoke/T_DensitySmoke02_5x5.dds", cloudTexture)))
	//if (FAILED(LoadTexture(L"../Bin/Resources/VFX/Common/Textures/Smoke/T_SmokeBall_01_8X8.dds", L"../Bin/Resources/VFX/Common/Textures/Smoke/T_SmokeBall_01_8X8.dds", cloudTexture)))
	//if (FAILED(LoadTexture(L"../Bin/Resources/VFX/Common/Textures/Smoke/T_SmokePuff01_4x2.dds", L"../Bin/Resources/VFX/Common/Textures/Smoke/T_SmokePuff01_4x2.dds", cloudTexture)))
	if (FAILED(LoadTexture(L"../Bin/Resources/VFX/Common/Textures/Smoke/Anim_8X8_Wisp_01.dds", L"../Bin/Resources/VFX/Common/Textures/Smoke/Anim_8X8_Wisp_01.dds", cloudTexture)))
		return E_FAIL;


	return S_OK;
}

HRESULT CloudEffect::Awake(void)
{
	shader = GetShader(L"DefaultParticleShader");
	if (shader == nullptr)
	{
		shader = Engine::DefaultParticleShader::Create(dxDevice, dxDeviceContext);
		AddShader(L"DefaultParticleShader", shader);
	}
	return S_OK;
}

void CloudEffect::Update(void)
{
}

void CloudEffect::LateUpdate(void)
{
}

void CloudEffect::FixedUpdate(void)
{
}

void CloudEffect::Render(void)
{
	if (FAILED(SetShader(shader)))
		return;
	Matrix inversedViewMatrix;
	DirectX::XMStoreFloat4x4(&inversedViewMatrix, DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&EngineInstance()->RenderManager()->ViewMatrix())));
	shader->BindMatrix("inversedViewMatrix", inversedViewMatrix);
	shader->BindTexture("diffuseTexture", cloudTexture);

	shader->PassNumber(4);
	SetViewProjectionMatrix();
	if (FAILED(BindWorldBuffer()))
		return;
	BindPointParticleAssembler();
	dxDeviceContext->DrawIndexedInstanced(indexCount, instanceNumber, 0, 0, 0);
}

HRESULT CloudEffect::BindWorldBuffer(void)
{
	if (vertexMatrix == nullptr)
		return E_FAIL;

	D3D11_MAPPED_SUBRESOURCE worldSubResource{};

	if (FAILED(dxDeviceContext->Map(worldBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &worldSubResource)))
		return E_FAIL;

	Engine::VertexMatrix* matrixBuffer = static_cast<Engine::VertexMatrix*>(worldSubResource.pData);

	if (vertexMatrix->size() >= instanceNumber)
		memcpy(matrixBuffer, vertexMatrix->data(), sizeof(Engine::VertexMatrix) * instanceNumber);
	else
		memcpy(matrixBuffer, vertexMatrix->data(), sizeof(Engine::VertexMatrix) * vertexMatrix->size());

	dxDeviceContext->Unmap(worldBuffer, 0);
	return S_OK;
}
