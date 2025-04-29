#pragma once
#include "UIObject.h"

class TitleBackground : public Engine::UIObject
{
private:
	TitleBackground(void) = delete;
	TitleBackground(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	//BackGround(const BackGround& other);
	virtual ~TitleBackground(void) = default;
	virtual void Free(void);
public:
	virtual TitleBackground* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
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
