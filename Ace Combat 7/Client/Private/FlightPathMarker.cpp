#include "pch.h"
#include "FlightPathMarker.h"
#include "FlightMovement.h"

FlightPathMarker::FlightPathMarker(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::UIObject(dxDevice, dxDeviceContext)
{
}

void FlightPathMarker::Free(void)
{
	pathMarker.Free();
	centerMarker.Free();
}

FlightPathMarker* FlightPathMarker::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, Engine::GameObject* player)
{
	FlightPathMarker* newInstance = new FlightPathMarker(dxDevice, dxDeviceContext);
	newInstance->LinkInfomation(static_cast<FlightMovement*>(player->GetComponent(L"FlightMovement")));
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


HRESULT FlightPathMarker::Start(void)
{
	Vector2 scale = { 52.0f, 38.0f };

	innerErrorCheck(pathMarker.LoadUITexture(L"../Bin/Resources/UI/HUD/velocityVector.png"), L"Load texture - velocityVector.png");
	CreateScale(pathMarker.texture, scale);
	innerErrorCheck(CreateVertex(pathMarker.vertexBuffer, pathMarker.indexBuffer, pathMarker.indexCount, scale), L"Vertex/Index Create");

	innerErrorCheck(centerMarker.LoadUITexture(L"../Bin/Resources/UI/HUD/whiskyMark.png"), L"Load texture - whiskyMark.png");
	CreateScale(centerMarker.texture, scale);
	innerErrorCheck(CreateVertex(centerMarker.vertexBuffer, centerMarker.indexBuffer, centerMarker.indexCount, scale), L"Vertex/Index Create");
	return S_OK;
}

HRESULT FlightPathMarker::Awake(void)
{
	return S_OK;
}
#undef innerErrorCheck


void FlightPathMarker::FixedUpdate(void)
{
}

void FlightPathMarker::Update(void)
{
}

void FlightPathMarker::LateUpdate(void)
{
	if (forward == nullptr || force == nullptr)
		return;

	Vector3 heading = *forward - force->normalize();

	if (path.y != heading.y * 50.0f)
	{
		int alarm = 0;
	}


	path.x = heading.x * -1.0f;
	path.y = heading.y * -1.0f;
	path *= 50.0f;
	goingForward = true;

	AddRenderObject(RenderType::UI, this);
}

void FlightPathMarker::Render(void)
{
	static const UINT FPMUIstride = sizeof(UIVertex);
	std::string diffuseTexture = "diffuseTexture";
	std::string world = "worldMatrix";

	Vector2 position = Vector2::zero();
	Vector2 scale = Vector2::one();
	Matrix worldMatrix;

	DirectX::XMStoreFloat4x4(&worldMatrix, CreateMatrix(position, scale, 180.0f));
	SetMatrix(world, worldMatrix);

	SetTexture(diffuseTexture, centerMarker.texture);
	GetCurrentShader()->Render(centerMarker.indexBuffer, centerMarker.vertexBuffer, FPMUIstride);
	dxDeviceContext->DrawIndexed(centerMarker.indexCount, 0, 0);

	if (!goingForward)
		return;
	position = path;
	DirectX::XMStoreFloat4x4(&worldMatrix, CreateMatrix(position, scale, 180.0f));
	SetMatrix(world, worldMatrix);

	SetTexture(diffuseTexture, pathMarker.texture);
	GetCurrentShader()->Render(pathMarker.indexBuffer, pathMarker.vertexBuffer, FPMUIstride);
	dxDeviceContext->DrawIndexed(pathMarker.indexCount, 0, 0);
}

void FlightPathMarker::LinkInfomation(FlightMovement* playerFlightModule)
{
	force = playerFlightModule->LinkFlightVector();
	forward = &playerFlightModule->LinkTransform()->Forward();
}
