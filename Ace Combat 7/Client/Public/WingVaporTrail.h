#pragma once

#include "Component.h"

class WingVaporTrail : public Engine::Component
{
private:
	WingVaporTrail(void) = default;
	WingVaporTrail(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	WingVaporTrail(const WingVaporTrail& other);
	virtual ~WingVaporTrail(void) = default;
	virtual void Free(void) override;
public:
	static WingVaporTrail* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	virtual Engine::Component* Clone(void) override;

	HRESULT Start(void);
	virtual HRESULT Awake(void) override;
	virtual void Update(void) override;
	virtual void LateUpdate(void) override;
	virtual void FixedUpdate(void) override;
	void Render(void);

	HRESULT SetVertex(void);
	HRESULT CreateBuffer(void);

private:
	Vector3 trailOffset;
	std::list<Vector3> trailPositions;

	ID3D11Buffer* vertexBuffer{nullptr};
	ID3D11Buffer* indexBuffer{nullptr};


} typedef WVTrail;
