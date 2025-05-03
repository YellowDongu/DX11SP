#pragma once
#include "Shader.h"

class TerrainCollision : public Engine::Component
{
private:
	TerrainCollision(void) = default;
	TerrainCollision(ID3D11Device* device, ID3D11DeviceContext* context);
	TerrainCollision(const TerrainCollision& other);
	virtual ~TerrainCollision(void) = default;
	virtual void Free(void) override;
public:
	static TerrainCollision* Create(ID3D11Device* device, ID3D11DeviceContext* context);
	virtual Engine::Component* Clone(void) override;

	HRESULT Start(void);
	virtual HRESULT Awake(void) override;
	virtual void LateUpdate(void) override;
	virtual void FixedUpdate(void) override;

private:
	Engine::NaviNode* currentNode{nullptr};
	Engine::NavMesh* navMesh{nullptr};
	INT nodeIndex{-1};

	Vector3 frontLocalPosition{Vector3::zero()};
};
