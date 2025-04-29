#pragma once
#include "Component.h"

class AutoPilot : public Engine::Component
{
private:
	AutoPilot(void) = delete;
	AutoPilot(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	AutoPilot(const AutoPilot& other);
	virtual ~AutoPilot(void) = default;
	virtual void Free(void);
public:
	static AutoPilot* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, class FlightMovement* component);
	virtual Engine::Component* Clone(void);

	virtual void Update(void);
	virtual void LateUpdate(void);
	virtual void FixedUpdate(void);

	float CalculateRollAngle(Vector3 forward, Vector3 up, Vector3 right, Vector3 targetDirection);
	void LinkYoke(Vector3& target) { yoke = &target; }
private:
	Engine::Transform* transform{nullptr};
	Vector3* yoke{nullptr};
	Vector3 destination;
	const Vector3* menuverForce;
};
