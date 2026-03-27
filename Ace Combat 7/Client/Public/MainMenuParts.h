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

	void MainMenuRender(void);
	void SelectMenuRender(void);
	void MainMenuUpdate(void);
	void SelectMenuUpdate(void);
	void TitleScreenUpdate(void);
	//bool IsInMainMenu(void) { return InMainMenu; }
	//bool IsInSelectMenu(void) { return InMainMenu; }
	void LinkNextSceneTrigger(bool& trigger) { nextSceneTrigger = &trigger; }
private:
	bool flag{ false }, secondFlag{false};
	bool InMainMenu{ false }, missionThumnail{false};
	bool InSelectMenu{ false }, inLoading{false};
	bool* nextSceneTrigger{nullptr};

	UINT menuSelect{ 0 };
	FLOAT timer{2.0f};
	class Background* background{nullptr};
	class ScreenFadeOut* fadeout{nullptr};
	class SelectPreview* preview{nullptr};
	Engine::Text* text{ nullptr }, * largeText{nullptr};
	FMOD::Channel* effectSoundChannel{nullptr};
	FMOD::Channel* bgmChannel{nullptr};
	FMOD::Sound* select{nullptr};
	FMOD::Sound* changeSelect{nullptr};
	FMOD::Sound* aircraftSelected{nullptr};
	FMOD::Sound* TurnToMainMenu{nullptr};
	FMOD::Sound* MainMenuBGM{nullptr};
	FMOD::Sound* SelectMenuBGM{nullptr};
	FMOD::Sound* MenuSelected{nullptr};
	FMOD::Sound* MenuChange{nullptr};
	FMOD::Sound* MenuCancel{nullptr};
	const D3D11_VIEWPORT* viewPort{nullptr};


	float4 white = float4{ 1.0f,1.0f,1.0f,1.0f };
	float4 mainColor = float4{ 170.0f / 255.0f, 211.0f / 255.0f, 1.0f, 1.0f };
	UIParts selectedHighlight, line;
	const UINT UIstride = sizeof(UIVertex);
	const std::string diffuseTexture = "diffuseTexture";
	const std::string world = "worldMatrix";
};
