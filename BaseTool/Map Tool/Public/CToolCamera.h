#pragma once
#include "CComponent.h"
#include "CTransform.h"

class ToolCamera : public Engine::CComponent
{
public:
	ToolCamera(void);
	~ToolCamera(void);

	virtual _int Update_Component(const _float& fTimeDelta);
	virtual void LateUpdate_Component(void) {}
	const vec3& Forward(void) const { return directionZ; }

	void Set_Camera(void);
	void SetUICamera(void);
	void Get_Window_Infomation(void);

	bool UIPreviewActive(void) { return UIActive; }
	void UIPreviewActive(bool value) { UIActive = value; }

	vec3 CameraUp(void) { return directionY; }


	vec3 DirectionX(void) { return directionX; }
	vec3 DirectionY(void) { return directionY; }
	vec3 DirectionZ(void) { return directionZ; }

	static ToolCamera* Create(HWND _hWnd, LPDIRECT3DDEVICE9& device, CTransform* _transformComponent = nullptr);
private:
	Engine::CComponent* Clone(void) override;
	vec3 direction(vec3 angle)
	{
		float pitchRadian = D3DXToRadian(angle.x);
		float yawRadian = D3DXToRadian(angle.y);
		float rollRadian = D3DXToRadian(angle.z);

		float cosPitch = cosf(pitchRadian);
		vec3 direction = vec3{ cosf(yawRadian) * cosPitch, sinf(pitchRadian), sinf(yawRadian) * cosPitch };
		D3DXVec3Normalize(&direction, &direction);
		return direction;
	}
	vec3 cross(vec3 from, vec3 to)
	{
		vec3 result;
		D3DXVec3Cross(&result, &from, &to);
		return result;
	}

	bool UIActive;

	CTransform* transformComponent;

	LPDIRECT3DDEVICE9 device;
	HWND hWnd;

	float fov;
	float aspectRatio;
	float nearSightRange, maximumSightRange;

	vec2 windowSize;
	vec3 up, at;
	vec3 directionX, directionY, directionZ;
	vec3 currentPosition;
	matrix viewMatrix, projectionMatrix, orthoMatrix;
};
