#pragma once
#include "Component.h"

class FireControlSystem;

class RadarMissileWarningReceiver : public Engine::Component
{
private:
	RadarMissileWarningReceiver(void) = delete;
	RadarMissileWarningReceiver(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	RadarMissileWarningReceiver(const RadarMissileWarningReceiver& other);
	virtual ~RadarMissileWarningReceiver(void) = default;
	virtual void Free(void) override;
public:
	static RadarMissileWarningReceiver* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	virtual Component* Clone(void) override;

	virtual void Update(void);
	virtual void LateUpdate(void);
	virtual void FixedUpdate(void);

	void LockedMe(FireControlSystem* hunter) { hunters.push_back(hunter); }
	void FiredToMe(class Missile* missile) { chasedMissile.push_back(missile); }
	void HitMe(FLOAT damageValue) { currentHealth -= damageValue; }

	bool Warning(void) { return warning; }
	bool MissileWarning(void) { return missileWarning; }
	bool ClosedWarning(void) { return closedWarning; }
	FLOAT Health(void) { return currentHealth; }
	FLOAT MAXHealth(void) { return maxHealth; }

private:
	bool warning = false;
	bool missileWarning = false;
	bool closedWarning = false;

	std::list<FireControlSystem*> hunters;
	std::list<class Missile*> chasedMissile;

	float maxHealth = 100.0f;
	float currentHealth = 50.0f;

} typedef RMWR;
