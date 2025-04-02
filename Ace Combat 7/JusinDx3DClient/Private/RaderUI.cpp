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
	Vector2 scale = { 350.0f, 350.0f };

	innerErrorCheck(HUDOutline.LoadUITexture(L"../Bin/Resources/UI/HUD/Radar/hud_radarOutline_S.png"), L"Load texture - hud_radarOutline_S.png");
	CreateScale(HUDOutline.texture, scale);
	innerErrorCheck(CreateVertex(HUDOutline.vertexBuffer, HUDOutline.indexBuffer, HUDOutline.indexCount, scale), L"Vertex/Index Create");

	innerErrorCheck(HUDCircleS.LoadUITexture(L"../Bin/Resources/UI/HUD/Radar/RaderCircle_S.dds"), L"Load texture - RaderCircle_S.dds");
	CreateScale(HUDCircleS.texture, scale);
	innerErrorCheck(CreateVertex(HUDCircleS.vertexBuffer, HUDCircleS.indexBuffer, HUDCircleS.indexCount, scale), L"Vertex/Index Create"); // UV 좌표를 잘라서 원의 일부분만 나오게 만들어야됨

	innerErrorCheck(HUDCircleM.LoadUITexture(L"../Bin/Resources/UI/HUD/Radar/RaderCircle_M.png"), L"Load texture - RaderCircle_M.png");
	CreateScale(HUDCircleM.texture, scale);
	innerErrorCheck(CreateVertex(HUDCircleM.vertexBuffer, HUDCircleM.indexBuffer, HUDCircleM.indexCount, scale), L"Vertex/Index Create"); // UV 좌표를 잘라서 원의 일부분만 나오게 만들어야됨

	innerErrorCheck(HUDCircleL.LoadUITexture(L"../Bin/Resources/UI/HUD/Radar/RaderCircle_L.dds"), L"Load texture - RaderCircle_L.dds");
	CreateScale(HUDCircleL.texture, scale);
	innerErrorCheck(CreateCustomVertex(HUDCircleL.vertexBuffer, HUDCircleL.indexBuffer, HUDCircleL.indexCount, scale), L"Vertex/Index Create"); // UV 좌표를 잘라서 원의 일부분만 나오게 만들어야됨

	innerErrorCheck(RaderInnerLines.LoadUITexture(L"../Bin/Resources/UI/HUD/Radar/RaderInnerLines.dds"), L"Load texture - RaderInnerLines.dds");
	CreateScale(RaderInnerLines.texture, scale);
	innerErrorCheck(CreateVertex(RaderInnerLines.vertexBuffer, RaderInnerLines.indexBuffer, RaderInnerLines.indexCount, scale), L"Vertex/Index Create");



	//innerErrorCheck(DirectX::CreateDDSTextureFromFile(dxDevice, L"../Bin/Resources/UI/HUD/Rader/hud_alt_outline.dds", nullptr, &part.texture), L"Load texture - hud_alt_outline.dds");
	//innerErrorCheck(CreateVertex(part.vertexBuffer, part.indexBuffer, part.indexCount, scale), L"Vertex/Index Create");
	//innerErrorCheck(DirectX::CreateWICTextureFromFile(dxDevice, L"../Bin/Resources/UI/HUD/Rader/hud_alt_outline.png", nullptr, &part.texture), L"Load texture - hud_alt_outline.png");
	//innerErrorCheck(CreateVertex(part.vertexBuffer, part.indexBuffer, part.indexCount, scale), L"Vertex/Index Create");
	#undef checkSize
	return S_OK;
}

HRESULT RaderUI::Awake(void)
{
	return E_NOTIMPL;
}

void RaderUI::FixedUpdate(void)
{
}

void RaderUI::Update(void)
{
}

void RaderUI::LateUpdate(void)
{
	RECT rect;
	GetWindowRect(hWnd, &rect);
	CenterPosition = { windowSizeX * -0.165f, windowSizeY * -0.195f };

	AddRenderObject(RenderType::UI, this);
}

void RaderUI::Render(void)
{
	static const UINT RDUIstride = sizeof(UIVertex);
	std::string diffuseTexture = "diffuseTexture";
	std::string world = "worldMatrix";

	Vector2 scale = Vector2::one() * 0.4f;
	Matrix worldMatrix;

	DirectX::XMStoreFloat4x4(&worldMatrix, CreateMatrix(CenterPosition, scale, 180.0f));
	SetMatrix(world, worldMatrix);

	SetTexture(diffuseTexture, HUDOutline.texture);
	GetCurrentShader()->Render(HUDOutline.indexBuffer, HUDOutline.vertexBuffer, RDUIstride);
	dxDeviceContext->DrawIndexed(HUDOutline.indexCount, 0, 0);

	SetTexture(diffuseTexture, RaderInnerLines.texture);
	GetCurrentShader()->Render(RaderInnerLines.indexBuffer, RaderInnerLines.vertexBuffer, RDUIstride);
	dxDeviceContext->DrawIndexed(RaderInnerLines.indexCount, 0, 0);

	//DirectX::XMStoreFloat4x4(&worldMatrix, CreateMatrix(CenterPosition, scale, 180.0f));
	//SetMatrix(world, worldMatrix);
	Vector2 position = CenterPosition;
	position.y += CenterPosition.y * 0.09f;
	DirectX::XMStoreFloat4x4(&worldMatrix, CreateMatrix(position, scale, 180.0f));
	SetMatrix(world, worldMatrix);

	SetTexture(diffuseTexture, HUDCircleS.texture);
	GetCurrentShader()->Render(HUDCircleS.indexBuffer, HUDCircleS.vertexBuffer, RDUIstride);
	dxDeviceContext->DrawIndexed(HUDCircleS.indexCount, 0, 0);

	SetTexture(diffuseTexture, HUDCircleM.texture);
	GetCurrentShader()->Render(HUDCircleM.indexBuffer, HUDCircleM.vertexBuffer, RDUIstride);
	dxDeviceContext->DrawIndexed(HUDCircleM.indexCount, 0, 0);

	position.y -= CenterPosition.y * 0.065f;
	scale.y = 0.35f;
	DirectX::XMStoreFloat4x4(&worldMatrix, CreateMatrix(position, scale, 180.0f));
	SetMatrix(world, worldMatrix);
	SetTexture(diffuseTexture, HUDCircleL.texture);
	GetCurrentShader()->Render(HUDCircleL.indexBuffer, HUDCircleL.vertexBuffer, RDUIstride);
	dxDeviceContext->DrawIndexed(HUDCircleL.indexCount, 0, 0);
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
	newVertex.texturePosition = { 1.0f, 0.75f };
	vertex.push_back(newVertex);

	newVertex.position = { scale.x / -2.0f, scale.y / 2.0f };
	newVertex.texturePosition = { 0.0f, 0.75f };
	vertex.push_back(newVertex);

	index.push_back(0);
	index.push_back(2);
	index.push_back(1);

	index.push_back(0);
	index.push_back(3);
	index.push_back(2);
}