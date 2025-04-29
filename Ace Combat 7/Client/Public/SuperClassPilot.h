#pragma once

#include "AutoPilot.h"

class SuperClassPilot : public AutoPilot
{
private:
	SuperClassPilot(void) = delete;
	SuperClassPilot(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	SuperClassPilot(const SuperClassPilot& other);
	virtual ~SuperClassPilot(void) = default;
	virtual void Free(void) override;
public:
	static SuperClassPilot* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	virtual Engine::Component* Clone(void) override;

	//virtual void TurnToDestination(Vector3& direction) override;
	//virtual void TurnToDirection(Vector3& angle) override;
	//virtual void TurnToDirection(Vector3& angle, bool& done) override;
	virtual void Turn(Vector3& angleDifference)override;
private:

};
