#pragma once
#include "Missile.h"

namespace Engine
{
	class LineDrawer;
}
class StandardMissile : public Missile
{
private:
	StandardMissile(void) = delete;
	StandardMissile(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	StandardMissile(const StandardMissile& other);
	virtual ~StandardMissile(void) = default;
	virtual void Free(void) override;
public:
	static StandardMissile* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	virtual Engine::GameObject* Clone(void) override;

	HRESULT Start(void);
	virtual void Update(void) override;
	virtual void LateUpdate(void) override;
	virtual void FixedUpdate(void) override;
	virtual void Render(void) override;

	void SetTarget(Engine::GameObject* object) { target = object; }
	virtual Missile* Launch(Engine::GameObject* shooter, Vector3 LaunchOffsetPosition, Engine::GameObject* target) override;
	void Chase(void);
	const float4& BaseRotation(void) { return baseRotation; }
	const float4& Rotation(void) { return rotation; }
private:
	float3 positionOffset;
	float4 baseRotation;

	float4 rotation;
	FLOAT priviousDistance = FLT_MAX;

	Engine::LineDrawer* debugDraw = nullptr;
	float timer = 0.0f;
};
