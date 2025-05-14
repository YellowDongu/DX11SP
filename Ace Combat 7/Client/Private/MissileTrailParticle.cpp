#include "pch.h"
#include "MissileTrailParticle.h"
#include "DefaultParticleShader.h"

MissileTrailParticle::MissileTrailParticle(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::MultiObjectBuffer(dxDevice, dxDeviceContext)
{
}

MissileTrailParticle::MissileTrailParticle(const MissileTrailParticle& other) : Engine::MultiObjectBuffer(other)
{
}

void MissileTrailParticle::Free(void)
{
	Engine::MultiObjectBuffer::Free();
}

MissileTrailParticle* MissileTrailParticle::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
	MissileTrailParticle* newInstance = new MissileTrailParticle(dxDevice, dxDeviceContext);

	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}

	return newInstance;
}

Engine::Component* MissileTrailParticle::Clone(void)
{
	return new MissileTrailParticle(*this);
}

HRESULT MissileTrailParticle::Start(void)
{
	//if (FAILED(::LoadTexture(L"../Bin/Resources/VFX/Common/Textures/Missile/smokeTrail_303.dds", L"../Bin/Resources/VFX/Common/Textures/Missile/smokeTrail_303.dds", texture)))
	if (FAILED(::LoadTexture(L"../Bin/Resources/VFX/Common/Textures/Smoke/Anim_8X8_Wisp_01.dds", L"../Bin/Resources/VFX/Common/Textures/Smoke/Anim_8X8_Wisp_01.dds", texture)))
		return E_FAIL;

	std::vector<UINT> indices = { 0 };
	std::vector<StaticModelVertex> vertices = { {{ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f }} };
	instanceNumber = 250;
	if (FAILED(CreatePointIndexBuffer(dxDevice, indexBuffer, instanceNumber, indexCount, wholeIndexCount, indices)))
		return E_FAIL;
	if (FAILED(CreateVertexBuffer(dxDevice, vertexBuffer, vertexStride, vertices)))
		return E_FAIL;
	if (FAILED(CreateAdditionalVertexInfomationBuffer(dxDevice, additionalBuffer, additionalInfoStride, instanceNumber)))
		return E_FAIL;
	if (FAILED(CreateWorldBuffer(dxDevice, worldBuffer, instanceNumber, worldStride)))
		return E_FAIL;

	refinedMatrix.resize(instanceNumber);
	refinedInfomation.resize(instanceNumber);

	return S_OK;
}

HRESULT MissileTrailParticle::Awake(void)
{
	shader = GetShader(L"DefaultParticleShader");
	if (shader == nullptr)
	{
		shader = Engine::DefaultParticleShader::Create(dxDevice, dxDeviceContext);
		AddShader(L"DefaultParticleShader", shader);
	}
	trailBuffer = shader->ConstantBuffer(L"TrailBuffer");
	if (trailBuffer == nullptr)
		return E_FAIL;
	rotationBuffer = shader->ConstantBuffer(L"RotationMatrixBuffer");
	if (rotationBuffer == nullptr)
		return E_FAIL;

	return S_OK;
}

void MissileTrailParticle::Render(void)
{
	const std::wstring& currentShaderName = ::GetCurrentShaderName();
	if (FAILED(SetShader(shader)))
	{
		targets = nullptr;
		return;
	}
	shader->PassNumber(3);
	shader->BindTexture("diffuseTexture", texture);
	::SetViewProjectionMatrix();

	if (FAILED(UpdateInfomation()))
		return;
	BindWorldBuffer();
	BindAdditionalBuffer();
	__super::Render();
	SetShader(currentShaderName);
}

HRESULT MissileTrailParticle::UpdateInfomation(void)
{
	if (targetList == nullptr || targetList->empty())
		return E_FAIL;

	auto iterator = targetList->begin(), iteratorEnd = targetList->end();
	xmMatrix identity = DirectX::XMMatrixIdentity();
	for (UINT i = 0; i < instanceNumber; i++)
	{
		if (iterator == iteratorEnd)
		{
			ZeroMemory(&refinedMatrix[i], sizeof(Engine::VertexMatrix));
			ZeroMemory(&refinedInfomation[i], sizeof(Engine::AdditionalVertexInfomation));
		}
		else
		{
			DirectX::XMStoreFloat4(&refinedMatrix[i].right, identity.r[0]);
			DirectX::XMStoreFloat4(&refinedMatrix[i].up, identity.r[1]);
			DirectX::XMStoreFloat4(&refinedMatrix[i].look, identity.r[2]);
			refinedMatrix[i].position = float4{ iterator->first.first.x, iterator->first.first.y, iterator->first.first.z, 1.0f };

			memcpy(&refinedInfomation[i], &iterator->second, sizeof(Engine::AdditionalVertexInfomation));
			iterator++;
		}

	}

	Matrix viewInverseMatix;
	DirectX::XMStoreFloat4x4(&viewInverseMatix, DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&::GetPipeLineStatus().viewMatrix)));
	shader->BindMatrix("inversedViewMatrix", viewInverseMatix);

	return S_OK;

	//if (targets == nullptr || targets->empty())
	//	return E_FAIL;
	//
	//auto iterator = targets->begin(), iteratorEnd = targets->end();
	////UINT size = targets->size() - 1;
	//
	//for (UINT i = 0; i < instanceNumber; i++)
	//{
	//	ZeroMemory(&refinedMatrix[i], sizeof(Engine::VertexMatrix));
	//	if (iterator == iteratorEnd)
	//	{
	//		refinedMatrix[i].right = float4{ static_cast<FLOAT>(i), 0.0f, 0.0f, 0.0f };
	//		ZeroMemory(&refinedInfomation[i], sizeof(Engine::AdditionalVertexInfomation));
	//	}
	//	else
	//	{
	//		refinedMatrix[i].right = float4{ static_cast<FLOAT>(i), iterator->first.second, 0.0f, 0.0f };
	//		memcpy(&refinedInfomation[i], &iterator->second, sizeof(Engine::AdditionalVertexInfomation));
	//		iterator++;
	//	}
	//
	//}
	//
	//return S_OK;
	//
	//xmMatrix matrix;
	//for (UINT i = 0; i < instanceNumber; i++)
	//{
	//	if (iterator == iteratorEnd)
	//	{
	//		ZeroMemory(&refinedMatrix[i], sizeof(Engine::VertexMatrix));
	//		ZeroMemory(&refinedInfomation[i], sizeof(Engine::AdditionalVertexInfomation));
	//	}
	//	else
	//	{
	//		//matrix = DirectX::XMLoadFloat4x4(&iterator->first);
	//		DirectX::XMStoreFloat4(&refinedMatrix[i].right, matrix.r[0]);
	//		DirectX::XMStoreFloat4(&refinedMatrix[i].up, matrix.r[1]);
	//		DirectX::XMStoreFloat4(&refinedMatrix[i].look, matrix.r[2]);
	//		DirectX::XMStoreFloat4(&refinedMatrix[i].position, matrix.r[3]);
	//
	//		memcpy(&refinedInfomation[i], &iterator->second, sizeof(Engine::AdditionalVertexInfomation));
	//
	//		iterator++;
	//	}
	//}
	//
	//targets = nullptr;
	//return S_OK;
}

HRESULT MissileTrailParticle::BindWorldBuffer(void)
{
	D3D11_MAPPED_SUBRESOURCE worldSubResource{};

	if (FAILED(dxDeviceContext->Map(worldBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &worldSubResource)))
		return E_FAIL;

	Engine::VertexMatrix* matrixBuffer = static_cast<Engine::VertexMatrix*>(worldSubResource.pData);
	memcpy(matrixBuffer, refinedMatrix.data(), sizeof(Engine::VertexMatrix) * refinedMatrix.size());

	dxDeviceContext->Unmap(worldBuffer, 0);
	return S_OK;
}

HRESULT MissileTrailParticle::BindAdditionalBuffer(void)
{
	D3D11_MAPPED_SUBRESOURCE subResource{};

	if (FAILED(dxDeviceContext->Map(additionalBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource)))
		return E_FAIL;

	Engine::AdditionalVertexInfomation* buffer = static_cast<Engine::AdditionalVertexInfomation*>(subResource.pData);
	memcpy(buffer, refinedInfomation.data(), sizeof(Engine::AdditionalVertexInfomation) * refinedInfomation.size());
	dxDeviceContext->Unmap(additionalBuffer, 0);

	return S_OK;
}

HRESULT MissileTrailParticle::BindTrailConstantBuffer(void)
{
	D3D11_MAPPED_SUBRESOURCE subResource{};
	auto iterator = targets->begin();
	auto iteratorSecond = targets->begin();
	auto iteratorEnd = targets->end();
	iteratorSecond++;

	for (size_t i = 0; i < 256; i++)
	{
		if (iteratorSecond == iteratorEnd)
		{
			ZeroMemory(&trailBufferStorage.frontMatrix[i], sizeof(DirectX::XMFLOAT4X4));
			ZeroMemory(&trailBufferStorage.rearMatrix[i], sizeof(DirectX::XMFLOAT4X4));
		}
		else
		{
			trailBufferStorage.frontMatrix[i] = iterator->first.first;
			trailBufferStorage.rearMatrix[i] = iteratorSecond->first.first;

			iterator++;
			iteratorSecond++;
		}
	}

	if (FAILED(dxDeviceContext->Map(trailBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource)))
		return E_FAIL;

	TrailBuffer* buffer = static_cast<TrailBuffer*>(subResource.pData);
	memcpy(buffer, &trailBufferStorage, sizeof(TrailBuffer));
	dxDeviceContext->Unmap(trailBuffer, 0);

	BindConstantBuffer("TrailBuffer", trailBuffer);


	if (FAILED(dxDeviceContext->Map(rotationBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource)))
		return E_FAIL;

	rotationMatrixBuffer* otherBuffer = static_cast<rotationMatrixBuffer*>(subResource.pData);
	memcpy(otherBuffer, &rotationStorage, sizeof(rotationMatrixBuffer));
	dxDeviceContext->Unmap(rotationBuffer, 0);

	BindConstantBuffer("RotationMatrixBuffer", rotationBuffer);

	
	return S_OK;
}


