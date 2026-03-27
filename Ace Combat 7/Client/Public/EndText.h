#pragma once

class EndText : public Engine::UIObject
{
private:
	EndText(void) = delete;
	EndText(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	EndText(const EndText& other);
	virtual ~EndText(void) = default;
	virtual void Free(void) override;
public:
	static EndText* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	virtual Engine::GameObject* Clone(void) override { return nullptr; }

	virtual HRESULT Start(void) override;
	virtual HRESULT Awake(void) override;
	virtual void FixedUpdate(void) override;
	virtual void Update(void) override;
	virtual void LateUpdate(void) override;
	virtual void Render(void) override;
private:
	Engine::Text* text{nullptr};
};
