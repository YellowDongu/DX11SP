#pragma once

class AboveText final : public Engine::UIObject
{
private:
	AboveText(void) = delete;
	AboveText(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	AboveText(const AboveText& other) = delete;
	virtual ~AboveText(void) = default;
	virtual void Free(void) override;
public:
	static AboveText* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	virtual Engine::GameObject* Clone(void) { return nullptr; }

	virtual HRESULT Start(void) override;
	virtual HRESULT Awake(void) override;
	virtual void Update(void) override;
	virtual void LateUpdate(void) override;
	virtual void FixedUpdate(void) override;
	virtual void Render(void) override;

private:
	bool initial{ true }, incomeBattle{ false };
	FLOAT timer{ 2.5f }, secondTimer{ 5.0f }, thridTimer{ 5.0f }, forthTimer{ 5.0f };
	class ScreenFadeOut* fadeOutScreen{ nullptr };
	Engine::Layer* layer{ nullptr };
	Engine::Text* text{ nullptr };
	Vector2 position;
};
