#include "Foundation.h"
#include "ColliderManager.h"

using namespace Engine;

ColliderManager::ColliderManager(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : device(dxDevice), context(dxDeviceContext)
{
	navMesh = NavMesh::Create(device, context);
}

void ColliderManager::Free(void)
{
	Base::DestroyInstance(navMesh);
}

ColliderManager* ColliderManager::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
	ColliderManager* newInstance = new ColliderManager(dxDevice, dxDeviceContext);
	
	return newInstance;
}

void ColliderManager::Update(void)
{
	for (auto& targetCollider : colliders)
	{
		targetCollider->Update();
	}
}

void ColliderManager::LateUpdate(void)
{
	for (auto& collider : colliders)
	{
		for (auto& targetCollider : colliders)
		{
			if (collider == targetCollider)
				continue;

			if (collider->Intersect(targetCollider))
			{
				if (collider->GetOwner() != nullptr)
					collider->GetOwner()->Collide(targetCollider->GetOwner(), targetCollider);

				if (targetCollider->GetOwner() != nullptr)
					targetCollider->GetOwner()->Collide(collider->GetOwner(), collider);
			}

		}
	}
	colliders.clear();
}

void ColliderManager::Render(void)
{
}

HRESULT ColliderManager::LoadNavMesh(std::wstring filePath)
{
	if (navMesh == nullptr)
	{
		navMesh = NavMesh::Create(device, context);
		if (navMesh == nullptr)
			return E_FAIL;
	}

	return navMesh->LoadMesh(filePath);
}

bool ColliderManager::CheckCollision(Collider* collider, Collider* otherCollider)
{
	return false;
}
