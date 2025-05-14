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
	void PlayerControl(void);

	const ObjectInfomation& LinkObjectInfomation(void) const { return objectInfomation; }
	void LinkCamera(class MainCamera* object) { camera = object; }
	int CameraState(void) { return cameraState; }
	//Test Area
	Engine::GameObject** testEnemyPointer{nullptr};
	Engine::GameObject* testEnemy{nullptr};
private:
	ObjectInfomation objectInfomation;
	Engine::Transform* transformComponent{nullptr};
	class FlightMovement* flightModule{nullptr};
	class FireControlSystem* fcs{nullptr};
	class RadarMissileWarningReceiver* rmwr{nullptr};

	Vector3 cameraTrdViewOffset;
	Vector3 cameraFstViewOffset;

	class MainCamera* camera{nullptr};
	int cameraState = -1;

	float accelValue = 0.5f;
};