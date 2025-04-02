#pragma once

#include "UIObject.h"

class Compass : public Engine::UIObject
{
private:
	Compass(void) = delete;
	Compass(const Compass&) = delete;
	Compass(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	virtual ~Compass(void) = default;
	virtual void Free(void);
public:
	static Compass* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, Engine::GameObject* player);
	virtual Engine::GameObject* Clone(void) override { return nullptr; }
	
	virtual HRESULT Start(void) override;
	virtual HRESULT Awake(void) override;
	virtual void FixedUpdate(void) override;
	virtual void Update(void) override;
	virtual void LateUpdate(void) override;
	virtual void Render(void) override;

	FLOAT GetDirection(fxmVector forward);
private:
	UIParts compassLine;
	const Vector3* forward;
	UINT currentAngle;
	Engine::Text* text;
	wchar_t buffer[4];
};
