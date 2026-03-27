#pragma once

class SelectPreview : public Engine::GameObject
{
private:
	SelectPreview(void) = delete;
	SelectPreview(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	SelectPreview(const SelectPreview& other);
	virtual ~SelectPreview(void) = default;
	virtual void Free(void) override;
public:
	static SelectPreview* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	virtual Engine::GameObject* Clone(void) override;

	HRESULT Start(void);
	virtual HRESULT Awake(void) override;
	virtual void Update(void) override;
	virtual void LateUpdate(void) override;
	virtual void FixedUpdate(void) override;
	virtual void Render(void) override;

	void ChangeModel(UINT index);
	void RestartGameScene(void);
	UINT Selected(void) { return displayingModelIndex; }
private:
	UINT displayingModelIndex{0};
	FLOAT viewAngle{120.0f};
	Engine::StaticModel* hanger{ nullptr };
	std::map<std::wstring, std::pair<Engine::StaticModel*, Engine::StaticModel*>> models; // main, gear
	std::pair<Engine::StaticModel*, Engine::StaticModel*>* currentDisplayingModel{nullptr};
	class MainCamera* mainCamera{nullptr};
};
