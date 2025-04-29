#pragma once

#include "UIObject.h"


class FireControlSystemHeadUpDisplay : public Engine::UIObject
{
private:
	FireControlSystemHeadUpDisplay(void) = delete;
	FireControlSystemHeadUpDisplay(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	virtual ~FireControlSystemHeadUpDisplay(void) = default;
	virtual void Free(void);
public:
	static FireControlSystemHeadUpDisplay* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, Engine::GameObject* player, Engine::GameObject* camera);

	// UIObject을(를) 통해 상속됨
	HRESULT Start(void) override;
	HRESULT Awake(void) override;
	void FixedUpdate(void) override;
	void Update(void) override;
	void LateUpdate(void) override;
	void Render(void) override;
private:
	UIParts standardLock;
	Vector2 markerScale;
	UIParts specialLock;
	Vector2 specialMarkerScale;

	Vector2 targetUIPosition;

	float4 defaultColor = {0.0f, 1.0f, 0.0f, 1.0f};
	float4 red = {1.0f, 0.0f, 0.0f, 1.0f};

	bool inScreen;
	float* lockStatus;
	Engine::GameObject** target;
	Engine::Camera* camera;

	const D3D11_VIEWPORT* deviceInfomation;

	std::string diffuseTexture = "diffuseTexture";
	std::string world = "worldMatrix";
	const UINT stride = sizeof(UIVertex);
	Matrix worldMatrix;

}typedef FCSHUD;
