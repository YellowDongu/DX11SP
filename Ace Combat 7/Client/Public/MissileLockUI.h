#pragma once
#include "UIObject.h"

class MissileLockUI : public Engine::UIObject
{
private:
	MissileLockUI(void) = delete;
	MissileLockUI(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	MissileLockUI(const MissileLockUI&) = delete;
	virtual ~MissileLockUI(void) = default;
	virtual void Free(void);
public:
	static MissileLockUI* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);

	HRESULT Start(void) override;
	HRESULT Awake(void) override;
	void FixedUpdate(void) override;
	void Update(void) override;
	void LateUpdate(void) override;
	void Render(void) override;
private:

};
