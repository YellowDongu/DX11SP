#pragma once
#include "AircraftBoneHandler.h"

class SU33Handler : public AircraftBoneHandler
{
private:
	SU33Handler(void) = delete;
	SU33Handler(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	SU33Handler(const SU33Handler& other);
	virtual ~SU33Handler(void) = default;
	virtual void Free(void) override;
public:
	static SU33Handler* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, Engine::Model* body, Engine::Model* gear);
	static SU33Handler* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
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
	std::vector<Engine::BoneSet*> flapParts;
	std::vector<Engine::BoneSet*> airbreakParts;
	std::vector<Engine::BoneSet*> airIntakeParts;

	std::vector<Engine::BoneSet*> landingGearParts;
	std::vector<Engine::BoneSet*> wingParts;

};
