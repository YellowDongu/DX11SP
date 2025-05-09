#include "pch.h"
#include "MissileTrail.h"
#include "StaticModel.h"

MissileTrail::MissileTrail(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::GameObject(dxDevice, dxDeviceContext)
{
}

MissileTrail::MissileTrail(const MissileTrail& other) : Engine::GameObject(other)
{
	particle = static_cast<MissileTrailParticle*>(GetComponent(L"MissileTrailParticle"));
}

void MissileTrail::Free(void)
{
	Engine::GameObject::Free();
}

MissileTrail* MissileTrail::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
	MissileTrail* newInstance = new MissileTrail(dxDevice, dxDeviceContext);
	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
	return newInstance;
}

Engine::GameObject* MissileTrail::Clone(void)
{
	return new MissileTrail(*this);
}

HRESULT MissileTrail::Start(void)
{
	particle = MissileTrailParticle::Create(dxDevice, dxDeviceContext);
	if (particle == nullptr)
		return E_FAIL;
	AddComponent(particle, L"MissileTrailParticle");
	maxTime = 2.5f;

	return S_OK;
}

HRESULT MissileTrail::Awake(void)
{
	if (FAILED(particle->Awake()))
		return E_FAIL;
	return S_OK;
}

void MissileTrail::Update(void)
{
	for (auto iterator = trailInfoamtion.begin(); iterator != trailInfoamtion.end(); )
	{
		if (iterator->gameObjectPointer != nullptr)
		{
			if (iterator->gameObjectPointer->Destroy())
			{
				iterator->gameObjectPointer = nullptr;
			}
			else
				UpdateTrail(*iterator);
		}
		else
		{
			UpdateTrail(*iterator, true);
			if (iterator->pointList.empty())
			{
				iterator = trailInfoamtion.erase(iterator);
				continue;
			}
		}


		iterator++;
	}

	Engine::GameObject::Update();
}

void MissileTrail::LateUpdate(void)
{
	AddRenderObject(RenderType::Effect, this);
	Engine::GameObject::LateUpdate();
}

void MissileTrail::FixedUpdate(void)
{
	Engine::GameObject::FixedUpdate();
}

void MissileTrail::Render(void)
{
	for (auto& trailInfo : trailInfoamtion)
	{
		particle->LinkTargetPoints(&trailInfo.pointList);
		particle->Render();
	}
}

HRESULT MissileTrail::AddTrail(Engine::GameObject* object)
{
	for (auto& trailInfo : trailInfoamtion)
	{
		if (trailInfo.gameObjectPointer == object)
			return S_OK;
	}

	MissileTrailParticle::TrailInfo newInfo;
	newInfo.gameObjectPointer = object;
	trailInfoamtion.push_back(newInfo);

	return S_OK;
}

void MissileTrail::UpdateTrail(MissileTrailParticle::TrailInfo& infomation, bool extinction)
{
	for (auto iterator = infomation.pointList.begin(); iterator != infomation.pointList.end(); )
	{
		iterator->second.infoFirst += DeltaTime();
		iterator->second.infoSecond.x += DeltaTime();

		if (iterator->second.infoSecond.x >= iterator->second.infoSecond.y)
		{
			iterator = infomation.pointList.erase(iterator);
			continue;
		}

		if (iterator->second.infoFirst >= 1.0f)
			iterator->second.infoFirst = 1.0f;

		iterator++;
	}

	if (extinction)
		infomation.pointList.pop_front();
	else
	{
		infomation.pointList.push_back({ infomation.gameObjectPointer->transform()->WorldMatrix(), {0.0f, {0.0f,maxTime}} });
		if (infomation.pointList.size() > particle->InstanceNumber())
			infomation.pointList.pop_front();
	}
}
