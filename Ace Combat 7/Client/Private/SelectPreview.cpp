#include "pch.h"
#include "SelectPreview.h"
#include "MainCamera.h"
SelectPreview::SelectPreview(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::GameObject(dxDevice, dxDeviceContext)
{
}

SelectPreview::SelectPreview(const SelectPreview& other) : Engine::GameObject(other)
{
}

void SelectPreview::Free(void)
{
	Base::DestroyInstance(hanger);

	for (auto& model : models)
	{
		Base::DestroyInstance(model.second.first);
		Base::DestroyInstance(model.second.second);
	}
	models.clear();

	currentDisplayingModel = nullptr;
	Engine::GameObject::Free();
}

SelectPreview* SelectPreview::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
	SelectPreview* newInstance = new SelectPreview(dxDevice, dxDeviceContext);

	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}

	return newInstance;
}

Engine::GameObject* SelectPreview::Clone(void)
{
	return new SelectPreview(*this);
}

HRESULT SelectPreview::Start(void)
{
	if (FAILED(CreateTransform()))
		return E_FAIL;

	xmMatrix offsetMatrix = DirectX::XMMatrixIdentity();
	::ConvertModel("../Bin/Resources/Environment/HangarMaintenance/Hanger.FBX", L"../Bin/Resources/Environment/HangarMaintenance/Hanger.model", offsetMatrix, true);

	offsetMatrix = DirectX::XMMatrixRotationRollPitchYaw(0.0f, DirectX::XMConvertToRadians(90.0f), DirectX::XMConvertToRadians(-90.0f));
	//Matrix offsetMatrix;
	//DirectX::XMStoreFloat4x4(&offsetMatrix, DirectX::XMMatrixIdentity());
	//::ConvertModel("../Bin/Resources/Environment/HangarMaintenance/Hanger.FBX", L"../Bin/Resources/Environment/HangarMaintenance/Hanger.model", offsetMatrix);

	if (FAILED(::LoadStaticModel(L"../Bin/Resources/Environment/HangarMaintenance/Hanger.model", hanger)))
		return E_FAIL;

	Engine::StaticModel* body = nullptr, * gear = nullptr;
	AircraftMetaData tempralData;
	{
		tempralData = F15EMetaData();
		::LoadStaticModel(tempralData.modelFilePath, body);
		::LoadStaticModel(tempralData.gearModelfilePath, gear);
		body->SetOffsetMatrix(offsetMatrix);
		gear->SetOffsetMatrix(offsetMatrix);
		models[tempralData.AircraftModelName] = { body, gear };
	}
	{
		tempralData = F14DMetaData();
		::LoadStaticModel(tempralData.modelFilePath, body);
		::LoadStaticModel(tempralData.gearModelfilePath, gear);
		body->SetOffsetMatrix(offsetMatrix);
		gear->SetOffsetMatrix(offsetMatrix);
		models[tempralData.AircraftModelName] = { body, gear };
	}
	{
		tempralData = F16CMetaData();
		::LoadStaticModel(tempralData.modelFilePath, body);
		::LoadStaticModel(tempralData.gearModelfilePath, gear);
		body->SetOffsetMatrix(offsetMatrix);
		gear->SetOffsetMatrix(offsetMatrix);
		models[tempralData.AircraftModelName] = { body, gear };
	}


	return S_OK;
}

HRESULT SelectPreview::Awake(void)
{
	SetActive(false);
	currentDisplayingModel = &models.find(L"F15E")->second;
	transformComponent->Scale() = Vector3::one() * 0.5f;
	transformComponent->Position() = Vector3{ 0.0f, 14.0f, -25.0f };

	return S_OK;
}

void SelectPreview::Update(void)
{
	if(!active)
		return;

	if (mainCamera == nullptr)
	{
		Vector3 finalCameraPosition;

		mainCamera = static_cast<MainCamera*>(::EngineInstance()->SceneManager()->LoadingScene()->FindLayer(L"BaseLayer")->GetGameObject(L"MainCamera"));
		DirectX::XMStoreFloat3(&finalCameraPosition, DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(0.0f, 0.0f, -75.0f, 1.0f), DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(viewAngle))));
		finalCameraPosition.y = 40.0f;
		mainCamera->transform()->Position() = finalCameraPosition;
		mainCamera->transform()->SetAngle(20.0f, viewAngle, 0.0f);
		mainCamera->transform()->UpdateWorldMatrix();
	}

	FLOAT cameraInput = static_cast<FLOAT>(Input()->getButton(KeyType::A)) - static_cast<FLOAT>(Input()->getButton(KeyType::D));
	if (cameraInput != 0.0f)
	{
		Vector3 finalCameraPosition;

		viewAngle += cameraInput;
		DirectX::XMStoreFloat3(&finalCameraPosition, DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(0.0f, 0.0f, -75.0f, 1.0f), DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(viewAngle))));
		finalCameraPosition.y = 40.0f;
		mainCamera->transform()->Position() = finalCameraPosition;
		mainCamera->transform()->SetAngle(20.0f, viewAngle, 0.0f);
		mainCamera->transform()->UpdateWorldMatrix();
		//mainCamera->transform()->Position() = Vector3{ 0.0f, 40.0f, 0.0f };
	}


	if (Input()->getButtonDown(KeyType::S) || Input()->getButtonDown(KeyType::DOWN))
	{
		displayingModelIndex++;
		ChangeModel(displayingModelIndex);
	}
	if (Input()->getButtonDown(KeyType::W) || Input()->getButtonDown(KeyType::UP))
	{
		if(displayingModelIndex != 0)
			displayingModelIndex--;
		ChangeModel(displayingModelIndex);
	}


}

void SelectPreview::LateUpdate(void)
{
	if (!active)
		return;

	::AddRenderObject(RenderType::NonBlend, this);
}

void SelectPreview::FixedUpdate(void)
{
}

void SelectPreview::Render(void)
{
	if (currentDisplayingModel == nullptr)
		return;

	static std::string world = "worldMatrix";
	Matrix identity;
	DirectX::XMStoreFloat4x4(&identity, DirectX::XMMatrixScaling(0.5f, 0.5f, 0.5f));
	::SetMatrix(world, identity);

	hanger->Render();

	transformComponent->Render();
	currentDisplayingModel->first->Render();
	currentDisplayingModel->second->Render();
	
}

void SelectPreview::ChangeModel(UINT index)
{
	switch (displayingModelIndex)
	{
	case 0:
		transformComponent->Position() = Vector3{ 0.0f, 14.0f, -25.0f };
		currentDisplayingModel = &models.find(L"F15E")->second;
		break;
	case 1:
		transformComponent->Position() = Vector3{ 0.0f, 14.0f, -20.0f };
		currentDisplayingModel = &models.find(L"F14D")->second;
		break;
	case 2:
		transformComponent->Position() = Vector3{ 0.0f, 14.0f, -20.0f };
		currentDisplayingModel = &models.find(L"F16C")->second;
		break;
	default:
		transformComponent->Position() = Vector3{ 0.0f, 14.0f, -25.0f };
		currentDisplayingModel = &models.find(L"F15E")->second;
		displayingModelIndex = 0;
		break;
	}
}

void SelectPreview::RestartGameScene(void)
{
	::EngineInstance()->SceneManager()->ReadyOtherScene(1, 0);
}
