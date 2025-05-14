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
	void UpdateTrail(MissileTrailParticle::TrailInfo& infomation, bool extinction);
	void UpdateTrail(MissileTrailParticle::TrailInfo& infomation);
	void GenerateInterpolatedPoints(const Vector3& pointA, const Vector3& pointB, FLOAT frontTime, FLOAT interval, std::vector<std::pair<Vector3, FLOAT>>& result);
private:
	MissileTrailParticle* particle{nullptr};
	FLOAT maxTime = 0.0f;
	FLOAT Timer = 0.0f, recordTime = 0.05f;
	std::vector<MissileTrailParticle::TrailInfo> trailInfoamtion;
	std::vector<MissileTrailParticle::TrailInfo> trails;
};