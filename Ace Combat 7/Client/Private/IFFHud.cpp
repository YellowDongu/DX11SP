#include "pch.h"
#include "IFFHud.h"
#include "Camera.h"
#include "TargetDummy.h"
#include "FireControlSystem.h"
#include "Missile.h"
#include "AIPilot.h"
#include "SuperClassAIPilot.h"

IdentificationFriendorFoeHeadUpDisplay::IdentificationFriendorFoeHeadUpDisplay(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::UIObject(dxDevice, dxDeviceContext)
{
}

void IdentificationFriendorFoeHeadUpDisplay::Free(void)
{
	aircraftMarker.Free();
	negativeMarker.Free();
	Base::DestroyInstance(text);
}

IdentificationFriendorFoeHeadUpDisplay* IdentificationFriendorFoeHeadUpDisplay::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, Engine::Scene* scene, Engine::GameObject* player, Engine::GameObject* camera)
{
	IFFHUD* newInstance = new IFFHUD(dxDevice, dxDeviceContext);

	//newInstance->AddLayers(scene);
	newInstance->player = player;
	newInstance->cameraObject = camera;
	newInstance->camera = static_cast<Engine::Camera*>(camera->GetComponent(L"Camera"));
	newInstance->deviceInfomation = &Device()->ViewPortInfomation();
	newInstance->fcs = static_cast<FireControlSystem*>(player->GetComponent(L"FCS"));
	newInstance->targeted = &newInstance->fcs->LinkTarget();
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

	innerErrorCheck(negativeMarker.LoadUITexture(L"../Bin/Resources/UI/HUD/Container/hud_lockNG.dds"), L"Load texture - hud_lockNG.dds");
	CreateScale(negativeMarker.texture, markerScale);
	innerErrorCheck(CreateVertex(negativeMarker.vertexBuffer, negativeMarker.indexBuffer, negativeMarker.indexCount, markerScale), L"Vertex/Index Create");

	
	text = ::CreateText(L"../Bin/ACES07.spritefont");
	if (text == nullptr)
		return E_FAIL;
	return S_OK;
}
#undef innerErrorCheck

HRESULT IdentificationFriendorFoeHeadUpDisplay::Awake(void)
{
	AddLayers(EngineInstance()->SceneManager()->CurrentScene());
	return S_OK;
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
	static std::wstring pilot = L"AIPilot";
	const ObjectInfomation* infomation = nullptr;
	bool groundAim, airAim, canAim;
	switch (fcs->WeaponSelectedStatus())
	{
	case -1:
		groundAim = false;
		airAim = false;
		break;
	case 0:
		groundAim = true;
		airAim = true;
		break;
	case 1:
	{
		Missile* missile = fcs->GetUniqueMissile();
		groundAim = missile->Ground();
		airAim = missile->Air();
		break;
	}
	default:
		break;
	}


	static std::string uiColorName = "UIcolor";
	Vector2 scale = Vector2::one() * 0.65f;
	::GetCurrentShader()->BindVariable(uiColorName, &uiColor, sizeof(DirectX::XMFLOAT4));
	if (Enemy != nullptr)
	{
		for (auto& object : Enemy->GameObjectList())
		{
			infomation = &static_cast<AIPilot*>(object.second->GetComponent(pilot))->LinkObjectInfomation();
			switch (infomation->aircraftInfomation.unitType)
			{
			case GroundUnit:
				canAim = groundAim;
				break;
			case AirUnit:
				canAim = airAim;
				break;
			case SeaUnit:
				canAim = false;
				break;
			default:
				canAim = false;
				break;
			}
			RenderHUD(object.second, aircraftMarker, markerScale, Vector2::one() * 0.65f, false, !canAim);
		}
	}
	if (MainTargetEnemy != nullptr)
	{
		for (auto& object : MainTargetEnemy->GameObjectList())
		{
			infomation = &static_cast<AIPilot*>(object.second->GetComponent(pilot))->LinkObjectInfomation();
			switch (infomation->aircraftInfomation.unitType)
			{
			case GroundUnit:
				canAim = groundAim;
				break;
			case AirUnit:
				canAim = airAim;
				break;
			case SeaUnit:
				canAim = false;
				break;
			default:
				canAim = false;
				break;
			}
			RenderHUD(object.second, aircraftMarker, markerScale, Vector2::one() * 0.65f, true, !canAim);
		}
	}
	if (Ally != nullptr)
	{
		static DirectX::XMFLOAT4 mint = { 0.0f, 1.0f, 1.0f, 1.0f };
		::GetCurrentShader()->BindVariable(uiColorName, &mint, sizeof(DirectX::XMFLOAT4));
		for (auto& object : Ally->GameObjectList())
		{
			if (object.second == player)
				continue;
			RenderHUD(object.second, aircraftMarker, markerScale, Vector2::one() * 0.65f);
		}
		::GetCurrentShader()->BindVariable(uiColorName, &uiColor, sizeof(DirectX::XMFLOAT4));
	}
}

void IdentificationFriendorFoeHeadUpDisplay::RenderUI(Engine::Layer* layer, UIParts& parts)
{
}

void IdentificationFriendorFoeHeadUpDisplay::RenderHUD(Engine::GameObject* object, UIParts& parts, Vector2 markerScale, Vector2 scale, bool mainTarget, bool aimNegative)
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
	distance = ::ConvertWorldToFeet(distance) / 2.5f;

	//if (distance >= maxDistance)
	//	continue;

	screenPosition = Vector2(::WorldToScreen(object->transform()->Position(), DirectX::XMLoadFloat4x4(&camera->ViewProjectionMatrix()), deviceInfomation, inScreen));
	if (!inScreen)
		return;

	screenPosition.x *= (deviceInfomation->Width * 0.5f);
	screenPosition.y *= (deviceInfomation->Height * 0.5f);

	DirectX::XMStoreFloat4x4(&worldMatrix, CreateMatrix(screenPosition, scale, 0.0f));
	::SetMatrix(world, worldMatrix);

	::SetTexture(diffuseTexture, parts.texture);
	::GetCurrentShader()->Render(parts.indexBuffer, parts.vertexBuffer, SIUIstride);
	dxDeviceContext->DrawIndexed(parts.indexCount, 0, 0);

	if (aimNegative)
	{
		::SetTexture(diffuseTexture, negativeMarker.texture);
		::GetCurrentShader()->Render(negativeMarker.indexBuffer, negativeMarker.vertexBuffer, SIUIstride);
		dxDeviceContext->DrawIndexed(negativeMarker.indexCount, 0, 0);
	}


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

	::SetTexture(diffuseTexture, parts.texture);
	::GetCurrentShader()->Render(parts.indexBuffer, parts.vertexBuffer, SIUIstride);
	dxDeviceContext->DrawIndexed(parts.indexCount, 0, 0);
	screenPosition.y += markerScale.y * 0.35f;
	screenPosition.x += markerScale.x * 0.325f;
	text->RenderText(std::to_wstring(static_cast<INT>(distance)), screenPosition, float2{ 0.55f, 0.5f }, uiColor, 0.0f);

	//Test
	pilot = static_cast<AIPilot*>(object->GetComponent(aiPilotComponentName));
	if (pilot == nullptr)
	{
		pilot = static_cast<AIPilot*>(object->GetComponent(aiSuperClassPilotComponentName));
	}

	if (pilot->LinkObjectInfomation().allyInfo == 1)
	{
		screenPosition.y -= markerScale.y * 0.35f;

		text->RenderText(L"ALLY", screenPosition, float2{ 0.55f, 0.5f }, uiColor, 0.0f);
	}
	else if (*targeted == object)
	{
		screenPosition.y -= markerScale.y * 0.35f;

		if (pilot == nullptr)
			text->RenderText(L"CURRENTTARGET", screenPosition, float2{ 0.55f, 0.5f }, uiColor, 0.0f);
		else
			text->RenderText(pilot->LinkObjectInfomation().aircraftInfomation.AircraftModelName, screenPosition, float2{ 0.55f, 0.5f }, uiColor, 0.0f);
	}
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
	Ally = scene->FindLayer(L"Ally");
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
