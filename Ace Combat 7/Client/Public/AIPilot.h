#pragma once
#include "Component.h"

class AIPilot : public Engine::Component
{
protected:
	struct AISpec
	{
		bool isAlly;
		struct AircraftMetaData* data;
	};


	AIPilot(void) = delete;
	AIPilot(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	AIPilot(const AIPilot& other);
	virtual ~AIPilot(void) = default;
	virtual void Free(void);
public:
	static AIPilot* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, ObjectInfomation& objectInfomation);
	Engine::Component* Clone(void) override;

	HRESULT Start(void);
	virtual HRESULT Awake(void) override;
	virtual void Update(void) override;
	virtual void FixedUpdate(void) override;
	virtual void LateUpdate(void) override;

	// Non Combat
	void NonCombat(void);
	void LeadFlight(void);
	void FormationFlight(void);

	// In Combat
	void Evade(void);
	void SearchEnemy(void);
	void ChaseEnemy(void);

	const ObjectInfomation& LinkObjectInfomation(void) const { return objectInfomation; }
	void setDestination(Vector3 position);
protected:
	ObjectInfomation objectInfomation;
	bool defend = false;
	Engine::GameObject* target{nullptr};
	FLOAT timer = 0.0f;
	UINT squadNumber = 0;
	Engine::GameObject* leader{nullptr};
	std::vector<Engine::GameObject*> follower;
	Vector3 storage = Vector3::one() * -1.0f;
	AISpec aiSpec;
	Vector3 wingManPositionLocal = { -1.0f, 0.0f, -1.0f };
	class FlightMovement* flightModule = nullptr;
	class FlightMovement* leaderFlightModule = nullptr;
	class AircraftBoneHandler* boneHandler = nullptr;
	class FireControlSystem* fcs = nullptr;
	class AutoPilot* autoPilot = nullptr;
	class RadarMissileWarningReceiver* rmwr = nullptr;

	std::vector<Engine::Layer*> enemyLayer;
	float minimumAltitude = ConvertFeetToWorld(2000.0f);
	Vector3 direction, towardEnemy;
	float enemyDistance = 0.0f;
};
