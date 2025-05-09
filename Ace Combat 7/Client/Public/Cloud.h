#pragma once

class Cloud : public Engine::GameObject
{
private:
	Cloud(void) = delete;
	Cloud(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	Cloud(const Cloud& other);
	virtual ~Cloud(void) = default;
	virtual void Free(void);
public:
	static Cloud* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	virtual Engine::GameObject* Clone(void) override;

	HRESULT Start(void);
	virtual HRESULT Awake(void) override;
	virtual void Update(void) override;
	virtual void LateUpdate(void) override;
	virtual void FixedUpdate(void) override;
	virtual void Render(void) override;


private:
};