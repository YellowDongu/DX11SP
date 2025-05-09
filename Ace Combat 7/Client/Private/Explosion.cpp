#include "pch.h"
#include "Explosion.h"

Explosion::Explosion(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::GameObject(dxDevice, dxDeviceContext)
{
}

Explosion::Explosion(const Explosion& other) : Engine::GameObject(other)
{
	particle = static_cast<ExplosionParticle*>(GetComponent(L""));
}

void Explosion::Free(void)
{
	Engine::GameObject::Free();
}

Explosion* Explosion::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
	Explosion* newInstance = new Explosion(dxDevice, dxDeviceContext);

	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}

	return newInstance;
}

Engine::GameObject* Explosion::Clone(void)
{
	return new Explosion(*this);
}

HRESULT Explosion::Start(void)
{
	particle = ExplosionParticle::Create(dxDevice, dxDeviceContext, 100);
	AddComponent(particle, L"ExplosionParticle");
	particle->Awake();

	targetPoints.resize(particle->InstanceNumber());
	additionalInfomations.resize(particle->InstanceNumber());
	return S_OK;
}

void Explosion::Update(void)
{
	if (ExplosionQueue.empty())
		return;

	for (auto iterator = ExplosionQueue.begin(); iterator != ExplosionQueue.end(); )
	{
		if (iterator->second.infoSecond.x >= iterator->second.infoSecond.y)
		{
			iterator = ExplosionQueue.erase(iterator);
		}
		else
		{
			iterator->second.infoSecond.x += DeltaTime();
			if (iterator->second.infoSecond.x >= iterator->second.infoSecond.y)
				iterator->second.infoSecond.x = iterator->second.infoSecond.y;
			iterator++;
		}
	}
}

void Explosion::LateUpdate(void)
{
	if (ExplosionQueue.empty())
		return;

	AddRenderObject(RenderType::Effect, this);

	auto iterator = ExplosionQueue.begin();
	auto iteratorEnd = ExplosionQueue.end();
	size_t size = targetPoints.size();
	for (size_t i = 0; i < size; i++)
	{
		if (iterator == iteratorEnd)
		{
			ZeroMemory(&targetPoints[i], sizeof(Engine::VertexMatrix));
			ZeroMemory(&additionalInfomations[i], sizeof(Engine::AdditionalVertexInfomation));
		}
		else
		{
			memcpy(&targetPoints[i], &iterator->first, sizeof(Engine::VertexMatrix));
			memcpy(&additionalInfomations[i], &iterator->second, sizeof(Engine::AdditionalVertexInfomation));

			iterator++;
		}
	}
}

void Explosion::FixedUpdate(void)
{
}

void Explosion::Render(void)
{
	particle->LinkInfomation(&targetPoints, &additionalInfomations);
	particle->Render();
}

void Explosion::AddExplosion(Vector3 position, Vector3 scale)
{
	Engine::VertexMatrix vertexMatrix;
	Engine::AdditionalVertexInfomation infomation;
	xmMatrix worldMatrix = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	worldMatrix.r[3] = DirectX::XMVectorSet(position.x, position.y, position.z, 1.0f);

	DirectX::XMStoreFloat4(&vertexMatrix.right, worldMatrix.r[0]);
	DirectX::XMStoreFloat4(&vertexMatrix.up, worldMatrix.r[1]);
	DirectX::XMStoreFloat4(&vertexMatrix.look, worldMatrix.r[2]);
	DirectX::XMStoreFloat4(&vertexMatrix.position, worldMatrix.r[3]);
	infomation.infoFirst = 64.0f;
	infomation.infoSecond = float2{ 0.0f,1.0f };

	ExplosionQueue.push_back({ vertexMatrix, infomation });
}
