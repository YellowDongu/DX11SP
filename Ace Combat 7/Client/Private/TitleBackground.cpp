#include "pch.h"
#include "TitleBackground.h"
#include "ScreenFadeOut.h"

Background::Background(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::UIObject(dxDevice, dxDeviceContext)
{
}

Background::Background(const Background& other) : Engine::UIObject(other)
{
}

void Background::Free(void)
{
	Engine::GameObject::Free();
	backGround.Free();
}

Background* Background::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
	Background* newInstance = new Background(dxDevice, dxDeviceContext);

	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}

	return newInstance;
}

Engine::GameObject* Background::Clone(void)
{
	Background* newInstance = new Background(*this);

	if (FAILED(newInstance->Awake()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}

	return newInstance;
}

#define innerErrorCheck(result, errmsg)					   \
if (FAILED(result))										   \
{														   \
	ErrMsg((std::wstring(L"Background::Failed : ") + errmsg).c_str()); \
	return E_FAIL;										   \
}

HRESULT Background::Start(void)
{
	HRESULT result = CreateTransform();
	if (FAILED(result))
		return result;

	innerErrorCheck(backGround.LoadUITexture(L"../Bin/UI/Menu/Boot/Assets/Title_bg.png"), L"Load texture - Title_bg.png");
	CreateScale(backGround.texture, backGround.scale);
	innerErrorCheck(CreateVertex(backGround.vertexBuffer, backGround.indexBuffer, backGround.indexCount,Vector2::one()), L"Vertex/Index Create");

	std::wstring path = L"../Bin/Resources/UI/Menu/Boot/Assets/Campaign_Loading_bg_005.dds";
	if (FAILED(LoadTexture(path, path, MainMenuTexture))) return E_FAIL;

	ID3D11ShaderResourceView* newTexture = nullptr;
	path = L"../Bin/Resources/UI/Menu/Boot/Assets/CopyrightUEImage.dds";
	if (FAILED(LoadTexture(path, path, newTexture))) return E_FAIL;
	else textures.push_back(newTexture);

	path = L"../Bin/Resources/UI/Menu/Boot/Assets/CopyrightBNEImage.dds";
	if (FAILED(LoadTexture(path, path, newTexture))) return E_FAIL;
	else textures.push_back(newTexture);

	path = L"../Bin/Resources/UI/Menu/Boot/Assets/logo_BNS_ACES.png";
	if (FAILED(LoadTexture(path, path, newTexture))) return E_FAIL;
	else textures.push_back(newTexture);

	path = L"../Bin/Resources/UI/Menu/Boot/Assets/WarningCopyCesaImage.dds";
	if (FAILED(LoadTexture(path, path, newTexture))) return E_FAIL;
	else textures.push_back(newTexture);
	

	//std::vector<UIVertex> vertex;
	//std::vector<UINT> index;
	//
	//vertex.clear();
	//UIVertex newVertex;
	//
	//
	//newVertex.position = { -0.5f, 0.5f };
	//newVertex.texturePosition = { 0.0f, 0.0f };
	//vertex.push_back(newVertex);
	//
	//newVertex.position = { 0.5f, 0.5f };
	//newVertex.texturePosition = { 1.0f, 0.0f };
	//vertex.push_back(newVertex);
	//
	//newVertex.position = { 0.5f,-0.5f };
	//newVertex.texturePosition = { 1.0f, 1.0f };
	//vertex.push_back(newVertex);
	//
	//newVertex.position = { -0.5f,-0.5f };
	//newVertex.texturePosition = { 0.0f, 1.0f };
	//vertex.push_back(newVertex);
	//
	//index.push_back(0);
	//index.push_back(1);
	//index.push_back(2);
	//
	//index.push_back(0);
	//index.push_back(2);
	//index.push_back(3);
	//
	//backGround.indexCount = static_cast<UINT>(index.size());
	//CreateBuffer(dxDevice, backGround.vertexBuffer, backGround.indexBuffer, vertex, index);


	//newVertex.position = { backGround.scale.x *-0.5f, backGround.scale.y * 0.5f };
	//newVertex.texturePosition = { 0.0f, 0.0f };
	//vertex.push_back(newVertex);
	//
	//newVertex.position = { backGround.scale.x * 0.5f, backGround.scale.y * 0.5f };
	//newVertex.texturePosition = { 1.0f, 0.0f };
	//vertex.push_back(newVertex);
	//
	//newVertex.position = { backGround.scale.x * 0.5f, backGround.scale.y *-0.5f };
	//newVertex.texturePosition = { 1.0f, 1.0f };
	//vertex.push_back(newVertex);
	//
	//newVertex.position = { backGround.scale.x *-0.5f, backGround.scale.y *-0.5f };
	//newVertex.texturePosition = { 0.0f, 1.0f };
	//vertex.push_back(newVertex);


	return S_OK;
}

HRESULT Background::Awake(void)
{
	return S_OK;
}

void Background::FixedUpdate(void)
{
}

void Background::Update(void)
{
	if (fadeoutScreen == nullptr)
		fadeoutScreen = static_cast<ScreenFadeOut*>(::EngineInstance()->SceneManager()->LoadingScene()->FindLayer(L"BaseLayer")->GetGameObject(L"ZZZScreenFadeOut"));

	if (fadeoutScreen->fadeOutStatus() == 1.0f)
	{
		splashScreenCount++;
		timer = 3.0f;
		fadeoutScreen->SetFadeOut(false);
		fadeoutScreen->SetFadeSpeed(5.0f);
	}
	else if (fadeoutScreen->fadeOutStatus() == 0.0f)
	{
		if (splashScreenCount < textures.size())
		{
			timer -= DeltaTime();

			if (timer < 0.0f)
			{
				fadeoutScreen->SetFadeOut(true);
			}
		}
	}


}

void Background::LateUpdate(void)
{
	AddRenderObject(RenderType::UI, this);
}

void Background::Render(void)
{
	Engine::Shader* shader = GetCurrentShader();
	shader->PassNumber(2);
	const D3D11_VIEWPORT& viewPortSetting = Device()->ViewPortInfomation();
	Vector2 scale = Vector2{ viewPortSetting.Width, viewPortSetting.Height };
	Matrix worldMatrix;

	DirectX::XMStoreFloat4x4(&worldMatrix, CreateMatrix(Vector2::zero(), scale, 0.0f));
	SetMatrix(world, worldMatrix);

	if (splashScreenCount >= textures.size())
	{
		if(splashScreen)
			shader->BindTexture(diffuseTexture, backGround.texture);
		else
			shader->BindTexture(diffuseTexture, MainMenuTexture);
	}
	else
		shader->BindTexture(diffuseTexture, textures[splashScreenCount]);
	shader->Render(backGround.indexBuffer, backGround.vertexBuffer, stride);
	dxDeviceContext->DrawIndexed(backGround.indexCount, 0, 0);
	shader->PassNumber(0);
}

bool Background::IsSplashScreenEnd(void)
{
	return splashScreenCount >= textures.size() && fadeoutScreen->fadeOutStatus() == 0.0f;
}
