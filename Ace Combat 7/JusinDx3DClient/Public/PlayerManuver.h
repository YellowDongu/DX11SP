#pragma once

class FlightMovement;

class PlayerManuver : public Engine::Component
{
public:
	PlayerManuver(void) = delete;
	PlayerManuver(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	PlayerManuver(const Component& other);
	virtual ~PlayerManuver(void) = default;
	virtual void Free(void);
public:
	static PlayerManuver* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	virtual Engine::Component* Clone(void);

	virtual void Update(void);
	virtual void LateUpdate(void);
	virtual void FixedUpdate(void);

	void GrabYoke(Vector3& object) { yoke = &object; }
	void GrabThrottle(float& object) { throttle = &object; }
	

private:
	FlightMovement* flightModule;

	Vector3 inputs;
	float* throttle;
	Vector3* yoke;
};