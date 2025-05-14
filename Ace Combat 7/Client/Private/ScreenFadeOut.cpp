#include "pch.h"
#include "ScreenFadeOut.h"

#include "RectanglePolygon.h"

ScreenFadeOut::ScreenFadeOut(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::UIObject(dxDevice, dxDeviceContext)
{
}

ScreenFadeOut::ScreenFadeOut(const ScreenFadeOut& other) : Engine::UIObject(other)
{
}

void ScreenFadeOut::Free(void)
{
	part.Free();
}

ScreenFadeOut* ScreenFadeOut::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
	ScreenFadeOut* newInstance = new ScreenFadeOut(dxDevice, dxDeviceContext);

	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
	return nullptr;
	}

		return newInstance;
}

Engine::GameObject* ScreenFadeOut::Clone(void)
{
	return new ScreenFadeOut(*this);
}

HRESULT ScreenFadeOut::Start(void)
{
	part.scale = Vector2::one();
	if (FAILED(CreateVertex(part.vertexBuffer, part.indexBuffer, part.indexCount, Vector2::one())))
		return E_FAIL;

	viewPortInfomation = &EngineInstance()->Device()->ViewPortInfomation();
	return S_OK;
}

HRESULT ScreenFadeOut::Awake(void)
{
	return S_OK;
}

void ScreenFadeOut::Update(void)
{
	if (fadeOut && fadeOutTimer != 1.0f)
	{
		fadeOutTimer += DeltaTime() * fadeSpeed;
		if (fadeOutTimer >= 1.0f)
			fadeOutTimer = 1.0f;
	}
	else if (!fadeOut && fadeOutTimer != 0.0f)
	{
		fadeOutTimer -= DeltaTime() * fadeSpeed;
		if (fadeOutTimer <= 0.0f)
			fadeOutTimer = 0.0f;
	}

}

void ScreenFadeOut::LateUpdate(void)
{
	AddRenderObject(RenderType::UI, this);
}

void ScreenFadeOut::FixedUpdate(void)
{
}

void ScreenFadeOut::Render(void)
{
	if (fadeOutTimer == 0.0f)
		return;
	static UINT stride = sizeof(UIVertex);
	GetCurrentShader()->PassNumber(3);
	Vector2 scale = Vector2{ viewPortInfomation->Width, viewPortInfomation->Height };
	Matrix worldMatrix;

	DirectX::XMStoreFloat4x4(&worldMatrix, CreateMatrix(Vector2::zero(), scale, 180.0f));
	SetMatrix("worldMatrix", worldMatrix);

	GetCurrentShader()->BindVariable("prograss", &fadeOutTimer, sizeof(FLOAT));
	GetCurrentShader()->Render(part.indexBuffer, part.vertexBuffer, stride);
	dxDeviceContext->DrawIndexed(part.indexCount, 0, 0);
	GetCurrentShader()->PassNumber(0);

}
