#pragma once
#include "CubePolygon.h"

class SimpleBullet : public Engine::GameObject
{
private:
	SimpleBullet(void) = delete;
	SimpleBullet(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	SimpleBullet(const SimpleBullet& other);
	virtual ~SimpleBullet(void) = default;
	virtual void Free(void);
public:
	static SimpleBullet* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	virtual Engine::GameObject* Clone(void);

    HRESULT Start(void);
    virtual void Update(void) override;
    virtual void LateUpdate(void) override;
    virtual void FixedUpdate(void) override;
    virtual void Render(void) override;

	void CreateCustomVertex(std::vector<CubeVertex>& vertices, std::vector<UINT>& indices);
	SimpleBullet* Shoot(Engine::GameObject* shooter, Vector3 offsetPosition, float4 quaternion);
	virtual void Collide(Engine::GameObject* target, Engine::Collider* targetCollision) override;

	void SetSpeed(FLOAT value) { bulletSpeed = value; }
	void SetDamage(FLOAT value) { damage = value; }
	bool Destroyed(void) { return destroy; }
private:
	bool destroy{ false };
	FLOAT lifeTime{ 0.0f }, maxTime{ 2.0f }, damage{ 5.0f };
	FLOAT bulletSpeed{ ConvertFeetToWorld(10000.0f) };
	float4 quaternion;
	Engine::CubePolygon* model{nullptr};
	Engine::Collider* collider{nullptr};
	static Engine::Shader* shader;
};
