#pragma once

class TU160 : public Engine::GameObject
{
private:
	TU160(void) = delete;
	TU160(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	TU160(const TU160& other);
	virtual ~TU160(void) = default;
	virtual void Free(void) override;
public:
	static TU160* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	static TU160* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, ObjectInfomation& metaData);
	virtual Engine::GameObject* Clone(void) override;

	HRESULT Start(void);
	virtual HRESULT Awake(void);
	virtual void Update(void);
	virtual void LateUpdate(void);
	virtual void FixedUpdate(void);
	virtual void Render(void);
private:
	ObjectInfomation metaData;

	Engine::Model* model;
	Engine::Model* gearModel;
	class FlightMovement* flightModule;
	class AircraftBoneHandler* boneHandler;
	class FireControlSystem* fcs;
	class AutoPilot* autoPilot;
	Engine::Collider* collider;
	Engine::GameObject* player = nullptr;

};