#pragma once

#include "Component.h"

class Missile;
class RadarMissileWarningReceiver;
class FireControlSystem : public Engine::Component
{
protected:
	FireControlSystem(void) = delete;
	FireControlSystem(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	FireControlSystem(const FireControlSystem& other);
	virtual ~FireControlSystem(void) = default;
	virtual void Free(void) override;
public:
	static FireControlSystem* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, ObjectInfomation& metaData);
	Engine::Component* Clone(void) override;

	HRESULT Start(void);
	virtual HRESULT Awake(void) override;
	virtual void Update(void) override;
	virtual void LateUpdate(void) override;
	virtual void FixedUpdate(void) override;

	void WeaponControl(void);
	void ChangeWeapon(void);
	void Lock(void);

	void UnLockFCS(void) { selectedWeapon = 0; }
	void LockFCS(void) { selectedWeapon = -1; }
	void SetSingleTarget(Engine::GameObject* target);
	void SetGunRPM(FLOAT value) { gunRPM = value; gunFireTick = 1.0f / (gunRPM / 60.0f); }
	void SetStandardMissile(class StandardMissile* missile) { standardMissile = missile; }
	void SetUniqueMissile(Missile* missile);

	bool Locked(void) { return !static_cast<bool>(lock); }
	float LockStatus(void) { return lock; }
	Engine::GameObject*& LinkTarget(void) { return targeted; }
	RadarMissileWarningReceiver*& LinkTargetRMWR(void) { return targetedRMWR; }
	float& LinkLockStatus(void) { return lock; }
	Missile* GetLastMissile(void) { if (activeMissile.empty()) return nullptr; return activeMissile.back(); }
	const std::list<Missile*>& GetMissiles(void) const { return activeMissile; }
	void SetForceTargetChange(Engine::GameObject* target) { SetSingleTarget(target); }
	INT WeaponSelectedStatus(void) { return selectedWeapon; }
	bool StandardMissileFired(void) { return standardMissileFired; }
	bool UniqueMissileFired(void) { return uniqueMissileFired; }
	bool GunFired(void) { return gunFired; }
	const FLOAT& CoolDownLStandardMissile(void) const { return coolDownLStandardMissile; }
	const FLOAT& CoolDownRStandardMissile(void) const { return coolDownRStandardMissile; }
	const std::vector<std::pair<FLOAT, FLOAT>>& UniqueMissileCoolTime(void) const { return uniqueMissileCoolTime; }
	const INT& StandardMissileCount(void) const { return standardMissileCount; }
	const INT& UniqueMissileCount(void) const { return uniqueMissileCount; }
	const INT& GunCount(void) const { return gunCount; }
	const ObjectInfomation& LinkMetaData(void) { return metaData; }
	// interface
	bool weaponRelease = false;
	bool gunFire = false;
protected:
	bool standardMissileFired, uniqueMissileFired, gunFired;

	ObjectInfomation metaData;
	Engine::GameObject* targeted{ nullptr };
	RadarMissileWarningReceiver* targetedRMWR{ nullptr };
	//std::vector<Engine::GameObject*, RadarMissileWarningReceiver*> multiTargeted;
	UINT index{ 0 };

	Vector3 forward;
	FLOAT lock;
	FLOAT gunRPM = 750.0f, gunFireTick = 1 / (750.0f / 60.0f);
	FLOAT directionMax = std::cosf(DirectX::XMConvertToRadians(15.0f));
	FLOAT lockSpeed = 75.0f;
	INT selectedWeapon = 0;
	INT standardMissileCount = 100, uniqueMissileCount = 50, gunCount = 1000;
	FLOAT coolDownGun = 0.0f;
	FLOAT coolDownLStandardMissile = 0.0f, coolDownRStandardMissile = 0.0f;
	FLOAT coolDownLStandardMissileSpeed = 0.1f, coolDownRStandardMissileSpeed = 0.1f;
	std::vector<std::pair<FLOAT, FLOAT>> uniqueMissileCoolTime;

	std::vector<Engine::Layer*> searchLayer;
	std::map<std::wstring, Engine::Layer*> layers;
	std::vector<Engine::GameObject*> sorted;

	Missile* uniqueMissile{ nullptr };
	class StandardMissile* standardMissile{ nullptr };
	class SimpleBullet* bullet{ nullptr };
	std::list<Missile*> activeMissile;
	std::list<class SimpleBullet*> activeBullet;

	std::wstring mainTargetEnemy = L"MainTargetEnemy";
	std::wstring mainTargetEnemyGround = L"MainTargetEnemyGround";
	std::wstring enemy = L"Enemy";
	std::wstring groundEnemy = L"GroundEnemy";
	std::wstring unKnown = L"UnKnown";
	std::wstring groundUnKnown = L"GroundUnKnown";
} typedef FCS;