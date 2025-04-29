#pragma once


class TargetDummy : public Engine::GameObject
{
private:
	TargetDummy(void) = delete;
	TargetDummy(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	TargetDummy(const GameObject& other);
	virtual ~TargetDummy(void);
	virtual void Free(void);
public:
	static TargetDummy* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, Vector3 position);
	virtual GameObject* Clone(void);

	HRESULT Start(void);
	virtual void Update(void);
	virtual void LateUpdate(void);
	virtual void FixedUpdate(void);
	virtual void Render(void);

private:

};