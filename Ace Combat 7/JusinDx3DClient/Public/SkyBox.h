#pragma once


class SkyBox : public Engine::GameObject
{
private:
	SkyBox(void) = delete;
	SkyBox(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	SkyBox(const SkyBox&) = delete;
	virtual ~SkyBox(void) = default;
	virtual void Free(void);
public:
	static SkyBox* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	virtual GameObject* Clone(void) override;

	HRESULT Start(void);
	HRESULT Awake(void);

	virtual void Update(void) override;
	virtual void LateUpdate(void) override;
	virtual void FixedUpdate(void) override;
	virtual void Render(void) override;

private:
	bool originObject;
	float4 skyColor;
	

};