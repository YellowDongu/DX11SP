#pragma once


class PlayerPilot : public Engine::Component
{
public:
	PlayerPilot(void) = delete;
	PlayerPilot(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	PlayerPilot(const Component& other);
	virtual ~PlayerPilot(void) = default;
	virtual void Free(void);
public:
	static PlayerPilot* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, ObjectInfomation& objectInfomation);
	virtual Engine::Component* Clone(void);

	HRESULT Awake(void);
	virtual void Update(void);
	virtual void LateUpdate(void);
	virtual void FixedUpdate(void);

	const ObjectInfomation& LinkObjectInfomation(void) const { return objectInfomation; }
	void LinkCamera(class MainCamera* object) { camera = object; }
	//Test Area
	Engine::GameObject** testEnemyPointer;
	Engine::GameObject* testEnemy;
private:
	ObjectInfomation objectInfomation;
	Engine::Transform* transformComponent;
	class FlightMovement* flightModule;
	class FireControlSystem* fcs;
	class RadarMissileWarningReceiver* rmwr;

	Vector3 cameraTrdViewOffset;
	Vector3 cameraFstViewOffset;

	class MainCamera* camera;
	int cameraState = -1;


};