#include "pch.h"
#include "RaderUI.h"

RaderUI::RaderUI(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::UIObject(dxDevice, dxDeviceContext)
{
}

void RaderUI::Free(void)
{
	HUDOutline.Free();
	HUDCircleL.Free();
	HUDCircleM.Free();
	HUDCircleS.Free();
	RaderInnerLines.Free();
	//rader.Free();
	guideLine.Free();
	aircraft.Free();
	targetAircraft.Free();
}

RaderUI* RaderUI::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, Engine::GameObject* player, Engine::Layer** layers, UINT layerNumber)
{
	RaderUI* newInstance = new RaderUI(dxDevice, dxDeviceContext);
	newInstance->player = player;
	for (UINT i = 0; i < layerNumber; i++)
	{
		newInstance->searchLayers.push_back(layers[i]);
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
	ErrMsg((std::wstring(L"RaderUI::Failed : ") + errmsg).c_str()); \
	return E_FAIL;										   \
}														   \


HRESULT RaderUI::Start(void)
{
	Vector2 scale;

	innerErrorCheck(HUDOutline.LoadUITexture(L"../Bin/Resources/UI/HUD/Radar/hud_radarOutline_S.png"), L"Load texture - hud_radarOutline_S.png");
	CreateScale(HUDOutline.texture, HUDOutlineScale);
	innerErrorCheck(CreateVertex(HUDOutline.vertexBuffer, HUDOutline.indexBuffer, HUDOutline.indexCount, HUDOutlineScale), L"Vertex/Index Create");
	HUDLargeOutlineScale = Vector2{ HUDOutlineScale.x * 2.0f, HUDOutlineScale.y * 1.5f };
	innerErrorCheck(HUDCircleS.LoadUITexture(L"../Bin/Resources/UI/HUD/Radar/RaderCircle_S.dds"), L"Load texture - RaderCircle_S.dds");
	CreateScale(HUDCircleS.texture, scale);
	innerErrorCheck(CreateVertex(HUDCircleS.vertexBuffer, HUDCircleS.indexBuffer, HUDCircleS.indexCount, scale), L"Vertex/Index Create");

	innerErrorCheck(HUDCircleM.LoadUITexture(L"../Bin/Resources/UI/HUD/Radar/RaderCircle_M.png"), L"Load texture - RaderCircle_M.png");
	CreateScale(HUDCircleM.texture, scale);
	innerErrorCheck(CreateVertex(HUDCircleM.vertexBuffer, HUDCircleM.indexBuffer, HUDCircleM.indexCount, scale), L"Vertex/Index Create");

	innerErrorCheck(HUDCircleL.LoadUITexture(L"../Bin/Resources/UI/HUD/Radar/RaderCircle_L.dds"), L"Load texture - RaderCircle_L.dds");
	CreateScale(HUDCircleL.texture, scale);
	innerErrorCheck(CreateCustomVertex(HUDCircleL.vertexBuffer, HUDCircleL.indexBuffer, HUDCircleL.indexCount, scale), L"Vertex/Index Create");

	innerErrorCheck(RaderInnerLines.LoadUITexture(L"../Bin/Resources/UI/HUD/Radar/RaderInnerLines.dds"), L"Load texture - RaderInnerLines.dds");
	CreateScale(RaderInnerLines.texture, scale);
	innerErrorCheck(CreateVertex(RaderInnerLines.vertexBuffer, RaderInnerLines.indexBuffer, RaderInnerLines.indexCount, scale), L"Vertex/Index Create");



	innerErrorCheck(aircraft.LoadUITexture(L"../Bin/Resources/UI/HUD/Radar/RaderIcon_Player.dds"), L"Load texture - RaderIcon_Player.dds");
	CreateScale(aircraft.texture, scale);
	innerErrorCheck(CreateVertex(aircraft.vertexBuffer, aircraft.indexBuffer, aircraft.indexCount, scale), L"Vertex/Index Create");

	innerErrorCheck(targetAircraft.LoadUITexture(L"../Bin/Resources/UI/HUD/Radar/RaderIcon_airTGT.dds"), L"Load texture - RaderIcon_airTGT.dds");
	CreateScale(targetAircraft.texture, scale);
	innerErrorCheck(CreateVertex(targetAircraft.vertexBuffer, targetAircraft.indexBuffer, targetAircraft.indexCount, scale), L"Vertex/Index Create");

	CenterPosition = { windowSizeX * -0.375f, windowSizeY * -0.3f };

	//innerErrorCheck(DirectX::CreateDDSTextureFromFile(dxDevice, L"../Bin/Resources/UI/HUD/Rader/hud_alt_outline.dds", nullptr, &part.texture), L"Load texture - hud_alt_outline.dds");
	//innerErrorCheck(CreateVertex(part.vertexBuffer, part.indexBuffer, part.indexCount, scale), L"Vertex/Index Create");
	//innerErrorCheck(DirectX::CreateWICTextureFromFile(dxDevice, L"../Bin/Resources/UI/HUD/Rader/hud_alt_outline.png", nullptr, &part.texture), L"Load texture - hud_alt_outline.png");
	//innerErrorCheck(CreateVertex(part.vertexBuffer, part.indexBuffer, part.indexCount, scale), L"Vertex/Index Create");
	#undef checkSize
	return S_OK;
}

HRESULT RaderUI::Awake(void)
{
	if (FAILED(CollectLayers()))
		return E_FAIL;
	return S_OK;
}

void RaderUI::FixedUpdate(void)
{
}

void RaderUI::Update(void)
{
	if (Input()->getButtonDown(KeyType::R))
	{
		raderState++;
	}


}

void RaderUI::LateUpdate(void)
{
	AddRenderObject(RenderType::UI, this);

}

void RaderUI::Render(void)
{
	switch (raderState)
	{
	case 0:
		MiniMapModeRender();
		break;
	case 1:
		LargeMapModeRender();
		break;
	default:
		raderState = 0;
		MiniMapModeRender();
		break;
	}

}

void RaderUI::MiniMapModeRender(void)
{
	Vector2 scale = Vector2::one();
	Matrix worldMatrix;
	Vector2 position = CenterPosition;
	Engine::Shader* shader = GetCurrentShader();

	DirectX::XMStoreFloat4x4(&worldMatrix, CreateMatrix(position, scale, 0.0f));
	SetMatrix(world, worldMatrix);

	SetTexture(diffuseTexture, HUDOutline.texture);
	shader->Render(HUDOutline.indexBuffer, HUDOutline.vertexBuffer, stride);
	dxDeviceContext->DrawIndexed(HUDOutline.indexCount, 0, 0);

	SetTexture(diffuseTexture, RaderInnerLines.texture);
	shader->Render(RaderInnerLines.indexBuffer, RaderInnerLines.vertexBuffer, stride);
	dxDeviceContext->DrawIndexed(RaderInnerLines.indexCount, 0, 0);

	position.y -= windowSizeY * 0.045f;
	DirectX::XMStoreFloat4x4(&worldMatrix, CreateMatrix(position, scale, 0.0f));
	SetMatrix(world, worldMatrix);

	SetTexture(diffuseTexture, HUDCircleS.texture);
	shader->Render(HUDCircleS.indexBuffer, HUDCircleS.vertexBuffer, stride);
	dxDeviceContext->DrawIndexed(HUDCircleS.indexCount, 0, 0);

	SetTexture(diffuseTexture, HUDCircleM.texture);
	shader->Render(HUDCircleM.indexBuffer, HUDCircleM.vertexBuffer, stride);
	dxDeviceContext->DrawIndexed(HUDCircleM.indexCount, 0, 0);

	position.y += HUDOutlineScale.y * 0.07f;
	scale.y = 0.825f;
	DirectX::XMStoreFloat4x4(&worldMatrix, CreateMatrix(position, scale, 180.0f));
	SetMatrix(world, worldMatrix);
	SetTexture(diffuseTexture, HUDCircleL.texture);
	shader->Render(HUDCircleL.indexBuffer, HUDCircleL.vertexBuffer, stride);
	dxDeviceContext->DrawIndexed(HUDCircleL.indexCount, 0, 0);


	static std::string uiColorName = "UIcolor";
	static DirectX::XMFLOAT4 mint = { 0.0f, 1.0f, 1.0f, 1.0f };
	static DirectX::XMFLOAT4 green = { 0.0f, 1.0f, 0.0f, 1.0f };
	static DirectX::XMFLOAT4 red = { 1.0f, 0.0f, 0.0f, 1.0f };

	CollectGameObjects(Allylayer, relativePositions);
	scale = Vector2::one();

	shader->BindVariable(uiColorName, &mint, sizeof(DirectX::XMFLOAT4));
	for (auto& relativePosition : relativePositions)
	{
		position = CenterPosition;
		position.x += relativePosition.first.x * 0.25f;
		position.y += relativePosition.first.y * 0.25f;

		DirectX::XMStoreFloat4x4(&worldMatrix, CreateMatrix(position, scale, relativePosition.second * -1.0f));
		shader->BindMatrix(world, worldMatrix);
		shader->BindTexture(diffuseTexture, aircraft.texture);
		shader->Render(aircraft.indexBuffer, aircraft.vertexBuffer, stride);
		dxDeviceContext->DrawIndexed(aircraft.indexCount, 0, 0);
	}


	CollectGameObjects(MainTargetlayer, relativePositions);

	shader->BindVariable(uiColorName, &red, sizeof(DirectX::XMFLOAT4));
	for (auto& relativePosition : relativePositions)
	{
		position = CenterPosition;
		position.x += relativePosition.first.x * 0.25f;
		position.y += relativePosition.first.y * 0.25f;

		DirectX::XMStoreFloat4x4(&worldMatrix, CreateMatrix(position, scale, relativePosition.second * -1.0f));
		shader->BindMatrix(world, worldMatrix);
		shader->BindTexture(diffuseTexture, aircraft.texture);
		shader->Render(aircraft.indexBuffer, aircraft.vertexBuffer, stride);
		dxDeviceContext->DrawIndexed(aircraft.indexCount, 0, 0);
	}
	shader->BindVariable(uiColorName, &green, sizeof(DirectX::XMFLOAT4));
}

void RaderUI::LargeMapModeRender(void)
{
}

HRESULT RaderUI::CollectLayers(void)
{
	std::list<Engine::Layer*> layersBucket;
	Engine::Scene* currentScene = EngineInstance()->SceneManager()->CurrentScene();
	Engine::Layer* layer = nullptr;



	//layer = currentScene->FindLayer(L"MainTargetAlly");
	//if (layer != nullptr)
	//	layersBucket.push_back(layer);
	//layer = currentScene->FindLayer(L"MainTargetAllyGround");
	//if (layer != nullptr)
	//	layersBucket.push_back(layer);
	Allylayer = currentScene->FindLayer(L"Ally");

	//layer = currentScene->FindLayer(L"GroundAlly");
	//if (layer != nullptr)
	//	layersBucket.push_back(layer);
	//layer = currentScene->FindLayer(L"NonTargetAlly");
	//if (layer != nullptr)
	//	layersBucket.push_back(layer);
	//layer = currentScene->FindLayer(L"NonTargetGroundAlly");
	//if (layer != nullptr)
	//	layersBucket.push_back(layer);


	MainTargetlayer = currentScene->FindLayer(L"MainTargetEnemy");
	Enemylayer = currentScene->FindLayer(L"Enemy");


	//layer = currentScene->FindLayer(L"MainTargetEnemyGround");
	//if (layer != nullptr)
	//	layersBucket.push_back(layer);
	//layer = currentScene->FindLayer(L"GroundEnemy");
	//if (layer != nullptr)
	//	layersBucket.push_back(layer);
	//layer = currentScene->FindLayer(L"NonTargetEnemy");
	//if (layer != nullptr)
	//	layersBucket.push_back(layer);
	//layer = currentScene->FindLayer(L"NonTargetGroundEnemy");
	//if (layer != nullptr)
	//	layersBucket.push_back(layer);
	//
	//layer = currentScene->FindLayer(L"UnKnown");
	//layer = currentScene->FindLayer(L"GroundUnKnown");
	return S_OK;
}

void RaderUI::CollectGameObjectsWorld(Engine::Layer* layer, std::vector<std::pair<Vector2, FLOAT>>& relativePositionsList)
{
	relativePositionsList.clear();
	if (layer == nullptr)
		return;

	for (auto& gameObject : layer->GameObjectList())
	{
		if (gameObject.second == player)
			continue;
		relativePositionsList.push_back({ {gameObject.second->transform()->Position().x, gameObject.second->transform()->Position().z}, Vector3::getDirection(gameObject.second->transform()->Forward()).y });
	}
}

void RaderUI::CollectGameObjects(Engine::Layer* layer, std::vector<std::pair<Vector2, FLOAT>>& relativePositionsList)
{
	relativePositionsList.clear();
	if (layer == nullptr)
		return;

	Vector3& playerPosition = player->transform()->Position();
	FLOAT angle = Vector3::getDirection(player->transform()->Forward()).y;
	static Vector2 outlineScale = HUDOutlineScale * 0.5f;
	FLOAT relativeAngle = 0.0f;
	Vector3 relativePosition;
	for (auto& gameObject : layer->GameObjectList())
	{
		relativePosition = gameObject.second->transform()->Position() - playerPosition;

		DirectX::XMStoreFloat3(&relativePosition, DirectX::XMVector3Rotate(DirectX::XMLoadFloat3(&relativePosition), DirectX::XMQuaternionRotationRollPitchYaw(0.0f, DirectX::XMConvertToRadians(-angle), 0.0f)));

		if (relativePosition.x > outlineScale.x || relativePosition.z > outlineScale.y)
			continue;

		relativeAngle = Vector3::getDirection(gameObject.second->transform()->Forward()).y - angle;
		while (relativeAngle < 0.0f)
			relativeAngle += 360.0f;

		relativePositionsList.push_back({ {relativePosition.x, relativePosition.z}, relativeAngle });
	}
}

HRESULT RaderUI::CreateCustomVertex(ID3D11Buffer*& vertexBuffer, ID3D11Buffer*& indexBuffer, UINT& vertexCount, const Vector2& scale)
{
	std::vector<UIVertex> vertex;
	std::vector<UINT> index;
	CreateCustomVertex(vertex, index, scale);
	vertexCount = static_cast<UINT>(index.size());
	return CreateBuffer(dxDevice, vertexBuffer, indexBuffer, vertex, index);
}

void RaderUI::CreateCustomVertex(std::vector<UIVertex>& vertex, std::vector<UINT>& index, const Vector2& scale)
{
	vertex.clear();
	UIVertex newVertex;
	Vector2 customScale = scale;
	customScale.y *= 0.75f;

	newVertex.position = { scale.x / -2.0f, scale.y / -2.0f };
	newVertex.texturePosition = { 0.0f, 0.0f };
	vertex.push_back(newVertex);

	newVertex.position = { scale.x / 2.0f, scale.y / -2.0f };
	newVertex.texturePosition = { 1.0f, 0.0f };
	vertex.push_back(newVertex);

	newVertex.position = { scale.x / 2.0f, scale.y / 2.0f };
	newVertex.texturePosition = { 1.0f, 0.825f };
	vertex.push_back(newVertex);

	newVertex.position = { scale.x / -2.0f, scale.y / 2.0f };
	newVertex.texturePosition = { 0.0f, 0.825f };
	vertex.push_back(newVertex);

	index.push_back(0);
	index.push_back(2);
	index.push_back(1);

	index.push_back(0);
	index.push_back(3);
	index.push_back(2);
}