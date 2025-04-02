#pragma once
#include "GameObject.h"

class StandardMissile : public Engine::GameObject
{
private:
	StandardMissile(void) = delete;
	StandardMissile(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	StandardMissile(const StandardMissile& other);
	virtual ~StandardMissile(void) = default;
	virtual void Free(void);
public:
	static StandardMissile* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	virtual GameObject* Clone(void) override;

	HRESULT Start(void);
	virtual void Update(void) override;
	virtual void LateUpdate(void) override;
	virtual void FixedUpdate(void) override;
	virtual void Render(void) override;

	void SetTarget(Engine::GameObject* object) { target = object; }
	void Chase(void);
	void Detonate(void);

private:
	Engine::GameObject* target = nullptr;
	float lifeTime = 0.0f;
	float maximumLifeTime = 990.0f;
	float speed = 1.0f;
};
