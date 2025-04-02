#pragma once
#include "Component.h"

class FlightMovement : public Engine::Component
{
public:
	struct FlightSpec
	{
		Vector3 maneuverSpeed;
		float EnginePower, airbreakPower, mass;

		float liftCoefficient, dragCoefficient;
		float landingLiftCoefficient, landingDragCoefficient;
		float criticalSpeed; // max ground speed
		float stallSpeed; // stall speed(ground speed)
	};
private:
	FlightMovement(void) = delete;
	FlightMovement(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, Engine::Transform* transform, FlightSpec& flightSpec);
	FlightMovement(const FlightMovement& other);
	virtual ~FlightMovement(void) = default;
	virtual void Free(void);
public:
	static FlightMovement* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, Engine::Transform* transform, FlightSpec& flightSpec);
	Engine::Component* Clone(void) override;

	HRESULT Start(void);
	void Render(void);

	virtual void Update(void) override;
	virtual void LateUpdate(void) override;

	void CalculateMovement(void);
	void CalculatePrimarySurfaceForce(void);
	float IdlePower(float EnginePower, float stallSpeed);
	float IdlePower(float EnginePower, float mass, float liftCoefficient, float dragCoefficient);
	float IdleVelocity(float mass, float liftCoefficient);
	void ThrottleIdle(void) { throttle = idleThrottle; }

	void LinkTransform(Engine::Transform* component) { transform = component; }
	
	const Vector3* LinkFlightVector(void) const { return &force; }
	const Engine::Transform* LinkTransform(void) const { return transform; }
	const FLOAT& Velocity(void) const { return velocity; }
private:
	bool isOnGround;

	Engine::Transform* transform;
	Vector3 force, manuverForce, finalMenuverForce; // final Vector
	
	// flight spec
	//using
	Vector3 maneuverSpeed;

	//notusing
	float liftCoefficient, dragCoefficient;
	float landingLiftCoefficient, landingDragCoefficient;

	float idleThrottle, idleVelocity;

	// Calculate
	//using
	Vector3 gravity;
	Vector3 thrust;
	Vector3 Lift;
	Vector3 Drag;
	float velocity;

	float EnginePower, airbreakPower, mass;
	float criticalSpeed;
	float stallSpeed;
	float speedFactor;
	float thrustPower;


public:
	// Inputs -> interface
	bool airbreakActive;
	Vector3 yoke = Vector3();	// X,Y,Z => 0 ~ 1
	float throttle; // 0 ~ 1
	// Inputs
};
