#pragma once

namespace Engine
{
	class StaticModel;
}

class AntiAirGunTruck : public Engine::GameObject
{
public:
	AntiAirGunTruck(void) = delete;
	AntiAirGunTruck(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	AntiAirGunTruck(const AntiAirGunTruck& other);
	virtual ~AntiAirGunTruck(void) = default;
	virtual void Free(void) override;
	static AntiAirGunTruck* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	virtual Engine::GameObject* Clone(void) override;

	HRESULT Start(void);
	virtual HRESULT Awake(void) override;
	virtual void Update(void) override;
	virtual void LateUpdate(void) override;
	virtual void FixedUpdate(void) override;
	virtual void Render(void) override;
private:
	Engine::StaticModel* model{ nullptr };
} typedef AAGUN;