#include "pch.h"
#include "AttitudeIndicator.h"
#include "RMWR.h"

AttitudeIndicator::AttitudeIndicator(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::UIObject(dxDevice, dxDeviceContext), transform(nullptr)
{
}

void AttitudeIndicator::Free(void)
{
	for (auto& part : parts)
	{
		part.second.Free();
	}
	PitchLevel.Free();
	PitchNegative.Free();
	PitchNegativeHalf.Free();
	PitchPositive.Free();
	PitchPositiveHalf.Free();
	PitchSide.Free();
}

AttitudeIndicator* AttitudeIndicator::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, Engine::Transform* transform)
{
	if (transform == nullptr)
		return nullptr;

	AttitudeIndicator* newInstance = new AttitudeIndicator(dxDevice, dxDeviceContext);
	newInstance->transform = transform;
	newInstance->warning = static_cast<RMWR*>(transform->GetOwner()->GetComponent(L"RMWR"))->LinkMissileWarning();
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

	innerErrorCheck(PitchLevel.LoadUITexture(L"../Bin/Resources/UI/HUD/hud_pitch_mark_center.png"), L"Load texture - hud_pitch_mark_center.png");
	CreateScale(PitchLevel.texture, scale);
	indicatorWidth = scale.x * 0.70f;
	innerErrorCheck(CreateVertex(PitchLevel.vertexBuffer, PitchLevel.indexBuffer, PitchLevel.indexCount, scale), L"Vertex/Index Create");
	//parts[PitchLevelName] = PitchLevel;

	innerErrorCheck(PitchNegative.LoadUITexture(L"../Bin/Resources/UI/HUD/hud_pitch_mark_neg.png"), L"Load texture - hud_pitch_mark_neg.png");
	innerErrorCheck(CreateVertex(PitchNegative.vertexBuffer, PitchNegative.indexBuffer, PitchNegative.indexCount, scale), L"Vertex/Index Create");
	//parts[PitchNegativeName] = PitchNegative;

	innerErrorCheck(PitchNegativeHalf.LoadUITexture(L"../Bin/Resources/UI/HUD/hud_pitch_mark_neg_half.png"), L"Load texture - hud_pitch_mark_neg_half.png");
	innerErrorCheck(CreateVertex(PitchNegativeHalf.vertexBuffer, PitchNegativeHalf.indexBuffer, PitchNegativeHalf.indexCount, scale), L"Vertex/Index Create");
	//parts[PitchNegativeHalfName] = PitchNegativeHalf;

	innerErrorCheck(PitchPositive.LoadUITexture(L"../Bin/Resources/UI/HUD/hud_pitch_mark_pos.png"), L"Load texture - hud_pitch_mark_pos.png");
	innerErrorCheck(CreateVertex(PitchPositive.vertexBuffer, PitchPositive.indexBuffer, PitchPositive.indexCount, scale), L"Vertex/Index Create");
	//parts[PitchPositiveName] = PitchPositive;

	innerErrorCheck(PitchPositiveHalf.LoadUITexture(L"../Bin/Resources/UI/HUD/hud_pitch_mark_pos_half.png"), L"Load texture - hud_pitch_mark_pos_half.png");
	innerErrorCheck(CreateVertex(PitchPositiveHalf.vertexBuffer, PitchPositiveHalf.indexBuffer, PitchPositiveHalf.indexCount, scale), L"Vertex/Index Create");
	//parts[PitchPositiveHalfName] = PitchPositiveHalf;

	innerErrorCheck(PitchSide.LoadUITexture(L"../Bin/Resources/UI/HUD/hud_pitch_mark_side.png"), L"Load texture - hud_pitch_mark_side.png");
	CreateScale(PitchSide.texture, scale); scale *= 1.2f;
	innerErrorCheck(CreateVertex(PitchSide.vertexBuffer, PitchSide.indexBuffer, PitchSide.indexCount, scale), L"Vertex/Index Create");
	//parts[PitchSideName] = PitchSide;
	indicatorDistance = 60.0f;
	
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
	static std::string diffuseTexture = "diffuseTexture", world = "worldMatrix";
	static Vector2 scale = Vector2::one();

	FLOAT PitchAdjustValue = transform->Angle().x / 2.5f;
	INT pitchLevel = static_cast<INT>(PitchAdjustValue);
	PitchAdjustValue = (PitchAdjustValue - static_cast<FLOAT>(pitchLevel)) * -1.0f;
	pitchLevel -= 2;

	Matrix worldMatrix;
	Vector2 position{ 0.0f, 0.0f };
	Engine::Shader* currentShader = GetCurrentShader();
	UIParts* part{ };
	float indicatorDistanceCalculated = indicatorDistance * (1.0f + (1.0f - std::abs(std::abs(transform->Angle().z) - 90.0f) / 90.0f * 0.9f)), calibrateValue = (windowSizeX / windowSizeY) * 2.0f;

	if (*warning)
		color = { 1.0f, 0.0f, 0.0f, 1.0f };
	else
		color = { 0.0f, 1.0f, 0.0f, 1.0f };

	currentShader->BindVariable("UIcolor", &color, sizeof(float4));

	for (INT i = 0; i < 5; i++)
	{
		DirectX::XMStoreFloat2(&position, DirectX::XMVector2TransformCoord(DirectX::XMVectorSet(0.0f, indicatorDistanceCalculated * static_cast<FLOAT>(i - 2) + (indicatorDistanceCalculated * PitchAdjustValue), 0.0f, 1.0f), DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(transform->Angle().z))));
		position.x /= calibrateValue;

		DirectX::XMStoreFloat4x4(&worldMatrix, CreateMatrix(position, scale, transform->Angle().z));
		::SetMatrix(world, worldMatrix);

		if ((pitchLevel + i) % 2 == 0)
		{
			if ((pitchLevel + i) == 0)
			{
				currentShader->BindTexture(diffuseTexture, PitchLevel.texture);
				currentShader->Render(PitchLevel.indexBuffer, PitchLevel.vertexBuffer, AIUIstride);
				dxDeviceContext->DrawIndexed(PitchLevel.indexCount, 0, 0);
			}
			else
			{
				if ((pitchLevel + i) < 0)
					part = &PitchNegative;
				else
					part = &PitchPositive;

				currentShader->BindTexture(diffuseTexture, part->texture);
				currentShader->Render(part->indexBuffer, part->vertexBuffer, AIUIstride);
				dxDeviceContext->DrawIndexed(part->indexCount, 0, 0);
			}
		}
		else
		{
			if ((pitchLevel + i) < 0)
				part = &PitchNegativeHalf;
			else
				part = &PitchPositiveHalf;

			currentShader->BindTexture(diffuseTexture, part->texture);
			currentShader->Render(part->indexBuffer, part->vertexBuffer, AIUIstride);
			dxDeviceContext->DrawIndexed(part->indexCount, 0, 0);
		}
	}

	scale.x = 1.0f;
	position.y = 0.0f;
	position.x = indicatorWidth;

	DirectX::XMStoreFloat4x4(&worldMatrix, CreateMatrix(position, scale, 0.0f));
	::SetMatrix(world, worldMatrix);

	currentShader->BindTexture(diffuseTexture, PitchSide.texture);
	currentShader->Render(PitchSide.indexBuffer, PitchSide.vertexBuffer, AIUIstride);
	dxDeviceContext->DrawIndexed(PitchSide.indexCount, 0, 0);


	position.x *= -1.0f;

	DirectX::XMStoreFloat4x4(&worldMatrix, CreateMatrix(position, scale, 180.0f));
	::SetMatrix(world, worldMatrix);

	currentShader->BindTexture(diffuseTexture, PitchSide.texture);
	currentShader->Render(PitchSide.indexBuffer, PitchSide.vertexBuffer, AIUIstride);
	dxDeviceContext->DrawIndexed(PitchSide.indexCount, 0, 0);

}
