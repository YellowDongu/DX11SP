#pragma once

#include "FireControlSystem.h"

class SurfaceToAirMissileFireControlSystem : public FireControlSystem
{
private:
	SurfaceToAirMissileFireControlSystem(void) = delete;
	SurfaceToAirMissileFireControlSystem(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	SurfaceToAirMissileFireControlSystem(const SurfaceToAirMissileFireControlSystem& other);
	virtual ~SurfaceToAirMissileFireControlSystem(void) = default;
	virtual void Free(void);
public:
	static SurfaceToAirMissileFireControlSystem* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, ObjectInfomation& metaData);
	virtual Engine::Component* Clone(void);

	HRESULT Start(void);
	virtual HRESULT Awake(void) override;
	virtual void Update(void) override;
	virtual void LateUpdate(void) override;
	virtual void FixedUpdate(void) override;

	void MissileControl(void);
private:

} typedef SAMFCS;