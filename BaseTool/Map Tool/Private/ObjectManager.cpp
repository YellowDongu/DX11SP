#include "pch.h"
#include "ObjectManager.h"

#include "F15E.h"
#include "OceanTile.h"
#include "MS01Terrain.h"
#include "MS01TerrainA.h"
#include "MS01TerrainB.h"
#include "MS01TerrainC.h"
#include "MS01TerrainD.h"
#include "MS01TerrainE.h"

ObjectManager::ObjectManager(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : device(dxDevice), context(dxDeviceContext)
{
	device->AddRef();
	context->AddRef();
}

void ObjectManager::Free(void)
{
	device->Release();
	context->Release();
	device = nullptr;
	context = nullptr;

	selectedData.clear();
	for (auto& item : gameObjectData)
	{
		item->Free();
		delete item;
	}
	gameObjectData.clear();
	for (auto& item : loadedGameObject)
	{
		item.second->Free();
		delete item.second;
	}
	loadedGameObject.clear();
}

ObjectManager* ObjectManager::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
	ObjectManager* newInstance = new ObjectManager(dxDevice, dxDeviceContext);
	if (FAILED(newInstance->Start()))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
	return newInstance;
}

HRESULT ObjectManager::Start(void)
{
	Engine::GameObject* newInstance = F15E::Create(device, context);
	if (newInstance != nullptr)
	{
		newInstance->transform()->Position() = Vector3::zero();
		newInstance->transform()->UpdateWorldMatrix();
		LoadGameObject(newInstance, L"F15E");
	}

	newInstance = OceanTile::Create(device, context);
	if (newInstance != nullptr)
	{
		newInstance->transform()->Position() = Vector3::zero();
		newInstance->transform()->UpdateWorldMatrix();
		LoadGameObject(newInstance, L"OceanTile");
	}

	newInstance = MS01Terrain::Create(device, context);
	if (newInstance != nullptr)
	{
		newInstance->transform()->Position() = Vector3::zero();
		newInstance->transform()->UpdateWorldMatrix();
		LoadGameObject(newInstance, L"MS01Terrain");
	}

	newInstance = MS01TerrainA::Create(device, context);
	if (newInstance != nullptr)
	{
		newInstance->transform()->Position() = Vector3::zero();
		newInstance->transform()->UpdateWorldMatrix();
		LoadGameObject(newInstance, L"MS01TerrainA");
	}

	newInstance = MS01TerrainB::Create(device, context);
	if (newInstance != nullptr)
	{
		newInstance->transform()->Position() = Vector3::zero();
		newInstance->transform()->UpdateWorldMatrix();
		LoadGameObject(newInstance, L"MS01TerrainB");
	}

	newInstance = MS01TerrainC::Create(device, context);
	if (newInstance != nullptr)
	{
		newInstance->transform()->Position() = Vector3::zero();
		newInstance->transform()->UpdateWorldMatrix();
		LoadGameObject(newInstance, L"MS01TerrainC");
	}

	newInstance = MS01TerrainD::Create(device, context);
	if (newInstance != nullptr)
	{
		newInstance->transform()->Position() = Vector3::zero();
		newInstance->transform()->UpdateWorldMatrix();
		LoadGameObject(newInstance, L"MS01TerrainD");
	}

	newInstance = MS01TerrainE::Create(device, context);
	if (newInstance != nullptr)
	{
		newInstance->transform()->Position() = Vector3::zero();
		newInstance->transform()->UpdateWorldMatrix();
		LoadGameObject(newInstance, L"MS01TerrainE");
	}
	return S_OK;
}

void ObjectManager::Update(void)
{
}

void ObjectManager::LateUpdate(void)
{
	Engine::Transform* transform = nullptr;

	for (auto& objectData : gameObjectData)
	{
		if (objectData->gameObject == nullptr)
			continue;

		transform = objectData->gameObject->transform();
		if(transform != nullptr)
			transform->UpdateWorldMatrix();
		AddRenderObject(objectData->renderType, objectData->gameObject);
	}
}

void ObjectManager::FixedUpdate(void)
{
}

void ObjectManager::Unselect(GameObjectMetaData* target)
{
	for (auto iterator = selectedData.begin(); iterator != selectedData.end(); iterator++)
	{
		if (*iterator != target)
			continue;

		selectedData.erase(iterator);
		return;
	}
}

void ObjectManager::Unselect(std::list<GameObjectMetaData*> targets)
{
	for (auto& target : targets)
	{
		for (auto iterator = selectedData.begin(); iterator != selectedData.end(); iterator++)
		{
			if (*iterator != target)
				continue;

			selectedData.erase(iterator);
			break;
		}
	}
}

void ObjectManager::Delete(GameObjectMetaData* selected)
{
	Unselect(selected);
	for (auto iterator = gameObjectData.begin(); iterator != gameObjectData.end(); iterator++)
	{
		if (*iterator != selected)
			continue;

		gameObjectData.erase(iterator);
		break;
	}
	selected->Free();
	delete selected;
}

void ObjectManager::Add(GameObjectMetaData* selected)
{
	for (auto& gameObject : gameObjectData)
	{
		if (gameObject == selected)
			return;
	}
	gameObjectData.push_back(selected);
}

GameObjectMetaData* ObjectManager::GetGameObject(std::wstring name)
{
	for (auto& item : loadedGameObject)
	{
		if (item.second->name == name)
		{
			return item.second->Clone();
		}
	}
	return nullptr;
}

HRESULT ObjectManager::LoadGameObject(Engine::GameObject*& createdGameObject, std::wstring name, RenderType renderType)
{
	for (auto& item : loadedGameObject)
	{
		if (item.second->gameObject == createdGameObject)
		{
			Base::DestroyInstance(createdGameObject);
			return E_FAIL;
		}
	}

	GameObjectMetaData* newInstance = new GameObjectMetaData();
	newInstance->gameObject = createdGameObject;
	newInstance->presetName = newInstance->name = name;
	newInstance->nameA = To_String(name);
	newInstance->renderType = renderType;
	
	loadedGameObject[name] = newInstance;
	return S_OK;
}
