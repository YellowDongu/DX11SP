#pragma once

class Missile : public Engine::GameObject
{
protected:
	Missile(void) = delete;
	Missile(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	Missile(const Missile& other);
	virtual ~Missile(void) = default;
public:
	//static Missile* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	//virtual Engine::GameObject* Clone(void) override;
	
	virtual HRESULT Start(void) = 0;
	virtual HRESULT Awake(void);
	virtual void Update(void) override;
	virtual void LateUpdate(void) override;
	virtual void FixedUpdate(void) override;
	virtual void Render(void) override;

	virtual Missile* Launch(Engine::GameObject* shooter, Vector3 LaunchOffsetPosition, Engine::GameObject* target) = 0;
	virtual void Rotation(const Vector3& currentDestination);
	void Detonate(void);
	bool Detonated(void) { return detonated; }
	Engine::GameObject* Target(void) { return target; }
	FLOAT MaximumLockDistance(void) { return maximumLockDistance; }
	FLOAT MaximumLockDirection(void) { return maximumLockDirection; }
	FLOAT CoolTimeSpeed(void) { return coolTimeSpeed; }
	UINT MaximumShot(void) { return maximumShot; }
	UINT MissileCount(void) { return missileCount; }
protected:
	bool detonated = false;
	Engine::StaticModel* model{nullptr};
	Engine::GameObject* target{nullptr};
	static class Explosion* explosion;

	FLOAT damage = 30.0f;
	FLOAT lifeTime = 0.0f;
	FLOAT maximumLifeTime = 15.0f;
	FLOAT maximumLockDistance = ConvertFeetToWorld(6000.0f) * 2.5f;
	FLOAT speed = ConvertFeetToWorld(4000.0f);
	FLOAT maximumLockDirection = std::cosf(DirectX::XMConvertToRadians(15.0f));
	FLOAT coolTimeSpeed = 0.1f;
	UINT maximumShot = 2;
	UINT missileCount = 100;
	FLOAT explosionScale = 5.0f;

	Vector2 rotationSpeed = Vector2::zero();
};
