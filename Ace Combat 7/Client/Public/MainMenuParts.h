#pragma once
#include "UIObject.h"

class MainMenuParts : public Engine::UIObject
{
private:
	MainMenuParts(void) = delete;
	MainMenuParts(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	MainMenuParts(const MainMenuParts& other) = delete;
	virtual ~MainMenuParts(void) = default;
	virtual void Free(void);
public:
	static MainMenuParts* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	virtual Engine::GameObject* Clone(void);

	virtual HRESULT Start(void);
	virtual HRESULT Awake(void);
	virtual void FixedUpdate(void);
	virtual void Update(void);
	virtual void LateUpdate(void);
	virtual void Render(void);

	void Main(void);
	bool IsInMainMenu(void) { return InMainMenu; }
private:
	Engine::Text* text{nullptr};
	class Background* background{nullptr};
	UINT menuSelect{ 0 };
	FMOD::Channel* effectSoundChannel{nullptr};
	FMOD::Channel* bgmChannel{nullptr};
	FMOD::Sound* select{ nullptr };
	FMOD::Sound* changeSelect{ nullptr };
	FMOD::Sound* TurnToMainMenu{ nullptr };
	FMOD::Sound* MainMenuBGM{ nullptr };
	FMOD::Sound* MenuSelected{nullptr};
	FMOD::Sound* MenuChange{nullptr};
	FMOD::Sound* MenuCancel{nullptr};

	bool tempBoolean{false};
	bool InMainMenu{false};
	
};
