#pragma once

class ScreenFadeOut : public Engine::UIObject
{
private:
	ScreenFadeOut(void) = delete;
	ScreenFadeOut(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	ScreenFadeOut(const ScreenFadeOut& other);
	virtual ~ScreenFadeOut(void) = default;
	virtual void Free(void) override;
public:
	static ScreenFadeOut* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	virtual Engine::GameObject* Clone(void);

	HRESULT Start(void);
	virtual HRESULT Awake(void) override;
	virtual void Update(void) override;
	virtual void LateUpdate(void) override;
	virtual void FixedUpdate(void) override;
	virtual void Render(void) override;

	void SetFadeOut(bool value) { fadeOut = value; }
	bool IsFadeOut(void) { return fadeOut; }
	FLOAT fadeOutStatus(void) { return fadeOutTimer; }
	void SetFadeOutStatus(FLOAT value) { fadeOutTimer = value; }
	void SetFadeSpeed(FLOAT value) { fadeSpeed = value; }
private:
	bool fadeOut{false};
	FLOAT fadeOutTimer{0.0f}, fadeSpeed = 1.0f;
	UIParts part;
	const D3D11_VIEWPORT* viewPortInfomation{nullptr};
};