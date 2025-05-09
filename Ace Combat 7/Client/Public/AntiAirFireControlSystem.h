#pragma once

#include "FireControlSystem.h"

class AntiAirFireControlSystem : public FireControlSystem
{
private:
	AntiAirFireControlSystem(void) = delete;
	AntiAirFireControlSystem(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	AntiAirFireControlSystem(const AntiAirFireControlSystem& other);
	virtual ~AntiAirFireControlSystem(void) = default;
	virtual void Free(void) override;
public:
	static AntiAirFireControlSystem* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, AircraftMetaData& metaData);
	virtual Engine::Component* Clone(void) override;

	HRESULT Start(void);
	virtual HRESULT Awake(void) override;
	virtual void Update(void) override;
	virtual void LateUpdate(void) override;
	virtual void FixedUpdate(void) override;

	void GunControl(void);
private:
}typedef AAFCS;