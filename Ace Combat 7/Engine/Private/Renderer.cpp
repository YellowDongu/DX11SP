#include "Foundation.h"
#include "Renderer.h"
#include "GameObject.h"
#include "GameInstance.h"
#include "Camera.h"
#include "DefaultDeferredShader.h"
#include "RectanglePolygon.h"
#include "Transform.h"
#include "DefaultUIShader.h"

using namespace Engine;

Renderer::Renderer(ID3D11Device* _dxDevice, ID3D11DeviceContext* _dxDeviceContext) : dxDevice(_dxDevice), dxDeviceContext(_dxDeviceContext)
{
}

void Renderer::Free(void)
{
	ClearRenderList();
	Base::DestroyInstance(deferredBuffer);
}

Renderer* Renderer::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, const D3D11_VIEWPORT* viewPortInfomation)
{
	Renderer* newInstance = new Renderer(dxDevice, dxDeviceContext);
	newInstance->pipelineStatus.viewPortInfomation = viewPortInfomation;
	newInstance->uiShaderName = L"DefaultUIShader.hlsl";
	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}

	return newInstance;
}

HRESULT Renderer::Start(void)
{
	RenderTarget::RenderTargetInfomation targetInfomation;
	targetInfomation.clearColor = float4(1.0f, 1.0f, 1.0f, 0.0f);
	targetInfomation.pixelFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	targetInfomation.xSize = static_cast<UINT>(pipelineStatus.viewPortInfomation->Width);
	targetInfomation.ySize = static_cast<UINT>(pipelineStatus.viewPortInfomation->Height);

	if (FAILED(Device()->AddRenderTargets(RenderTargetType::Diffuse, targetInfomation)))
		return E_FAIL;
	
	targetInfomation.clearColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
	targetInfomation.pixelFormat = DXGI_FORMAT_R16G16B16A16_UNORM;
	if (FAILED(Device()->AddRenderTargets(RenderTargetType::Normal, targetInfomation)))
		return E_FAIL;

	targetInfomation.clearColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	if (FAILED(Device()->AddRenderTargets(RenderTargetType::Shade, targetInfomation)))
		return E_FAIL;
	if (FAILED(Device()->AddRenderTargets(RenderTargetType::Specular, targetInfomation)))
		return E_FAIL;
	if (FAILED(Device()->AddRenderTargets(RenderTargetType::MaterialSpecular, targetInfomation)))
		return E_FAIL;

	targetInfomation.pixelFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
	if (FAILED(Device()->AddRenderTargets(RenderTargetType::Depth, targetInfomation)))
		return E_FAIL;

	if (FAILED(Device()->BindMultiRenderTarget(DeferredRenderOrder::Object, RenderTargetType::Diffuse)))
		return E_FAIL;
	if (FAILED(Device()->BindMultiRenderTarget(DeferredRenderOrder::Object, RenderTargetType::Normal)))
		return E_FAIL;
	if (FAILED(Device()->BindMultiRenderTarget(DeferredRenderOrder::Object, RenderTargetType::Depth)))
		return E_FAIL;
	if (FAILED(Device()->BindMultiRenderTarget(DeferredRenderOrder::Object, RenderTargetType::MaterialSpecular)))
		return E_FAIL;
	if (FAILED(Device()->BindMultiRenderTarget(DeferredRenderOrder::Shade, RenderTargetType::Shade)))
		return E_FAIL;
	if (FAILED(Device()->BindMultiRenderTarget(DeferredRenderOrder::Shade, RenderTargetType::Specular)))
		return E_FAIL;

	DirectX::XMStoreFloat4x4(&worldMatrix, DirectX::XMMatrixScaling(pipelineStatus.viewPortInfomation->Width, pipelineStatus.viewPortInfomation->Height, 1.0f) * DirectX::XMMatrixTranslation(0.0f, 0.0f, 1.0f));
	DirectX::XMStoreFloat4x4(&viewMatrix, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&pipelineStatus.orthoProjectionMatrix, DirectX::XMMatrixOrthographicLH(pipelineStatus.viewPortInfomation->Width, pipelineStatus.viewPortInfomation->Height, 0.1f, 1.0f));
	width = pipelineStatus.viewPortInfomation->Width;
	height = pipelineStatus.viewPortInfomation->Height;

	//DirectX::XMStoreFloat4x4(&viewProjectionMatrix, DirectX::XMMatrixOrthographicLH(800.0f, 600.0f, 0.1f, 1.0f));
	//SetMatrix("viewProjectionMatrix", pipelineStatus.orthoProjectionMatrix);
	deferredShader = DefaultDeferredShader::Create(dxDevice, dxDeviceContext);
	if (deferredShader == nullptr)
		return E_FAIL;

	::AddShader(deferredShader->shaderFile, deferredShader);
	deferredBuffer = RectanglePolygon::Create(dxDevice, dxDeviceContext);
	if (deferredBuffer == nullptr)
		return E_FAIL;

	return S_OK;
}

void Renderer::AddRenderObject(RenderType type, GameObject* object)
{
	renderTargets[static_cast<int>(type)].push_back(object);
}

void Renderer::Render(void)
{
	if (width != pipelineStatus.viewPortInfomation->Width || height != pipelineStatus.viewPortInfomation->Height)
	{
		DirectX::XMStoreFloat4x4(&worldMatrix, DirectX::XMMatrixScaling(pipelineStatus.viewPortInfomation->Width, pipelineStatus.viewPortInfomation->Height, 1.f) * DirectX::XMMatrixTranslation(0.0f, 0.0f, 1.0f));
		DirectX::XMStoreFloat4x4(&pipelineStatus.orthoProjectionMatrix, DirectX::XMMatrixOrthographicLH(pipelineStatus.viewPortInfomation->Width, pipelineStatus.viewPortInfomation->Height, 0.1f, 1.0f));
		width = pipelineStatus.viewPortInfomation->Width;
		height = pipelineStatus.viewPortInfomation->Height;
	}
	if (skip)
	{
		skip = false;
		ClearRenderList();
		return;
	}

	BaseCameraSetting();
	PriorityRender();

	NonAlphaRender();
	ShaderRender();
	DeferredRender();

	NonAlphaNonLightRender();
	AlphaRender();
	EffectRender();
	UIRender();

	ClearRenderList();
}

void Renderer::BaseCameraSetting(void)
{
	for (auto& gameObject : renderTargets[static_cast<int>(RenderType::Camera)])
	{
		gameObject->Render();
	}
	pipelineStatus.currnetCamera->Render();
}

void Renderer::AlphaRender(void)
{
	for (auto& gameObject : renderTargets[static_cast<int>(RenderType::Blend)])
	{
		gameObject->Render();
	}
}

void Renderer::EffectRender(void)
{
	for (auto& gameObject : renderTargets[static_cast<int>(RenderType::Effect)])
	{
		gameObject->Render();
	}
}

void Renderer::NonAlphaRender(void)
{
	Device()->SetMultiRenderTarget(DeferredRenderOrder::Object);

	for (auto& gameObject : renderTargets[static_cast<int>(RenderType::NonBlend)])
	{
		gameObject->Render();
	}
}

void Renderer::ShaderRender(void)
{
	Device()->SetMultiRenderTarget(DeferredRenderOrder::Shade);
	static std::string worldMatrixName = "worldMatrix";
	static std::string viewProjectionMatrixName = "viewProjectionMatrix";

	static std::string normalTextureName = "normalTexture";
	static std::string depthTextureName = "depthTexture";
	static std::string materialSpecularTextureName = "materialSpecularTexture";
	static std::string cameraPosition = "cameraPosition";
	static std::string viewProjectionMatrixInversed = "viewProjectionMatrixInversed";

	Shader* appliedShader = pipelineStatus.currentShader;
	pipelineStatus.currentShader = deferredShader;
	DirectX::XMFLOAT3 position = static_cast<DirectX::XMFLOAT3>(pipelineStatus.currnetCamera->GetOwner()->transform()->Position());
	Matrix inversedViewProjectionMatrix;
	DirectX::XMStoreFloat4x4(&inversedViewProjectionMatrix, DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&pipelineStatus.viewProjectionMatrix)));
	deferredShader->BindVariable(cameraPosition,&position, sizeof(DirectX::XMFLOAT3));
	deferredShader->BindMatrix(viewProjectionMatrixInversed, inversedViewProjectionMatrix);

	deferredShader->BindMatrix(worldMatrixName, worldMatrix);
	deferredShader->BindMatrix(viewProjectionMatrixName, pipelineStatus.orthoProjectionMatrix);

	RenderTarget* renderTarget = Device()->GetRenderTarget(RenderTargetType::Normal);
	if (renderTarget == nullptr)
		return;
	deferredShader->BindTexture(normalTextureName, renderTarget->GetShaderResourceView());
	renderTarget = Device()->GetRenderTarget(RenderTargetType::Depth);
	if (renderTarget == nullptr)
		return;
	deferredShader->BindTexture(depthTextureName, renderTarget->GetShaderResourceView());
	renderTarget = Device()->GetRenderTarget(RenderTargetType::MaterialSpecular);
	if (renderTarget == nullptr)
		return;
	deferredShader->BindTexture(materialSpecularTextureName, renderTarget->GetShaderResourceView());


	if (FAILED(deferredShader->SetShader()))
		return;
	if (FAILED(deferredShader->ApplyShader()))
		return;
	deferredShader->PassNumber(1);

	deferredBuffer->Render(deferredShader);

	deferredShader->BindTexture(normalTextureName, nullptr);
	deferredShader->BindTexture(depthTextureName, nullptr);
	deferredShader->BindTexture(materialSpecularTextureName, nullptr);

	pipelineStatus.currentShader = appliedShader;

	if (FAILED(pipelineStatus.currentShader->SetShader()))
		return;
	if (FAILED(pipelineStatus.currentShader->ApplyShader()))
		return;



}

void Renderer::DeferredRender(void)
{
	Device()->ApplyBackBuffer();

	static std::string worldMatrixName = "worldMatrix";
	static std::string viewProjectionMatrixName = "viewProjectionMatrix";

	static std::string diffuseTextureName = "diffuseTexture";
	static std::string shadeTextureName = "shadeTexture";
	static std::string specularTextureName = "specularTexture";
	static std::string depthTextureName = "depthTexture";

	Shader* appliedShader = pipelineStatus.currentShader;
	pipelineStatus.currentShader = deferredShader;

	FLOAT farZ = pipelineStatus.currnetCamera->FarZ();
	deferredShader->BindVariable("farZ", &farZ, sizeof(FLOAT));
	deferredShader->BindMatrix(worldMatrixName, worldMatrix);
	deferredShader->BindMatrix(viewProjectionMatrixName, pipelineStatus.orthoProjectionMatrix);

	RenderTarget* renderTarget = Device()->GetRenderTarget(RenderTargetType::Diffuse);
	if (renderTarget == nullptr)
		return;
	deferredShader->BindTexture(diffuseTextureName, renderTarget->GetShaderResourceView());

	renderTarget = Device()->GetRenderTarget(RenderTargetType::Shade);
	if (renderTarget == nullptr)
		return;
	deferredShader->BindTexture(shadeTextureName, renderTarget->GetShaderResourceView());

	renderTarget = Device()->GetRenderTarget(RenderTargetType::Specular);
	if (renderTarget == nullptr)
		return;
	deferredShader->BindTexture(specularTextureName, renderTarget->GetShaderResourceView());

	renderTarget = Device()->GetRenderTarget(RenderTargetType::Depth);
	if (renderTarget == nullptr)
		return;
	deferredShader->BindTexture(depthTextureName, renderTarget->GetShaderResourceView());


	if (FAILED(deferredShader->SetShader()))
		return;
	if (FAILED(deferredShader->ApplyShader()))
		return;
	deferredShader->PassNumber(0);

	deferredBuffer->Render(deferredShader);

	deferredShader->BindTexture(diffuseTextureName, nullptr);
	deferredShader->BindTexture(shadeTextureName, nullptr);
	deferredShader->BindTexture(specularTextureName, nullptr);
	deferredShader->BindTexture(depthTextureName, nullptr);

	pipelineStatus.currentShader = appliedShader;

	pipelineStatus.currentShader->BindMatrix(pipelineStatus.currentShader->worldMatrixA, pipelineStatus.worldMatrix);
	pipelineStatus.currentShader->BindMatrix(pipelineStatus.currentShader->viewProjectionMatrixA, pipelineStatus.viewProjectionMatrix);

	if (FAILED(pipelineStatus.currentShader->SetShader()))
		return;
	if (FAILED(pipelineStatus.currentShader->ApplyShader()))
		return;

}

void Renderer::NonAlphaNonLightRender(void)
{
	for (auto& gameObject : renderTargets[static_cast<int>(RenderType::NonBlendNonLight)])
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
	if (pipelineStatus.currentUIShader == nullptr)
	{
		pipelineStatus.currentUIShader = GetShader(uiShaderName);
		if (pipelineStatus.currentUIShader == nullptr)
		{
			DefaultUIShader* newShader = DefaultUIShader::Create(dxDevice, dxDeviceContext);
			if(newShader == nullptr)
				return;

			AddShader(newShader->shaderFile, newShader);
			pipelineStatus.currentUIShader = newShader;
		}
	}
	Shader* oldShader = pipelineStatus.currentShader;
	SetShader(pipelineStatus.currentUIShader);


	for (auto& gameObject : renderTargets[static_cast<int>(RenderType::UI)])
	{
		SetMatrix(pipelineStatus.currentUIShader->viewProjectionMatrixA, pipelineStatus.orthoProjectionMatrix);

		pipelineStatus.currentUIShader->SetShader();
		pipelineStatus.currentUIShader->ApplyShader();
		gameObject->Render();
	}
	SetShader(oldShader);
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

HRESULT Renderer::BindCamera(Camera* camera)
{
	if (camera == nullptr)
		return E_FAIL;

	pipelineStatus.currnetCamera = camera;
	return S_OK;
}

HRESULT Renderer::BindVariable(const std::string& variableName, void* variable, size_t variableSize)
{
	if (pipelineStatus.currentShader == nullptr)
		return E_FAIL;
	return pipelineStatus.currentShader->BindVariable(variableName, variable, static_cast<UINT>(variableSize));
}

HRESULT Renderer::BindVariable(char* variableName, void* variable, size_t variableSize)
{
	if (pipelineStatus.currentShader == nullptr)
		return E_FAIL;
	return pipelineStatus.currentShader->BindVariable(variableName, variable, static_cast<UINT>(variableSize));
}

HRESULT Renderer::BindMatrix(const std::string& variableName, const Matrix& matrix)
{
	if (pipelineStatus.currentShader == nullptr)
		return E_FAIL;
	return pipelineStatus.currentShader->BindMatrix(variableName, matrix);
}

HRESULT Renderer::BindMatrix(char* variableName, const Matrix& matrix)
{
	if (pipelineStatus.currentShader == nullptr)
		return E_FAIL;
	return pipelineStatus.currentShader->BindMatrix(variableName, matrix);
}


HRESULT Renderer::BindWorldMatrix(const Matrix& matrix)
{
	if (pipelineStatus.currentShader == nullptr)
		return E_FAIL;
	return E_NOTIMPL;
}

HRESULT Renderer::BindViewProjectionMatrix(const Matrix& viewMatrix, const Matrix& projectionMatrix)
{
	if (pipelineStatus.currentShader == nullptr)
		return E_FAIL;

	memcpy(&pipelineStatus.viewMatrix, &viewMatrix, sizeof(Matrix));
	memcpy(&pipelineStatus.projectionMatrix, &projectionMatrix, sizeof(Matrix));
	DirectX::XMStoreFloat4x4(&pipelineStatus.viewProjectionMatrix, DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&viewMatrix), DirectX::XMLoadFloat4x4(&projectionMatrix)));

	return pipelineStatus.currentShader->BindMatrix(pipelineStatus.currentShader->viewProjectionMatrixA, pipelineStatus.viewProjectionMatrix);
}

HRESULT Renderer::BindMatrix(const std::string& variableName, fxmMatrix matrix)
{
	if (pipelineStatus.currentShader == nullptr)
		return E_FAIL;

	Matrix convertexdMatrix;
	DirectX::XMStoreFloat4x4(&convertexdMatrix, matrix);

	return pipelineStatus.currentShader->BindMatrix(variableName, convertexdMatrix);
}

HRESULT Renderer::BindMatrix(char* variableName, fxmMatrix matrix)
{
	if (pipelineStatus.currentShader == nullptr)
		return E_FAIL;

	Matrix convertexdMatrix;
	DirectX::XMStoreFloat4x4(&convertexdMatrix, matrix);

	return pipelineStatus.currentShader->BindMatrix(variableName, convertexdMatrix);
}

HRESULT Engine::Renderer::BindWorldMatrix(fxmMatrix matrix)
{
	if (pipelineStatus.currentShader == nullptr)
		return E_FAIL;

	Matrix convertexdMatrix;
	DirectX::XMStoreFloat4x4(&convertexdMatrix, matrix);

	return pipelineStatus.currentShader->BindMatrix(pipelineStatus.currentShader->worldMatrixA, convertexdMatrix);
}

HRESULT Renderer::BindViewProjectionMatrix(fxmMatrix viewMatrix, fxmMatrix projectionMatrix)
{
	if (pipelineStatus.currentShader == nullptr)
		return E_FAIL;

	DirectX::XMStoreFloat4x4(&pipelineStatus.viewMatrix, viewMatrix);
	DirectX::XMStoreFloat4x4(&pipelineStatus.projectionMatrix, projectionMatrix);
	DirectX::XMStoreFloat4x4(&pipelineStatus.viewProjectionMatrix, DirectX::XMMatrixMultiply(viewMatrix, projectionMatrix));

	return pipelineStatus.currentShader->BindMatrix(pipelineStatus.currentShader->viewProjectionMatrixA, pipelineStatus.viewProjectionMatrix);
}

HRESULT Renderer::SetShader(Shader* shader)
{
	if (pipelineStatus.currentShader == shader)
		return S_OK;

	pipelineStatus.currentShader = shader;

	pipelineStatus.currentShader->BindMatrix(pipelineStatus.currentShader->worldMatrixA, pipelineStatus.worldMatrix);
	pipelineStatus.currentShader->BindMatrix(pipelineStatus.currentShader->viewProjectionMatrixA, pipelineStatus.viewProjectionMatrix);

	if (FAILED(pipelineStatus.currentShader->SetShader()))
		return E_FAIL;
	if (FAILED(pipelineStatus.currentShader->ApplyShader()))
		return E_FAIL;

	return S_OK;
}
