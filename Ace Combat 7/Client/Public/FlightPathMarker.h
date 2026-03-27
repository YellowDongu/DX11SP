#pragma once

#include "UIObject.h"


class FlightPathMarker : public Engine::UIObject
{
private:
	FlightPathMarker(void) = delete;
	FlightPathMarker(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	FlightPathMarker(const FlightPathMarker&) = delete;
	virtual ~FlightPathMarker(void) = default;
	virtual void Free(void);
public:
	static FlightPathMarker* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, Engine::GameObject* player);

	HRESULT Start(void) override;
	HRESULT Awake(void) override;
	void FixedUpdate(void) override;
	void Update(void) override;
	void LateUpdate(void) override;
	void Render(void) override;

	void LinkInfomation(class FlightMovement* playerFlightModule);
private:
	UIParts pathMarker;
	UIParts centerMarker;
	const bool* warning{nullptr};
	const Vector3 *forward, *force;
	float4 color;
	Vector2 path;
	bool goingForward; // for test
};
