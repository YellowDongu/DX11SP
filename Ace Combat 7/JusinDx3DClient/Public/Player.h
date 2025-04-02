#pragma once
#include "AircraftMetaData.h"
#include "AircraftBoneHandler.h"

class MainCamera;
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
	virtual GameObject* Clone(void);

	HRESULT Start(void);
	HRESULT Awake(void);
	virtual void Update(void);
	virtual void LateUpdate(void);
	virtual void FixedUpdate(void);
	virtual void Render(void);
	
	void LinkCamera(MainCamera* object) { camera = object; }

private:
	//std::wstring selectedModel;
	AircraftMetaData metaData;
	Engine::Model* model;
	Engine::Model* gearModel;
	AircraftBoneHandler* boneHandler;
	FlightMovement* flightModule;

	Vector3 cameraTrdViewOffset = { 0.0f ,0.4f, -1.5f };
	Vector3 cameraFstViewOffset = { 0.0f,0.128f, 0.88f};

	MainCamera* camera;
	int cameraState = -1;
};
