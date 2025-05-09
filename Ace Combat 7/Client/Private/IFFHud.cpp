#include "pch.h"
#include "IFFHud.h"
#include "Camera.h"
#include "TargetDummy.h"
#include "FireControlSystem.h"

#include "AIPilot.h"
#include "SuperClassAIPilot.h"

IdentificationFriendorFoeHeadUpDisplay::IdentificationFriendorFoeHeadUpDisplay(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::UIObject(dxDevice, dxDeviceContext)
{
}

void IdentificationFriendorFoeHeadUpDisplay::Free(void)
{
	aircraftMarker.Free();
	Base::DestroyInstance(text);
}

IdentificationFriendorFoeHeadUpDisplay* IdentificationFriendorFoeHeadUpDisplay::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, Engine::Scene* scene, Engine::GameObject* player, Engine::GameObject* camera)
{
	IFFHUD* newInstance = new IFFHUD(dxDevice, dxDeviceContext);

	newInstance->AddLayers(scene);
	newInstance->player = player;
	newInstance->cameraObject = camera;
	newInstance->camera = static_cast<Engine::Camera*>(camera->GetComponent(L"Camera"));
	newInstance->deviceInfomation = &Device()->ViewPortInfomation();
	newInstance->targeted = &static_cast<FireControlSystem*>(player->GetComponent(L"FCS"))->LinkTarget();
	if (camera == nullptr)
	{
		Base::Destroy(newInstance);
		return nullptr;
	}

	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}

	return newInstance;
}

#define innerErrorCheck(result, errmsg)					   \
if (FAILED(result))										   \
{														   \
	ErrMsg((std::wstring(L"FlightPathMarker::Failed : ") + errmsg).c_str()); \
	return E_FAIL;										   \
}														   \

HRESULT IdentificationFriendorFoeHeadUpDisplay::Start(void)
{
	innerErrorCheck(aircraftMarker.LoadUITexture(L"../Bin/Resources/UI/HUD/Container/hud_container_air01.dds"), L"Load texture - hud_container_air01.dds");
	CreateScale(aircraftMarker.texture, markerScale);
	innerErrorCheck(CreateVertex(aircraftMarker.vertexBuffer, aircraftMarker.indexBuffer, aircraftMarker.indexCount, markerScale), L"Vertex/Index Create");

	text = CreateText(L"../Bin/ACES07.spritefont");
	if (text == nullptr)
		return E_FAIL;
	return S_OK;
}
#undef innerErrorCheck

HRESULT IdentificationFriendorFoeHeadUpDisplay::Awake(void)
{
	AddLayers(EngineInstance()->SceneManager()->CurrentScene());
	return E_NOTIMPL;
}

void IdentificationFriendorFoeHeadUpDisplay::FixedUpdate(void)
{
}

void IdentificationFriendorFoeHeadUpDisplay::Update(void)
{
}

void IdentificationFriendorFoeHeadUpDisplay::LateUpdate(void)
{
	AddRenderObject(RenderType::UI, this);
}

void IdentificationFriendorFoeHeadUpDisplay::Render(void)
{
	Vector2 scale = Vector2::one() * 0.65f;
	if (Enemy != nullptr)
	{
		for (auto& object : Enemy->GameObjectList())
		{
			RenderHUD(object.second, aircraftMarker, markerScale, Vector2::one() * 0.65f);

		}
	}
	if (MainTargetEnemy != nullptr)
	{
		for (auto& object : MainTargetEnemy->GameObjectList())
		{
			RenderHUD(object.second, aircraftMarker, markerScale, Vector2::one() * 0.65f, true);
		}
	}
}

void IdentificationFriendorFoeHeadUpDisplay::RenderUI(Engine::Layer* layer, UIParts& parts)
{
}

void IdentificationFriendorFoeHeadUpDisplay::RenderHUD(Engine::GameObject* object, UIParts& parts, Vector2 markerScale, Vector2 scale, bool mainTarget)
{
	static std::wstring aiPilotComponentName = L"AIPilot";
	static std::wstring aiSuperClassPilotComponentName = L"SuperClassAIPilot";
	AIPilot* pilot = nullptr;

	bool inScreen;
	Vector2 screenPosition;
	static const UINT SIUIstride = sizeof(UIVertex);
	std::string diffuseTexture = "diffuseTexture";
	std::string world = "worldMatrix";
	Matrix worldMatrix;
	float distance;

	distance = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&object->transform()->Position()), DirectX::XMLoadFloat3(&player->transform()->Position()))));
	distance = ConvertWorldToFeet(distance) / 2.5f;

	//if (distance >= maxDistance)
	//	continue;

	screenPosition = Vector2(WorldToScreen(object->transform()->Position(), DirectX::XMLoadFloat4x4(&camera->ViewProjectionMatrix()), deviceInfomation, inScreen));
	if (!inScreen)
		return;

	screenPosition.x *= (deviceInfomation->Width * 0.5f);
	screenPosition.y *= (deviceInfomation->Height * 0.5f);

	DirectX::XMStoreFloat4x4(&worldMatrix, CreateMatrix(screenPosition, scale, 0.0f));
	SetMatrix(world, worldMatrix);

	SetTexture(diffuseTexture, parts.texture);
	GetCurrentShader()->Render(parts.indexBuffer, parts.vertexBuffer, SIUIstride);
	dxDeviceContext->DrawIndexed(parts.indexCount, 0, 0);

	if (mainTarget)
	{
		screenPosition.y += markerScale.y * 0.325f;
		screenPosition.x -= markerScale.x * 0.7f;
		text->RenderText(L"TGT", screenPosition, float2{ 0.55f, 0.5f }, { 1.0f, 0.0f, 0.0f, 1.0f }, 0.0f);

		screenPosition.y -= markerScale.y * 0.325f;
		screenPosition.x += markerScale.x * 0.7f;
	}

	//if (*targeted != object.second)
	//	continue;

	SetTexture(diffuseTexture, parts.texture);
	GetCurrentShader()->Render(parts.indexBuffer, parts.vertexBuffer, SIUIstride);
	dxDeviceContext->DrawIndexed(parts.indexCount, 0, 0);
	screenPosition.y += markerScale.y * 0.325f;
	screenPosition.x += markerScale.x * 0.25f;
	text->RenderText(std::to_wstring(static_cast<INT>(distance)), screenPosition, float2{ 0.55f, 0.5f }, uiColor, 0.0f);

	//Test
	if (*targeted != object)
		return;
	pilot = static_cast<AIPilot*>(object->GetComponent(aiPilotComponentName));
	if (pilot == nullptr)
	{
		pilot = static_cast<AIPilot*>(object->GetComponent(aiSuperClassPilotComponentName));
	}


	screenPosition.y -= markerScale.y * 0.325f;

	if (pilot == nullptr)
		text->RenderText(L"CURRENTTARGET", screenPosition, float2{ 0.55f, 0.5f }, uiColor, 0.0f);
	else
		text->RenderText(pilot->LinkObjectInfomation().aircraftInfomation.AircraftModelName, screenPosition, float2{ 0.55f, 0.5f }, uiColor, 0.0f);
}

void IdentificationFriendorFoeHeadUpDisplay::AddLayers(Engine::Scene* scene)
{
	MainTargetEnemy = scene->FindLayer(L"MainTargetEnemy");
	MainTargetEnemyGround = nullptr; 
	Enemy = scene->FindLayer(L"Enemy");
	GroundEnemy = nullptr;
	NonTargetEnemy = nullptr;
	NonTargetGroundEnemy = nullptr;
	MainTargetAlly = nullptr;
	MainTargetAllyGround = nullptr;
	Ally = nullptr;
	GroundAlly = nullptr;
	NonTargetAlly = nullptr;
	NonTargetGroundAlly = nullptr;
	UnKnown = nullptr;
	GroundUnKnown = nullptr;
	//layers[L"MainTargetEnemy"] = nullptr;
	//layers[L"MainTargetEnemyGround"] = nullptr;
	//layers[L"Enemy"] = nullptr;
	//layers[L"GroundEnemy"] = nullptr;
	//layers[L"NonTargetEnemy"] = nullptr;
	//layers[L"NonTargetGroundEnemy"] = nullptr;
	//layers[L"MainTargetAlly"] = nullptr;
	//layers[L"MainTargetAllyGround"] = nullptr;
	//layers[L"Ally"] = nullptr;
	//layers[L"GroundAlly"] = nullptr;
	//layers[L"NonTargetAlly"] = nullptr;
	//layers[L"NonTargetGroundAlly"] = nullptr;
	//layers[L"UnKnown"] = nullptr;
	//layers[L"GroundUnKnown"] = nullptr;
}
