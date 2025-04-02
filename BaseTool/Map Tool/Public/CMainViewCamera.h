#pragma once
#include "CGameObject.h"
#include "CToolCamera.h" 

class CMainViewCamera : public Engine::CGameObject
{
public:
	CMainViewCamera(void);
	CMainViewCamera(LPDIRECT3DDEVICE9& device);
	~CMainViewCamera(void);

	static CMainViewCamera* Create(LPDIRECT3DDEVICE9& device, HWND _hWnd);

	virtual	_int Update_GameObject(const _float& fTimeDelta);
	void Render_GameObject(void) override;
	void Render_UI(void);

	CTransform* Transform(void) { return transformComponent; }
	ToolCamera* Camera(void) { return cameraComponent; }
	void UIPreviewActive(bool value) { cameraComponent->UIPreviewActive(value); }
	void SetCameraMoveSpeed(float value) { cameraMoveSpeed = value; }
	void SetCameraRotateSpeed(float value) { cameraRotateSpeed = value; }
	bool UIPreviewActive(void) { return cameraComponent->UIPreviewActive(); }




private:
	float cameraMoveSpeed;
	float cameraRotateSpeed;


	ToolCamera* cameraComponent;
	CTransform* transformComponent;





};