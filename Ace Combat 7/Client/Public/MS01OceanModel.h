#pragma once

#include "Polygon.h"

class MS01OceanModel : public Engine::Polygon
{
private:
	MS01OceanModel(void) = delete;
	MS01OceanModel(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	MS01OceanModel(const MS01OceanModel& other);
	virtual ~MS01OceanModel(void) = default;
	virtual void Free(void) override;
public:
	static MS01OceanModel* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	virtual Engine::Component* Clone(void);

	virtual HRESULT Awake(void) override;
	virtual void Update(void) override;
	virtual void LateUpdate(void) override;
	virtual void FixedUpdate(void) override;
	virtual void Render(void) override;

private:

};
