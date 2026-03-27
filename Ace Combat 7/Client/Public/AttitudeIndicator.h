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
	const bool* warning{nullptr};
	float4 color;

	std::map<std::wstring, UIParts> parts;

	FLOAT indicatorDistance = 30.0f;
	FLOAT indicatorWidth = 0.0f;

	std::wstring PitchLevelName = L"PitchLevelName";
	std::wstring PitchNegativeName = L"PitchNegativeName";
	std::wstring PitchNegativeHalfName = L"PitchNegativeHalfName";
	std::wstring PitchPositiveName = L"PitchPositiveName";
	std::wstring PitchPositiveHalfName = L"PitchPositiveHalfName";
	std::wstring PitchSideName = L"PitchSideName";


	UIParts PitchLevel;
	UIParts PitchNegative;
	UIParts PitchNegativeHalf;
	UIParts PitchPositive;
	UIParts PitchPositiveHalf;
	UIParts PitchSide;
};

