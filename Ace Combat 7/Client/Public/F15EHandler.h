#pragma once

#include "AircraftBoneHandler.h"
class F15EHandler : public AircraftBoneHandler
{
private:
	F15EHandler(void) = delete;
	F15EHandler(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	F15EHandler(const F15EHandler& other);
	virtual ~F15EHandler(void) = default;
	virtual void Free(void);
public:
	static F15EHandler* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, Engine::Model* F15E, Engine::Model* F15ELandingGear = nullptr);
	static F15EHandler* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	Engine::Component* Clone(void) override;

	HRESULT Start(void);
	virtual HRESULT Awake(void) override;
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
