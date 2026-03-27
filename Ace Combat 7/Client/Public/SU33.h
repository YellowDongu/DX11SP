#pragma once

namespace Engine
{
	class Collider;
}

class SU33 : public Engine::GameObject
{
private:
	SU33(void) = delete;
	SU33(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	SU33(const SU33& other);
	virtual ~SU33(void) = default;
	virtual void Free(void) override;
public:
	static SU33* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	static SU33* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, ObjectInfomation& infomation);
	virtual Engine::GameObject* Clone(void) override;

	HRESULT Start(void);

	virtual HRESULT Awake(void) override;
	virtual void Update(void) override;
	virtual void LateUpdate(void) override;
	virtual void FixedUpdate(void) override;
	virtual void Render(void) override;
private:
	ObjectInfomation metaData;

	Engine::Model* model;
	Engine::Model* gearModel;
	class  FlightMovement* flightModule;
	class AircraftBoneHandler* boneHandler;
	class FireControlSystem* fcs;
	class AutoPilot* autoPilot;
	class AIPilot* pilot = nullptr;
	Engine::Collider* collider;
	Engine::GameObject* player = nullptr;

};