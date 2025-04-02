#include "pch.h"
#include "AttitudeIndicator.h"

AttitudeIndicator::AttitudeIndicator(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::UIObject(dxDevice, dxDeviceContext), transform(nullptr)
{
}

void AttitudeIndicator::Free(void)
{
	for (auto& part : parts)
	{
		part.second.Free();
	}
}

AttitudeIndicator* AttitudeIndicator::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, Engine::Transform* transform)
{
	if (transform == nullptr)
		return nullptr;

	AttitudeIndicator* newInstance = new AttitudeIndicator(dxDevice, dxDeviceContext);
	newInstance->transform = transform;
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
	ErrMsg((std::wstring(L"AttitudeIndicator::Failed : ") + errmsg).c_str()); \
	return E_FAIL;										   \
}														   \


HRESULT AttitudeIndicator::Start(void)
{
	Vector2 scale;
	//Vector2 scale = { 353.0f, 7.0f }; // Vector2 scale = { 88.25f, 3.5f }; //Vector2 scale = { 176.5f, 3.5f };

	UIParts part;
	innerErrorCheck(part.LoadUITexture(L"../Bin/Resources/UI/HUD/hud_pitch_mark_center.png"), L"Load texture - hud_pitch_mark_center.png");
	CreateScale(part.texture, scale); scale /= 2; scale.x /= 2;
	indicatorWidth = scale.x * 0.70f;
	innerErrorCheck(CreateVertex(part.vertexBuffer, part.indexBuffer, part.indexCount, scale), L"Vertex/Index Create");
	parts[PitchLevel] = part;

	innerErrorCheck(part.LoadUITexture(L"../Bin/Resources/UI/HUD/hud_pitch_mark_neg.png"), L"Load texture - hud_pitch_mark_neg.png");
	innerErrorCheck(CreateVertex(part.vertexBuffer, part.indexBuffer, part.indexCount, scale), L"Vertex/Index Create");
	parts[PitchNegative] = part;

	innerErrorCheck(part.LoadUITexture(L"../Bin/Resources/UI/HUD/hud_pitch_mark_neg_half.png"), L"Load texture - hud_pitch_mark_neg_half.png");
	innerErrorCheck(CreateVertex(part.vertexBuffer, part.indexBuffer, part.indexCount, scale), L"Vertex/Index Create");
	parts[PitchNegativeHalf] = part;

	innerErrorCheck(part.LoadUITexture(L"../Bin/Resources/UI/HUD/hud_pitch_mark_pos.png"), L"Load texture - hud_pitch_mark_pos.png");
	innerErrorCheck(CreateVertex(part.vertexBuffer, part.indexBuffer, part.indexCount, scale), L"Vertex/Index Create");
	parts[PitchPositive] = part;

	innerErrorCheck(part.LoadUITexture(L"../Bin/Resources/UI/HUD/hud_pitch_mark_pos_half.png"), L"Load texture - hud_pitch_mark_pos_half.png");
	innerErrorCheck(CreateVertex(part.vertexBuffer, part.indexBuffer, part.indexCount, scale), L"Vertex/Index Create");
	parts[PitchPositiveHalf] = part;

	//scale = { 16.0f, 318.0f }; scale = { 8.0f, 159.0f };
	innerErrorCheck(part.LoadUITexture(L"../Bin/Resources/UI/HUD/hud_pitch_mark_side.png"), L"Load texture - hud_pitch_mark_side.png");
	CreateScale(part.texture, scale); scale /= 2;
	innerErrorCheck(CreateVertex(part.vertexBuffer, part.indexBuffer, part.indexCount, scale), L"Vertex/Index Create");
	parts[PitchSide] = part;
	return S_OK;
}
#undef innerErrorCheck

HRESULT AttitudeIndicator::Awake(void)
{
	// for clone
	//for (auto& part : parts)
	//{
	//	part.second.texture->AddRef();
	//	part.second.indexBuffer->AddRef();
	//	part.second.vertexBuffer->AddRef();
	//}
	return S_OK;
}

void AttitudeIndicator::FixedUpdate(void)
{
}

void AttitudeIndicator::Update(void)
{
}

void AttitudeIndicator::LateUpdate(void)
{
	AddRenderObject(RenderType::UI, this);
}

void AttitudeIndicator::Render(void)
{
	static const UINT AIUIstride = sizeof(UIVertex);
	FLOAT PitchAdjustValue = transform->Angle().x / 2.5f;
	INT pitchLevel = static_cast<INT>(PitchAdjustValue);
	PitchAdjustValue = abs(static_cast<FLOAT>(pitchLevel) - PitchAdjustValue) * -1.0f;
	UIParts* part = nullptr;
	pitchLevel -= 2;
	Vector2 scale = Vector2::one();
	Vector2 position = Vector2::zero();
	Matrix worldMatrix;
	static std::string diffuseTexture = "diffuseTexture";
	std::string& world = GetCurrentShader()->worldMatrixA;



	for (INT i = 0; i < 5; i++)
	{
		DirectX::XMStoreFloat2(&position, DirectX::XMVector2TransformCoord(
			DirectX::XMVectorSet(0.0f, indicatorDistance * static_cast<FLOAT>(i - 2) + (indicatorDistance * PitchAdjustValue), 0.0f, 1.0f), 
			DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(transform->Angle().z))));
		
		DirectX::XMStoreFloat4x4(&worldMatrix, CreateMatrix(position, scale, transform->Angle().z));
		SetMatrix(world, worldMatrix);

		if ((pitchLevel + i) % 2 == 0)
		{
			if ((pitchLevel + i) == 0)
			{
				part = &parts.find(PitchLevel)->second;
				SetTexture(diffuseTexture, part->texture);
				GetCurrentShader()->Render(part->indexBuffer, part->vertexBuffer, AIUIstride);
				dxDeviceContext->DrawIndexed(part->indexCount, 0, 0);
			}
			else
			{
				if ((pitchLevel + i) < 0)
					part = &parts.find(PitchNegative)->second;
				else
					part = &parts.find(PitchPositive)->second;

				SetTexture(diffuseTexture, part->texture);
				GetCurrentShader()->Render(part->indexBuffer, part->vertexBuffer, AIUIstride);
				dxDeviceContext->DrawIndexed(part->indexCount, 0, 0);
			}
		}
		else
		{
			if ((pitchLevel + i) < 0)
				part = &parts.find(PitchNegativeHalf)->second;
			else
				part = &parts.find(PitchPositiveHalf)->second;

			SetTexture(diffuseTexture, part->texture);
			GetCurrentShader()->Render(part->indexBuffer, part->vertexBuffer, AIUIstride);
			dxDeviceContext->DrawIndexed(part->indexCount, 0, 0);
		}
	}

	position.y = 0.0f;
	position.x = indicatorWidth;

	DirectX::XMStoreFloat4x4(&worldMatrix, CreateMatrix(position, scale, 0.0f));
	SetMatrix(world, worldMatrix);

	part = &parts.find(PitchSide)->second;
	SetTexture(diffuseTexture, part->texture);
	GetCurrentShader()->Render(part->indexBuffer, part->vertexBuffer, AIUIstride);
	dxDeviceContext->DrawIndexed(part->indexCount, 0, 0);


	position.x *= -1.0f;

	DirectX::XMStoreFloat4x4(&worldMatrix, CreateMatrix(position, scale, 180.0f));
	SetMatrix(world, worldMatrix);

	part = &parts.find(PitchSide)->second;
	SetTexture(diffuseTexture, part->texture);
	GetCurrentShader()->Render(part->indexBuffer, part->vertexBuffer, AIUIstride);
	dxDeviceContext->DrawIndexed(part->indexCount, 0, 0);

}
