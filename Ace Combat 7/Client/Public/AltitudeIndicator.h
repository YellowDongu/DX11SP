#pragma once
#include "UIObject.h"

class AltitudeIndicator : public Engine::UIObject
{
private:
	AltitudeIndicator(void) = delete;
	AltitudeIndicator(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	AltitudeIndicator(const AltitudeIndicator&) = delete;
	virtual ~AltitudeIndicator(void) = default;
	virtual void Free(void);
public:
	static AltitudeIndicator* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, Engine::GameObject* player);

	HRESULT Start(void) override;
	HRESULT Awake(void) override;
	void FixedUpdate(void) override;
	void Update(void) override;
	void LateUpdate(void) override;
	void Render(void) override;
private:
	UIParts AltMeter;
	Vector3* position;
	INT altitude;
	Engine::Text* text;
};
