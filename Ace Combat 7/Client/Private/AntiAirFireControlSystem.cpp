#include "pch.h"
#include "AntiAirFireControlSystem.h"
#include "SimpleBullet.h"
#include "Missile.h"
AntiAirFireControlSystem::AntiAirFireControlSystem(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : FireControlSystem(dxDevice, dxDeviceContext)
{
}

AntiAirFireControlSystem::AntiAirFireControlSystem(const AntiAirFireControlSystem& other) : FireControlSystem(other)
{

}

void AntiAirFireControlSystem::Free(void)
{
	FireControlSystem::Free();
}

AntiAirFireControlSystem* AntiAirFireControlSystem::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, AircraftMetaData& metaData)
{
	AntiAirFireControlSystem* newInstance = new AntiAirFireControlSystem(dxDevice, dxDeviceContext);
	newInstance->metaData = &metaData;
	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
	return newInstance;
}

Engine::Component* AntiAirFireControlSystem::Clone(void)
{
	return new AntiAirFireControlSystem(*this);
}

HRESULT AntiAirFireControlSystem::Start(void)
{
	bullet = SimpleBullet::Create(dxDevice, dxDeviceContext);
	if (bullet == nullptr)
		return E_FAIL;
	standardMissile = nullptr;
	uniqueMissile = nullptr;
	uniqueMissileFired = false;
	standardMissileFired = false;
	selectedWeapon = 0;

	return S_OK;
}

HRESULT AntiAirFireControlSystem::Awake(void)
{
	return S_OK;
}

void AntiAirFireControlSystem::Update(void)
{
	standardMissileFired = false;

	for (auto iterator = activeBullet.begin(); iterator != activeBullet.end(); )
	{
		if ((*iterator)->Destroyed())
		{
			Base::Destroy((*iterator));
			iterator = activeBullet.erase(iterator);
		}
		else
		{
			(*iterator)->Update();
			iterator++;
		}
	}

	GunControl();
}

void AntiAirFireControlSystem::LateUpdate(void)
{
	for (auto& bullet : activeBullet)
	{
		if (!bullet->Destroyed())
			bullet->LateUpdate();
	}

	if (targeted != nullptr && targeted->Destroy())
		targeted = nullptr;
}

void AntiAirFireControlSystem::FixedUpdate(void)
{
}

void AntiAirFireControlSystem::GunControl(void)
{
	selectedWeapon = 0;
	gunFired = false;

	coolDownGun -= DeltaTime();

	if (targeted == nullptr)
	{
		weaponRelease = false;
		return;
	}

	if (gunFire && coolDownGun <= 0.0f)
	{
		Transform& transform = *gameObject->transform();
		float4 quaternion;
		Vector3 direction = (targeted->transform()->Position() - transform.Position()).normalize().getDirection();

		DirectX::XMStoreFloat4(&quaternion, DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToDegrees(direction.x), DirectX::XMConvertToDegrees(direction.y), 0.0f));

		activeBullet.push_back(bullet->Shoot(gameObject, transform.Position() + transform.Forward() * 100.0f, quaternion));
		coolDownGun += gunFireTick;
		gunFired = true;
	}


	weaponRelease = false;
}
