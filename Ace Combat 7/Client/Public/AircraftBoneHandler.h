#pragma once
#include "BoneHandler.h"

class Engine::Model;
class AircraftBoneHandler : public Engine::BoneHandler
{
protected:
	AircraftBoneHandler(void) = delete;
	AircraftBoneHandler(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	AircraftBoneHandler(const AircraftBoneHandler& other);
	virtual ~AircraftBoneHandler(void) = default;
	virtual void Free(void);
public:

	virtual void Update(void) override;
	virtual void FixedUpdate(void) override;
	virtual void LateUpdate(void) override;

	void Aileron(float inputValue);
	void Rudder(float inputValue);
	void Elevator(float inputValue);

	void ChangeAnimationFrame(FLOAT value, const std::string channelName);
	void LinkYoke(const Vector3& target) { yoke = &target; }

	void LinkModels(Engine::Model* _body, Engine::Model* _landingGear) { body = _body; landingGear = _landingGear; }
	void CreateBoneSets(std::map<std::string, Engine::BoneSet>& boneSets, Engine::Model* model, Engine::Animation* targetAnimation);
protected:
	const Vector3* yoke{ nullptr };
	bool landingGearApply;

	std::map<std::string, Engine::BoneSet> gearSets;

	// primary surface
	std::vector<Engine::BoneSet*> aileronLParts;
	std::vector<Engine::BoneSet*> aileronRParts;
	std::vector<Engine::BoneSet*> rudderParts;
	std::vector<Engine::BoneSet*> elevatorParts;

	Engine::Model* body;
	Engine::Model* landingGear;
};
