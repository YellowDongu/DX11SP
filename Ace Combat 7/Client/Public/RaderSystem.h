#pragma once

#include "RaderSystem.h"
class RadarMissileWarningReceiver;
class RaderSystem : public Engine::Component
{
protected:
	RaderSystem(void) = delete;
	RaderSystem(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	RaderSystem(const RaderSystem& other);
	virtual ~RaderSystem(void) = default;
	virtual void Free(void);
public:
	static RaderSystem* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	virtual Engine::Component* Clone(void);
	
	virtual HRESULT Awake(void) override;
	virtual void Update(void) override;
	virtual void LateUpdate(void) override;
	virtual void FixedUpdate(void) override;

	void SearchEnemy(void);
	HRESULT LayerSearch(void);

	void CollectLayer(void);
	void PlayerControl(void);

	void UnTargeting(void) { *currentEnemy = nullptr; *currentEmemyRMWR = nullptr; currentTargetIndex = -1; }
	void SetMaxTimer(FLOAT value) { maxTimer = value; }
	void SetSearchMaxDistance(FLOAT value) { maxDistance = value; }
	void SetDefendArea(void) { defend = true; }
protected:
	bool allyFaction = false, player{ false }, defend{false};

	FLOAT timer = 0.0f, maxTimer = 1.0f;
	FLOAT maxDistance = ConvertFeetToWorld(10000.0f) * 2.5f;
	INT currentTargetIndex = -1;

	std::vector<Engine::Layer*> allyLayer;
	std::vector<Engine::Layer*> enemyLayer;
	std::list<std::pair<FLOAT, Engine::GameObject*>> sortedObjects;

	Engine::GameObject** currentEnemy{nullptr}; // fcs에게 줄거
	RadarMissileWarningReceiver** currentEmemyRMWR{ nullptr };
};
