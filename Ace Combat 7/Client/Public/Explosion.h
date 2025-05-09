#pragma once
#include "ExplosionParticle.h"

class Explosion : public Engine::GameObject
{
public:
	Explosion(void) = delete;
	Explosion(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	Explosion(const Explosion& other);
	virtual ~Explosion(void) = default;
	virtual void Free(void) override;
	static Explosion* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	virtual Engine::GameObject* Clone(void) override;

	HRESULT Start(void);
	virtual HRESULT Awake(void) { return S_OK; }
	virtual void Update(void);
	virtual void LateUpdate(void);
	virtual void FixedUpdate(void);
	virtual void Render(void);

	void AddExplosion(Vector3 position, Vector3 scale = Vector3::one());

private:
	ExplosionParticle* particle{nullptr};

	std::vector<Engine::VertexMatrix> targetPoints;
	std::vector<Engine::AdditionalVertexInfomation> additionalInfomations;
	std::list<std::pair<Engine::VertexMatrix, Engine::AdditionalVertexInfomation>> ExplosionQueue;
};