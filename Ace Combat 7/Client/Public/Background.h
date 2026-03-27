#pragma once
#include "UIObject.h"
#include "RectanglePolygon.h"

class Background : public Engine::UIObject
{
private:
	Background(void) = delete;
	Background(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	Background(const Background& other);
	virtual ~Background(void) = default;
	virtual void Free(void) override;
public:
	static Background* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	virtual Engine::GameObject* Clone(void);

	virtual HRESULT Start(void);
	virtual HRESULT Awake(void);
	virtual void FixedUpdate(void);
	virtual void Update(void);
	virtual void LateUpdate(void);
	virtual void Render(void);

	bool IsSplashScreenEnd(void);
	bool SplashScreen(void) { return splashScreen; }
	void EndSplashScreen(void) { splashScreen = false; }
private:
	bool splashScreen{ true }, onlyBackGround{false};
	ID3D11ShaderResourceView* backGroundTexture{nullptr};
	ID3D11ShaderResourceView* MainMenuTexture{nullptr};
	Engine::RectanglePolygon* rectangle{nullptr};
	std::vector<ID3D11ShaderResourceView*> textures;
	class ScreenFadeOut* fadeoutScreen{nullptr};

	FLOAT timer = 3.0f;
	INT splashScreenCount{ -1 };
	const UINT stride = sizeof(UIVertex);
	std::string diffuseTexture = "diffuseTexture";
	std::string world = "worldMatrix";
	UIParts backGround;
};
