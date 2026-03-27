#pragma once

#include "UIObject.h"

class SpeedIndicator : public Engine::UIObject
{
private:
	SpeedIndicator(void) = delete;
	SpeedIndicator(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	SpeedIndicator(const SpeedIndicator&) = delete;
	virtual ~SpeedIndicator(void) = default;
	virtual void Free(void);
public:
	static SpeedIndicator* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, float& velocity);
	static SpeedIndicator* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, Engine::GameObject* player);

	HRESULT Start(void) override;
	HRESULT Awake(void) override;
	void FixedUpdate(void) override;
	void Update(void) override;
	void LateUpdate(void) override;
	void Render(void) override;
private:
	Engine::Text* text;
	UIParts speedMeter;
	const float* velocity;
	const bool* warning;
	float4 color;
	Vector2 scale;
};
