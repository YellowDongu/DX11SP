#pragma once
#include "MissileTrailParticle.h"

class MissileTrail : public Engine::GameObject
{
private:
	MissileTrail(void) = delete;
	MissileTrail(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	MissileTrail(const MissileTrail& other);
	virtual ~MissileTrail(void) = default;
	virtual void Free(void);
public:
	static MissileTrail* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	virtual Engine::GameObject* Clone(void) override;

	HRESULT Start(void);
	virtual HRESULT Awake(void) override;
	virtual void Update(void) override;
	virtual void LateUpdate(void) override;
	virtual void FixedUpdate(void) override;
	virtual void Render(void) override;

	HRESULT AddTrail(Engine::GameObject* object);
	void UpdateTrail(MissileTrailParticle::TrailInfo& infomation, bool extinction = false);
private:
	MissileTrailParticle* particle{nullptr};
	FLOAT maxTime = 0.0f;
	std::vector<MissileTrailParticle::TrailInfo> trailInfoamtion;
};