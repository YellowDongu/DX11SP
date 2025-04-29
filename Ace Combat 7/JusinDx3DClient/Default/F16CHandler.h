#pragma once


#include "AircraftBoneHandler.h"
class F16CHandler : public AircraftBoneHandler
{
private:
	F16CHandler(void) = delete;
	F16CHandler(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	F16CHandler(const F16CHandler& other);
	virtual ~F16CHandler(void) = default;
	virtual void Free(void);
public:
	static F16CHandler* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, Engine::Model* F16C, Engine::Model* F16CGear);
	static F16CHandler* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	Engine::Component* Clone(void) override;

	HRESULT Start(void);
	virtual HRESULT Awake(void);
	virtual void Update(void) override;
	virtual void FixedUpdate(void) override;
	virtual void LateUpdate(void) override;

	void Flap(float inputValue);
	void AirBrake(float inputValue);
	void AirIntake(float inputValue);
	void LandingGear(void);

private:
	// secondary surface
	std::vector<Engine::BoneSet*> flapParts;
	std::vector<Engine::BoneSet*> airbreakParts;
	std::vector<Engine::BoneSet*> airIntakeParts;

	std::vector<Engine::BoneSet*> landingGearParts;
};
