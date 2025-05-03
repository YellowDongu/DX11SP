#pragma once

#include "UIObject.h"

class IdentificationFriendorFoeHeadUpDisplay : public Engine::UIObject
{
private:
	IdentificationFriendorFoeHeadUpDisplay(void) = delete;
	IdentificationFriendorFoeHeadUpDisplay(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	IdentificationFriendorFoeHeadUpDisplay(const IdentificationFriendorFoeHeadUpDisplay&) = delete;
	virtual ~IdentificationFriendorFoeHeadUpDisplay(void) = default;
	virtual void Free(void) override;
public:
	static IdentificationFriendorFoeHeadUpDisplay* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, Engine::Scene* scene, Engine::GameObject* player, Engine::GameObject* camera);

	HRESULT Start(void) override;
	HRESULT Awake(void) override;
	void FixedUpdate(void) override;
	void Update(void) override;
	void LateUpdate(void) override;
	void Render(void) override;

	void RenderUI(Engine::Layer* layer, UIParts& parts);
	void RenderHUD(Engine::GameObject* gameObject, UIParts& parts, Vector2 markerScale, Vector2 scale, bool mainTarget = false);
	void AddLayers(Engine::Scene* scene);

	void SetUIColor(float4 color) { uiColor = color; }
	void SetUIColor(fxmVector color) { DirectX::XMStoreFloat4(&uiColor, color); }

private:
	Engine::Layer* MainTargetEnemy = nullptr;
	Engine::Layer* MainTargetEnemyGround = nullptr;
	Engine::Layer* Enemy = nullptr;
	Engine::Layer* GroundEnemy = nullptr;
	Engine::Layer* NonTargetEnemy = nullptr;
	Engine::Layer* NonTargetGroundEnemy = nullptr;
	Engine::Layer* MainTargetAlly = nullptr;
	Engine::Layer* MainTargetAllyGround = nullptr;
	Engine::Layer* Ally = nullptr;
	Engine::Layer* GroundAlly = nullptr;
	Engine::Layer* NonTargetAlly = nullptr;
	Engine::Layer* NonTargetGroundAlly = nullptr;
	Engine::Layer* UnKnown = nullptr;
	Engine::Layer* GroundUnKnown = nullptr;

	Engine::GameObject** targeted;
	Engine::GameObject* player;
	Engine::GameObject* cameraObject;
	class Engine::Camera* camera;
	const D3D11_VIEWPORT* deviceInfomation;
	Engine::Text* text;

	UIParts aircraftMarker;
	UIParts groundMarker;
	UIParts nonTargetMarker;
	// * 30.2f / 50.0f
	float maxDistance = 5000.0f;
	Vector2 markerScale;

	float4 uiColor = { 0.0f, 1.0f, 0.0f, 1.0f };
}typedef IFFHUD;
//typedef IdentificationFriendorFoeHeadUpDisplay, IFFHUD
