#pragma once


class SurfaceToAirMissileTruck : public Engine::GameObject
{
public:
	SurfaceToAirMissileTruck(void) = delete;
	SurfaceToAirMissileTruck(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	SurfaceToAirMissileTruck(const SurfaceToAirMissileTruck& other);
	virtual ~SurfaceToAirMissileTruck(void) = default;
	virtual void Free(void) override;
	static SurfaceToAirMissileTruck* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	virtual Engine::GameObject* Clone(void) override;

	HRESULT Start(void);
	virtual HRESULT Awake(void) override;
	virtual void Update(void) override;
	virtual void LateUpdate(void) override;
	virtual void FixedUpdate(void) override;
	virtual void Render(void) override;

private:
	Engine::StaticModel* model{nullptr};
	class RadarMissileWarningReceiver* rmwr;
	class FireControlSystem* fcs;
}typedef SAM;

