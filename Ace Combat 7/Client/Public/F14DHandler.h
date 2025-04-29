#pragma once
#include "AircraftBoneHandler.h"

class F14DHandler : public AircraftBoneHandler
{
private:
	F14DHandler(void) = delete;
	F14DHandler(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	F14DHandler(const F14DHandler& other);
	virtual ~F14DHandler(void) = default;
	virtual void Free(void);
public:
	static F14DHandler* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, Engine::Model* body, Engine::Model* gear);
	static F14DHandler* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	Engine::Component* Clone(void) override;

	HRESULT Start(void);
	virtual HRESULT Awake(void) override;
	virtual void Update(void) override;
	virtual void FixedUpdate(void) override;
	virtual void LateUpdate(void) override;

	void Taileron(float elevatorInputValue, float aileronInputValue);
	void Flap(float inputValue);
	void AirBrake(float inputValue);
	void AirIntake(float inputValue);
	void LandingGear(void);
	void VariableWing(void);

private:
	// secondary surface
	std::vector<Engine::BoneSet*> taileronLParts;
	std::vector<Engine::BoneSet*> taileronRParts;
	std::vector<Engine::BoneSet*> flapParts;
	std::vector<Engine::BoneSet*> airbreakParts;
	std::vector<Engine::BoneSet*> airIntakeParts;
	std::vector<Engine::BoneSet*> wingParts;

	std::vector<Engine::BoneSet*> landingGearParts;

	bool wingFold = false;
};
