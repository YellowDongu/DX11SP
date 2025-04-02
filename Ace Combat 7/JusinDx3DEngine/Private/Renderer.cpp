#include "Foundation.h"
#include "Renderer.h"
#include "GameObject.h"
#include "GameInstance.h"

using namespace Engine;

Renderer::Renderer(ID3D11Device* _dxDevice, ID3D11DeviceContext* _dxDeviceContext) : dxDevice(_dxDevice), dxDeviceContext(_dxDeviceContext)
{
}

void Renderer::Free(void)
{
	ClearRenderList();
}

Renderer* Renderer::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, DeviceInfomation& deviceInfo)
{
	Renderer* newInstance = new Renderer(dxDevice, dxDeviceContext);
	newInstance->resolution = { static_cast<FLOAT>(deviceInfo.viewpoirtWidth), static_cast<FLOAT>(deviceInfo.viewpoirtLength) };
	return newInstance;
}

void Renderer::AddRenderObject(RenderType type, GameObject* object)
{
	renderTargets[static_cast<int>(type)].push_back(object);
}

void Renderer::Render(void)
{
	BaseCameraSetting();
	AlphaRender();
	NonAlphaRender();
	PriorityRender();
	UIRender();
	
	ClearRenderList();
}

void Renderer::BaseCameraSetting(void)
{
	for (auto& gameObject : renderTargets[static_cast<int>(RenderType::Camera)])
	{
		gameObject->Render();
	}
}

void Renderer::AlphaRender(void)
{
	for (auto& gameObject : renderTargets[static_cast<int>(RenderType::Blend)])
	{
		gameObject->Render();
	}
}

void Renderer::NonAlphaRender(void)
{
	for (auto& gameObject : renderTargets[static_cast<int>(RenderType::NonBlend)])
	{
		gameObject->Render();
	}
}

void Renderer::PriorityRender(void)
{
	for (auto& gameObject : renderTargets[static_cast<int>(RenderType::Priority)])
	{
		gameObject->Render();
	}
}

void Renderer::UIRender(void)
{
	SetShader(L"DefaultUIShader.hlsl");
	GetCurrentShader()->SetShader();
	GetCurrentShader()->ApplyShader();
	Matrix viewProjectionMatrix;
	DirectX::XMStoreFloat4x4(&viewProjectionMatrix, DirectX::XMMatrixOrthographicLH(800.0f, 600.0f, 0.1f, 1.0f));
	SetMatrix("viewProjectionMatrix", viewProjectionMatrix);

	for (auto& gameObject : renderTargets[static_cast<int>(RenderType::UI)])
	{
		gameObject->Render();
	}
}

void Renderer::ClearRenderList(void)
{
	for (int i = 0; i < static_cast<int>(RenderType::End); i++)
	{
		renderTargets[i].clear();
	}
}

void Renderer::RemoveRenderObject(RenderType type, GameObject* object)
{
	int listNumber = static_cast<int>(type);
	for (auto iterator = renderTargets[listNumber].begin(); iterator != renderTargets[listNumber].end(); )
	{
		if ((*iterator) == object)
		{
			iterator = renderTargets[listNumber].erase(iterator);
		}
		else
			iterator++;
	}
}

void Renderer::RemoveRenderObject(GameObject* object)
{
	for (int i = 0; i < static_cast<int>(RenderType::End); i++)
	{
		RemoveRenderObject(static_cast<RenderType>(i), object);
	}
}
