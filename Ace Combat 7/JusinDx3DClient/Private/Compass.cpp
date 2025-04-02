#include "pch.h"
#include "Compass.h"

Compass::Compass(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::UIObject(dxDevice, dxDeviceContext)
{
}

void Compass::Free(void)
{
	compassLine.Free();
	Base::Destroy(text);
}

Compass* Compass::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, Engine::GameObject* player)
{
	Compass* newInstance = new Compass(dxDevice, dxDeviceContext);
	newInstance->forward = &player->transform()->Forward();

	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
	return newInstance;
}

#define innerErrorCheck(result, errmsg)										  \
if (FAILED(result))															  \
{																			  \
	ErrMsg((std::wstring(L"AttitudeIndicator::Failed : ") + errmsg).c_str()); \
	return E_FAIL;															  \
}																			  \

HRESULT Compass::Start(void)
{
	UIParts& part = compassLine;

	innerErrorCheck(compassLine.LoadUITexture(L"../Bin/Resources/UI/HUD/Compass_VerticalLine.png"), L"Load texture - Compass_VerticalLine.png");
	CreateScale(compassLine.texture, scale);
	innerErrorCheck(CreateVertex(compassLine.vertexBuffer, compassLine.indexBuffer, compassLine.indexCount, scale), L"Vertex/Index Create");

	text = CreateText(L"../Bin/ACES07.spritefont");
	if (text == nullptr)
		return E_FAIL;	
	return S_OK;
}

HRESULT Compass::Awake(void)
{
	return E_NOTIMPL;
}

void Compass::FixedUpdate(void)
{
}

void Compass::Update(void)
{
}

void Compass::LateUpdate(void)
{
	AddRenderObject(RenderType::UI, this);
	currentAngle = GetDirection(DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(forward)));
}
float fontSize = 16.0f;
void Compass::Render(void)
{
	static const UINT CMPUIstride = sizeof(UIVertex);
	std::string diffuseTexture = "diffuseTexture";
	std::string world = "worldMatrix";
	//
	Vector2 position = { 0.0f, static_cast<FLOAT>(windowSizeY) * 0.08f };
	Matrix worldMatrix;
	FLOAT totalSize = 50.0f;
	float angleStart = currentAngle - 20.5f;
	float angleEnd = currentAngle + 20.5f;
	float angleScale = totalSize / 21.0f;  // 1도당 이동 거리
	int tryCount = 0;
	for (float theta = std::ceil(angleStart / 5.0f) * 5.0f; theta <= angleEnd; theta += 5.0f)
	{
		float xPos = angleScale * (theta - currentAngle);
		//ticks.push_back({ theta, xPos });
		position.x = xPos;
		
		DirectX::XMStoreFloat4x4(&worldMatrix, CreateMatrix(position, Vector2::one(), 0.0f));
		SetMatrix(world, worldMatrix);
		
		SetTexture(diffuseTexture, compassLine.texture);
		GetCurrentShader()->Render(compassLine.indexBuffer, compassLine.vertexBuffer, CMPUIstride);
		dxDeviceContext->DrawIndexed(compassLine.indexCount, 0, 0);

		if (std::abs(xPos) < (fontSize + 1.0f) * 1.25f)
			continue;
		if (theta == 0.0f || theta == 360.0f)
			text->RenderText(L"N", { position.x * 1.575f, static_cast<FLOAT>(windowSizeY) * 0.295f }, float2{ 0.8f, 1.25f }, { 0.0f, 1.0f, 0.0f, 1.0f }, 0.0f);
		else if (theta == 90.0f)
			text->RenderText(L"E", { position.x * 1.575f, static_cast<FLOAT>(windowSizeY) * 0.295f }, float2{ 0.8f, 1.25f }, { 0.0f, 1.0f, 0.0f, 1.0f }, 0.0f);
		else if (theta == 180.0f)
			text->RenderText(L"S", { position.x * 1.575f, static_cast<FLOAT>(windowSizeY) * 0.295f }, float2{ 0.8f, 1.25f }, { 0.0f, 1.0f, 0.0f, 1.0f }, 0.0f);
		else if (theta == 270.0f)
			text->RenderText(L"W", { position.x * 1.575f, static_cast<FLOAT>(windowSizeY) * 0.295f }, float2{ 0.8f, 1.25f }, { 0.0f, 1.0f, 0.0f, 1.0f }, 0.0f);
		tryCount++;
	}


	position = { 0.0f, static_cast<FLOAT>(windowSizeY) * 0.295f };
	wsprintf(buffer, L"%03d", currentAngle);
	
	position.x -= fontSize * 1.25f;
	text->RenderText(buffer, position, float2{ 0.8f, 1.25f }, { 0.0f, 1.0f, 0.0f, 1.0f }, 0.0f);
}

FLOAT Compass::GetDirection(fxmVector forward)
{
	DirectX::XMVECTOR North = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	DirectX::XMVECTOR Up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	float dotProduct = DirectX::XMVectorGetX(DirectX::XMVector3Dot(forward, North));
	dotProduct = clamp(dotProduct, -1.0f, 1.0f);

	float degree = DirectX::XMConvertToDegrees(acosf(dotProduct));

	if (DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMVector3Cross(forward, North), Up)) < 0)
		degree = 360 - degree;

	return 360 - degree;
}
