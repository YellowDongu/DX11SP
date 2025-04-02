#pragma once
#include "FlightMovement.h"
#include "Model.h"

class F15E : public Engine::GameObject
{
private:
	F15E(void) = delete;
	F15E(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	F15E(const F15E& other);
	virtual ~F15E(void) = default;
	virtual void Free(void);

public:
	static F15E* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	virtual GameObject* Clone(void);

	HRESULT Start(void);


	virtual void Update(void);
	virtual void LateUpdate(void);
	virtual void FixedUpdate(void);
	virtual void Render(void);

private:
	Engine::Model* model;
	FlightMovement* flightModule;

};
