#pragma once
#include "UIObject.h"

class BackGround : public Engine::UIObject
{
private:
	BackGround(void) = delete;
	BackGround(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	//BackGround(const BackGround& other);
	virtual ~BackGround(void) = default;
	virtual void Free(void);
public:
	virtual BackGround* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	virtual Engine::GameObject* Clone(void);

	virtual HRESULT Start(void);
	virtual HRESULT Awake(void);
	virtual void FixedUpdate(void);
	virtual void Update(void);
	virtual void LateUpdate(void);
	virtual void Render(void);


private:
	ID3D11ShaderResourceView* backGroundTexture;
	ID3D11Buffer* indexBuffer;
	ID3D11Buffer* vertexBuffer;
	UINT indexCount;
};
