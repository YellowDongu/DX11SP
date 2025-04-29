#pragma once
#include "Component.h"

class AutoPilot : public Engine::Component
{
protected:
	AutoPilot(void) = delete;
	AutoPilot(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	AutoPilot(const AutoPilot& other);
	virtual ~AutoPilot(void) = default;
	virtual void Free(void);
public:
	static AutoPilot* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, class FlightMovement* component);
	static AutoPilot* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	virtual Engine::Component* Clone(void);

	virtual HRESULT Awake(void);
	virtual void Update(void);
	virtual void LateUpdate(void);
	virtual void FixedUpdate(void);

	bool CheckAltitude(void);
	void Leveling(void);
	void TurnToDestination(Vector3& direction);
	void TurnToDirection(Vector3& angle);
	void TurnToDirection(Vector3& angle, bool& done);
	virtual void Turn(Vector3& angleDifference);
	virtual float CalculateRollAngle(Vector3 forward, Vector3 up, Vector3 right, Vector3 targetDirection);
	void LinkYoke(Vector3& target) { yoke = &target; }
	void LinkInfomation(class FlightMovement* component);

	void SetDestination(Vector3 point) { destination = point; }
	void QueueWaypoint(Vector3 point) { wayPoints.push_back(point); }
	void SetMinimumAltitude(FLOAT value) { minimumAltitude = value; }
protected:
	Engine::Transform* transform{ nullptr };
	Vector3* yoke{ nullptr };
	float* throttle{nullptr};
	float throttleIdle{0.0f};
	
	Vector3 destination;
	std::list<Vector3> wayPoints;

	float minimumAltitude = ConvertFeetToWorld(2000.0f);
};
