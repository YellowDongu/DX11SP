#pragma once
#include "CloudEffect.h"

class Cloud : public Engine::GameObject
{
private:
	Cloud(void) = delete;
	Cloud(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	Cloud(const Cloud& other);
	virtual ~Cloud(void) = default;
	virtual void Free(void);
public:
	static Cloud* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, Vector3 position, Vector3 cloudSize = Vector3{100.0f, 100.0f, 100.0f});
	virtual Engine::GameObject* Clone(void) override;

	HRESULT Start(void);
	virtual HRESULT Awake(void) override;
	virtual void Update(void) override;
	virtual void LateUpdate(void) override;
	virtual void FixedUpdate(void) override;
	virtual void Render(void) override;


private:
	Vector3 cloudSize, centerPosition;
	std::vector<Engine::VertexMatrix> distributionMatrix;
	CloudEffect* particle{nullptr};
};