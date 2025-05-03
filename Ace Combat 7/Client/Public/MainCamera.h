#pragma once
#include "GameObject.h"
#include "FlightMovement.h"

class MainCamera : public Engine::GameObject
{
private:
	MainCamera(void) = delete;
	MainCamera(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	MainCamera(const MainCamera& other);
	virtual ~MainCamera(void) = default;
	virtual void Free(void) override;
public:
	static MainCamera* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	virtual MainCamera* Clone(void);
	HRESULT Start(void);
	HRESULT Awake(void);

	virtual void Update(void);
	virtual void LateUpdate(void);
	virtual void FixedUpdate(void);
	virtual void Render(void);
	
	const Matrix& ViewProjectionMatrix(void) const { return camera->ViewProjectionMatrix(); }
	const Matrix& ProjectionMatrix(void) const { return camera->ProjectionMatrix(); }
	FLOAT FOV(void) { return camera->FOV(); }
	FLOAT AspectRaito(void) { return camera->AspectRaito(); }
	FLOAT NearZ(void) { return camera->NearZ(); }
	FLOAT FarZ(void) { return camera->FarZ(); }

	void FOV(FLOAT value) { camera->FOV(value); }
	void AspectRaito(FLOAT value) { camera->AspectRaito(value); }
	void NearZ(FLOAT value) { camera->NearZ(value); }
	void FarZ(FLOAT value) { camera->FarZ(value); }

private:
	Engine::Camera* camera;
};
