#include "pch.h"
#include "FireControlSystem.h"
#include "StandardMissile.h"
#include "RMWR.h"
#include "SimpleBullet.h"


FireControlSystem::FireControlSystem(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::Component(dxDevice, dxDeviceContext), gunFired(false), standardMissileFired(false), uniqueMissileFired(false), lock(0.0f)
{
}

FireControlSystem::FireControlSystem(const FireControlSystem& other) : Engine::Component(other), gunFired(false), standardMissileFired(false), uniqueMissileFired(false), index(other.index), metaData(other.metaData), targeted(nullptr), targetedRMWR(nullptr), forward(other.forward), lock(other.lock), directionMax(other.directionMax), lockSpeed(other.lockSpeed), selectedWeapon(other.selectedWeapon), standardMissileCount(other.standardMissileCount), uniqueMissileCount(), coolDownLStandardMissile(), coolDownRStandardMissile(), coolDownLStandardMissileSpeed(), coolDownRStandardMissileSpeed(), uniqueMissileCoolTime()
{
	if(other.standardMissile != nullptr)
		standardMissile = static_cast<StandardMissile*>(other.standardMissile->Clone());

	if (other.uniqueMissile != nullptr)
		uniqueMissile = static_cast<Missile*>(other.uniqueMissile->Clone());

	if (other.bullet != nullptr)
		bullet = static_cast<SimpleBullet*>(other.bullet->Clone());

}

void FireControlSystem::Free(void)
{
	for (auto simplebullet : activeBullet)
	{
		Base::DestroyInstance(simplebullet);
	}
	activeBullet.clear();
	Base::DestroyInstance(bullet);
	for (auto missile : activeMissile)
	{
		Base::DestroyInstance(missile);
	}
	activeMissile.clear();
	Base::DestroyInstance(standardMissile);
	Base::DestroyInstance(uniqueMissile);

}

FireControlSystem* FireControlSystem::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, ObjectInfomation& infomation)
{
	FCS* newInstance = new FCS(dxDevice, dxDeviceContext);
	newInstance->metaData = infomation;
	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}

	return newInstance;
}

Engine::Component* FireControlSystem::Clone(void)
{
	return new FireControlSystem(*this);
}

HRESULT FireControlSystem::Start(void)
{
	bullet = SimpleBullet::Create(dxDevice, dxDeviceContext);
	if (bullet == nullptr)
		return E_FAIL;

	return S_OK;
}

HRESULT FireControlSystem::Awake(void)
{
	return S_OK;
}

void FireControlSystem::Update(void)
{
	standardMissileFired = false;
	uniqueMissileFired = false;

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

	WeaponControl();

	if (targeted != nullptr && targeted->Destroy())
		targeted = nullptr;
}

void FireControlSystem::LateUpdate(void)
{
	for (auto& missile : activeMissile)
	{
		if(!missile->Detonated())
			missile->LateUpdate();
	}
	for (auto& bullet : activeBullet)
	{
		if (!bullet->Destroyed())
			bullet->LateUpdate();
	}

	if (targeted != nullptr && targeted->Destroy())
		targeted = nullptr;

}

void FireControlSystem::FixedUpdate(void)
{
}

void FireControlSystem::WeaponControl(void)
{
	gunFired = false;

	switch (selectedWeapon)
	{
	case -1: //FCS Locked
		break;
	case 0: //stm
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
			else if (coolDownRStandardMissile <= 0.0f)
			{
				activeMissile.push_back(standardMissile->Launch(gameObject, Vector3::zero(), targeted));
				coolDownRStandardMissile = 1.0f;
				standardMissileCount--;
				if (targetedRMWR != nullptr)
					targetedRMWR->FiredToMe(activeMissile.back());
				standardMissileFired = true;
			}
			weaponRelease = false;
		}
		if (gunFire && coolDownGun <= 0.0f)
		{
			Transform& transform = *gameObject->transform();
			activeBullet.push_back(bullet->Shoot(gameObject, transform.Position() + transform.Forward() * 5.0f, transform.Quaternion()));
			coolDownGun += gunFireTick;
			gunFired = true;
		}
		break;
	case 1: //uniqueWeapon
		if (!lock && weaponRelease)
		{
			if (uniqueMissileCount == 0)
			{
				weaponRelease = false;
				break;
			}

			for (auto& coolTime : uniqueMissileCoolTime)
			{
				if (coolTime.first > 0.0f)
					continue;

				Missile* missile = uniqueMissile->Launch(gameObject, Vector3::zero(), targeted);
				if (missile != nullptr)
					activeMissile.push_back(missile);

				if (targetedRMWR != nullptr)
					targetedRMWR->FiredToMe(missile);
				uniqueMissileFired = true;
				coolTime.first = 1.0f;
				uniqueMissileCount--;
				break;
			}
			if (gunFire && coolDownGun <= 0.0f)
			{
				Transform& transform = *gameObject->transform();
				activeBullet.push_back(bullet->Shoot(gameObject, transform.Position() + transform.Forward() * 5.0f, transform.Quaternion()));
				coolDownGun += gunFireTick;
				gunFired = true;
			}
			weaponRelease = false;
		}
		break;
	default:
		selectedWeapon = 0;
		break;
	}
	weaponRelease = false;


	if (coolDownGun <= 0.0f)
		coolDownGun = 0.0f;
	else
		coolDownGun -= DeltaTime();
	coolDownLStandardMissile -= DeltaTime() * coolDownLStandardMissileSpeed;
	coolDownRStandardMissile -= DeltaTime() * coolDownRStandardMissileSpeed;
	for (auto& coolTime : uniqueMissileCoolTime)
	{
		coolTime.first -= DeltaTime() * coolTime.second;
	}
}

void FireControlSystem::ChangeWeapon(void)
{
	if (selectedWeapon == -1)
		return;
	selectedWeapon++;
	switch (selectedWeapon)
	{
	case 1:
		if (uniqueMissile == nullptr)
			selectedWeapon = 0;
		break;
	case 2:
		selectedWeapon = 0;
		break;
	default:
		break;
	}
	weaponRelease = false;
	lock = 1.0f;
}

void FireControlSystem::SetSingleTarget(Engine::GameObject* target)
{
	targeted = target;
	targetedRMWR = static_cast<RadarMissileWarningReceiver*>(targeted->GetComponent(L"RMWR"));
	if(targetedRMWR != nullptr)
		targetedRMWR->LockedMe(this);
}

void FireControlSystem::Lock(void)
{
	if (targeted == nullptr)
	{lock = 1.0f; return;}

	Vector3 direction = targeted->transform()->Position() - gameObject->transform()->Position();
	FLOAT distance = direction.magnitude();
	direction = direction.normalize();
	float directionAmount = DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&gameObject->transform()->Forward()), DirectX::XMLoadFloat3(&direction)));

	switch (selectedWeapon)
	{
	case -1: //FCS Locked
		return;
	case 0: //stm
	{
		if (directionAmount < standardMissile->MaximumLockDirection() || distance > standardMissile->MaximumLockDistance())
		{
			lock += 0.0001f/*deltatime*/ * lockSpeed;
			if (lock > 1.0f) lock = 1.0f;
			break;
		}

		lock -= 0.0001f/*deltatime*/ * lockSpeed;
		if (lock < 0.0f) lock = 0.0f;
	}
		break;
	case 1: //uniqueWeapon
		if (directionAmount < uniqueMissile->MaximumLockDirection() || distance > uniqueMissile->MaximumLockDistance())
		{
			lock += 0.0001f/*deltatime*/ * lockSpeed;
			if (lock > 1.0f) lock = 1.0f;
			break;
		}

		lock -= 0.0001f/*deltatime*/ * lockSpeed;
		if (lock < 0.0f) lock = 0.0f;
		break;
	default:
		lock += 0.001f;
		break;
	}

	
}

void FireControlSystem::SetUniqueMissile(Missile* missile)
{
	uniqueMissile = missile;
	uniqueMissileCoolTime.clear();
	if (missile == nullptr)
		return;
	
	for (UINT i = 0; i < uniqueMissile->MaximumShot(); i++)
	{
		uniqueMissileCoolTime.push_back({ 0.0f, uniqueMissile->CoolTimeSpeed() });
	}
	uniqueMissileCount = uniqueMissile->MissileCount();


}