#include "pch.h"
#include "SurfaceToAirMissileFireControlSystem.h"
#include "SimpleBullet.h"
#include "StandardMissile.h"
#include "RMWR.h"

SurfaceToAirMissileFireControlSystem::SurfaceToAirMissileFireControlSystem(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : FireControlSystem(dxDevice, dxDeviceContext)
{
}

SurfaceToAirMissileFireControlSystem::SurfaceToAirMissileFireControlSystem(const SurfaceToAirMissileFireControlSystem& other) : FireControlSystem(other)
{
}

void SurfaceToAirMissileFireControlSystem::Free(void)
{
	FCS::Free();
}

SurfaceToAirMissileFireControlSystem* SurfaceToAirMissileFireControlSystem::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, ObjectInfomation& metaData)
{
	SAMFCS* newInstance = new SAMFCS(dxDevice, dxDeviceContext);
	newInstance->metaData = metaData;

	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}

	return newInstance;
}

Engine::Component* SurfaceToAirMissileFireControlSystem::Clone(void)
{
	return new SAMFCS(*this);
}

HRESULT SurfaceToAirMissileFireControlSystem::Start(void)
{
	//bullet = SimpleBullet::Create(dxDevice, dxDeviceContext);
	//if (bullet == nullptr)
	//	return E_FAIL;

	standardMissile = StandardMissile::Create(dxDevice, dxDeviceContext);
	if (standardMissile == nullptr)
		return E_FAIL;

	bullet = nullptr;
	uniqueMissile = nullptr;
	uniqueMissileFired = false;
	selectedWeapon = 0;

	return S_OK;
}

HRESULT SurfaceToAirMissileFireControlSystem::Awake(void)
{
	standardMissile->Awake();


	return S_OK;
}

void SurfaceToAirMissileFireControlSystem::Update(void)
{
	standardMissileFired = false;

	weaponRelease = Locked();
	Lock();

	for (auto iterator = activeMissile.begin(); iterator != activeMissile.end(); )
	{
		if ((*iterator)->Detonated())
		{
			Base::Destroy((*iterator));
			iterator = activeMissile.erase(iterator);
		}
		else
		{

			(*iterator)->Update();
			iterator++;
		}
	}

	MissileControl();

	if (targeted != nullptr && targeted->Destroy())
		targeted = nullptr;
}

void SurfaceToAirMissileFireControlSystem::LateUpdate(void)
{
	for (auto& missile : activeMissile)
	{
		if (!missile->Detonated())
			missile->LateUpdate();
	}

	if (targeted != nullptr && targeted->Destroy())
		targeted = nullptr;
}

void SurfaceToAirMissileFireControlSystem::FixedUpdate(void)
{
}

void SurfaceToAirMissileFireControlSystem::MissileControl(void)
{
	selectedWeapon = 0;
	gunFired = false;

	coolDownLStandardMissile -= DeltaTime() * coolDownLStandardMissileSpeed;


	if (!lock && weaponRelease)
	{
		if (coolDownLStandardMissile <= 0.0f)
		{
			activeMissile.push_back(standardMissile->Launch(gameObject, Vector3::zero(), targeted));
			coolDownLStandardMissile = 1.0f;
			standardMissileCount--;

			if (targetedRMWR != nullptr)
				targetedRMWR->FiredToMe(activeMissile.back());
			standardMissileFired = true;
		}
	}

	weaponRelease = false;
}
