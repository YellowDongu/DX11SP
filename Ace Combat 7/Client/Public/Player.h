#pragma once

class MainCamera;
namespace Engine
{
	class Collider;
}
class Player : public Engine::GameObject
{
private:
	Player(void) = delete;
	Player(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	Player(const Player& other);
	virtual ~Player(void) = default;
	virtual void Free(void);
public:
	static Player* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	static Player* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, ObjectInfomation& metaData);
	virtual GameObject* Clone(void);

	HRESULT Start(void);
	virtual HRESULT Awake(void) override;
	virtual void Update(void) override;
	virtual void LateUpdate(void) override;
	virtual void FixedUpdate(void) override;
	virtual void Render(void) override;
	
	void LinkCamera(MainCamera* object);
	int& LinkCameraState(void) { return cameraState; }
	virtual void Collide(Engine::GameObject* target, Engine::Collider* targetCollision) override;

private:
	//std::wstring selectedModel;
	ObjectInfomation objectInfomation;
	Engine::Model* model;
	Engine::Model* gearModel;
	class AircraftBoneHandler* boneHandler;
	class FlightMovement* flightModule;
	class FireControlSystem* fcs;
	class AutoPilot* autoPilot = nullptr;
	Engine::Collider* collider;
	class PlayerPilot* playerPilot = nullptr;

	Vector3 cameraTrdViewOffset;
	Vector3 cameraFstViewOffset;

	MainCamera* camera;
	int cameraState = -1;
};
