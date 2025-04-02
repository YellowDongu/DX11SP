#pragma once
#include "UIObject.h"

class GunReticle : public Engine::UIObject
{
private:
	GunReticle(void) = delete;
	GunReticle(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	GunReticle(const GunReticle&) = delete;
	virtual ~GunReticle(void) = default;
	virtual void Free(void);
public:
	static GunReticle* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);

	HRESULT Start(void) override;
	HRESULT Awake(void) override;
	void FixedUpdate(void) override;
	void Update(void) override;
	void LateUpdate(void) override;
	void Render(void) override;
private:
	UIParts innerCircle, outerCircle;

};