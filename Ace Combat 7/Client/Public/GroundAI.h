#pragma once

#include "AIPilot.h"

class GroundAI : public AIPilot
{
private:
	GroundAI(void) = delete;
	GroundAI(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	GroundAI(const GroundAI& other);
	virtual ~GroundAI(void) = default;
	virtual void Free(void);
public:
	static GroundAI* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, ObjectInfomation& objectInfomation);
	virtual Engine::Component* Clone(void) override;

	HRESULT Start(void);
	virtual HRESULT Awake(void) override;
	virtual void Update(void) override;
	virtual void LateUpdate(void) override;
	virtual void FixedUpdate(void) override;

	void SetFireDistance(FLOAT value) { fireDistance = value; }
private:
	FLOAT maxDistance;
	FLOAT fireDistance{ConvertFeetToWorld(2500.0f) * 2.5f};
	Engine::GameObject** target{nullptr};
};