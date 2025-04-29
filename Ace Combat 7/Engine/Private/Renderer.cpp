#include "Foundation.h"
#include "Renderer.h"
#include "GameObject.h"
#include "GameInstance.h"
#include "Camera.h"
#include "Shader.h"

using namespace Engine;

Renderer::Renderer(ID3D11Device* _dxDevice, ID3D11DeviceContext* _dxDeviceContext) : dxDevice(_dxDevice), dxDeviceContext(_dxDeviceContext)
{
}

void Renderer::Free(void)
{
	ClearRenderList();
}

Renderer* Renderer::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, const D3D11_VIEWPORT* viewPortInfomation)
{
	Renderer* newInstance = new Renderer(dxDevice, dxDeviceContext);
	newInstance->pipelineStatus.viewPortInfomation = viewPortInfomation;
	newInstance->uiShaderName = L"DefaultUIShader.hlsl";
	return newInstance;
}

void Renderer::AddRenderObject(RenderType type, GameObject* object)
{
	renderTargets[static_cast<int>(type)].push_back(object);
}

void Renderer::Render(void)
{
	BaseCameraSetting();
	PriorityRender();
	BaseCameraSetting();
	AlphaRender();
	NonAlphaRender();
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
	pipelineStatus.currentUIShader = GetShader(uiShaderName);
	SetShader(uiShaderName);
	Matrix viewProjectionMatrix;

	// 해상도 문제가 있음
	// 이걸 해결한 뒤(UI 재배치 등등) 다시 작업할 것
	DirectX::XMStoreFloat4x4(&viewProjectionMatrix, DirectX::XMMatrixOrthographicLH(pipelineStatus.viewPortInfomation->Width, pipelineStatus.viewPortInfomation->Height, 0.1f, 1.0f));
	//DirectX::XMStoreFloat4x4(&viewProjectionMatrix, DirectX::XMMatrixOrthographicLH(800.0f, 600.0f, 0.1f, 1.0f));
	SetMatrix("viewProjectionMatrix", viewProjectionMatrix);

	for (auto& gameObject : renderTargets[static_cast<int>(RenderType::UI)])
	{
		pipelineStatus.currentUIShader->SetShader();
		pipelineStatus.currentUIShader->ApplyShader();
		SetMatrix("viewProjectionMatrix", viewProjectionMatrix);
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
