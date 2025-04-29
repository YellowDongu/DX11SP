#pragma once
#include "Camera.h"

class ToolCamera : public Engine::GameObject
{
public:
	ToolCamera(void) = delete;
	ToolCamera(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	ToolCamera(const ToolCamera& other);
	virtual ~ToolCamera(void) = default;
	virtual void Free(void) override;
	static ToolCamera* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	virtual Engine::GameObject* Clone(void) override;

	HRESULT Start(void);
	virtual void Update(void);
	virtual void LateUpdate(void);
	virtual void FixedUpdate(void);
	virtual void Render(void);

private:
	Vector3 angle;
	Engine::Camera* camera;

};
