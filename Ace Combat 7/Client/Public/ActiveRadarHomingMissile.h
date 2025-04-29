#pragma once

#include "Missile.h"

namespace Engine
{
	class LineDrawer;
}
class ActiveRadarHomingMissile : public Missile
{
private:
	ActiveRadarHomingMissile(void) = delete;
	ActiveRadarHomingMissile(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	ActiveRadarHomingMissile(const ActiveRadarHomingMissile& other);
	virtual ~ActiveRadarHomingMissile(void) = default;
	virtual void Free(void) override;
public:
	static ActiveRadarHomingMissile* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	virtual Engine::GameObject* Clone(void) override;
	virtual Missile* Launch(Engine::GameObject* shooter, Vector3 LaunchOffsetPosition, Engine::GameObject* target) override;

	HRESULT Start(void);
	virtual void Update(void) override;
	virtual void LateUpdate(void) override;
	virtual void FixedUpdate(void) override;
	virtual void Render(void) override;

	void Chase(void);

	const float4& BaseRotation(void) { return baseRotation; }
	const float4& Rotation(void) { return rotation; }
private:
	class FireControlSystem* fcs = nullptr;

	Vector3 currentPosition = Vector3::one() * -1.0f;
	Vector3 priviousPosition = Vector3::one() * -1.0f;

	float4 baseRotation {};
	float4 rotation{};
	Vector3 predictedPosition = Vector3::one() * -1.0f;

	Engine::LineDrawer* debugDraw = nullptr;
	float timer = 0.0f;

}typedef ARHM;