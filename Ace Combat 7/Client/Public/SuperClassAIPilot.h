#pragma once
#include "AIPilot.h"

class SuperClassAIPilot : public AIPilot
{
private:
	SuperClassAIPilot(void) = delete;
	SuperClassAIPilot(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	SuperClassAIPilot(const SuperClassAIPilot& other);
	virtual ~SuperClassAIPilot(void) = default;
	virtual void Free(void) override;
public:
	static SuperClassAIPilot* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, ObjectInfomation& objectInfomation);
	virtual Engine::Component* Clone(void) override;

	virtual void Update(void) override;
	virtual void FixedUpdate(void) override;
	virtual void LateUpdate(void) override;

private:
	FLOAT targetVelocity{660.0f};
};
