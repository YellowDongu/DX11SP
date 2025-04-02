#pragma once
#include "UIObject.h"

class AttitudeIndicator : public Engine::UIObject
{
private:
	AttitudeIndicator(void) = delete;
	AttitudeIndicator(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	virtual ~AttitudeIndicator(void) = default;
	virtual void Free(void) override;
public:
	static AttitudeIndicator* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, Engine::Transform* transform);

	HRESULT Start(void) override;
	HRESULT Awake(void) override;
	void FixedUpdate(void) override;
	void Update(void) override;
	void LateUpdate(void) override;
	void Render(void) override;

private:
	Engine::Transform* transform;
	Vector3* Angle;


	std::map<std::wstring, UIParts> parts;

	FLOAT indicatorDistance = 30.0f;
	FLOAT indicatorWidth = 0.0f;

	std::wstring PitchLevel = L"PitchLevel";
	std::wstring PitchNegative = L"PitchNegative";
	std::wstring PitchNegativeHalf = L"PitchNegativeHalf";
	std::wstring PitchPositive = L"PitchPositive";
	std::wstring PitchPositiveHalf = L"PitchPositiveHalf";
	std::wstring PitchSide = L"PitchSide";
};

