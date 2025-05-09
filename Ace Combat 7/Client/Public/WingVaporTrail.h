#pragma once

#include "WingTrailParticle.h"

class WingVaporTrail : public Engine::GameObject
{
	struct AircraftInfo
	{
		Engine::GameObject* gameObjectPointer;
		Vector3 leftOffset;
		Vector3 rightOffset;
		FLOAT trailWidthSize;
	
	};
private:
	WingVaporTrail(void) = default;
	WingVaporTrail(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	WingVaporTrail(const WingVaporTrail& other);
	virtual ~WingVaporTrail(void) = default;
	virtual void Free(void) override;
public:
	static WingVaporTrail* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	virtual Engine::GameObject* Clone(void) override;

	HRESULT Start(void);
	virtual HRESULT Awake(void) override;
	virtual void Update(void) override;
	virtual void LateUpdate(void) override;
	virtual void FixedUpdate(void) override;
	void Render(void);

	void UpdatePoints(std::list<Matrix>& matrixQueue, std::vector<Engine::VertexMatrix>& points, Vector3& offset, FLOAT& trailWidth);
	void UpdatePoints(std::list<Matrix>& matrixQueue, std::vector<Engine::VertexMatrix>& leftpoints, Vector3& leftOffset, std::vector<Engine::VertexMatrix>& rightPoints, Vector3& rightOffset, FLOAT& trailWidth);

	void EnlistGameObject(Engine::GameObject* objectPointer);
private:
	Vector3 trailOffset;
	std::vector<std::pair<AircraftInfo, std::list<Matrix>>> trailPositions;
	WingTrailParticle* particle{nullptr};

	std::vector<std::pair<std::vector<Engine::VertexMatrix>, std::vector<Engine::VertexMatrix>>> points;

	UINT MaxListSize = 100;

	FLOAT Timer = 0.0f;
	FLOAT RecordTime = 0.025f;

} typedef WVTrail;
