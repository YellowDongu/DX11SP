#include "pch.h"
#include "CJsonIO.h"


CJsonIO::CJsonIO()
{
}

CJsonIO::~CJsonIO()
{
}

HRESULT CJsonIO::TESTSAVE(void)
{
	return E_NOTIMPL;
}

HRESULT CJsonIO::Save_Scene(std::list<GameObjectMetaData*>* gameObjects, std::list<GameObjectMetaData*>* UIObjects, std::wstring saveFilePath)
{
	nlohmann::json jsonObject;

	jsonObject["RenderInfomation"] = SaveRenderComponentInfomation();

	try
	{
		Save_GameObject(gameObjects, jsonObject);
		Save_UIObject(UIObjects, jsonObject);

		std::ofstream outFile(saveFilePath);
		if (outFile.is_open())
		{
			outFile << jsonObject.dump(4);
			outFile.close();

			return S_OK;
		}
	}
	catch (const std::exception& ex)
	{
		Console::WriteLine(std::string("Error parsing JSON: ") + ex.what());
	}

	return E_FAIL;
}

HRESULT CJsonIO::Save_GameObject(std::list<GameObjectMetaData*>* targetList, nlohmann::json& JSONObject)
{
	std::vector<nlohmann::json> gameObjects;

	try
	{
		for (auto& gameObject : *targetList)
		{
			nlohmann::json gameObjectJSON;

			// Save GameObjectMetaData
			gameObjectJSON["Preset"] = To_String(gameObject->presetName);
			gameObjectJSON["GameObjectName"] = gameObject->objectName;
			gameObjectJSON["Layer"] = gameObject->layerName;
			// Save GameObjectMetaData
			
			// Get All Component
			auto& dynamicComponent = *gameObject->gameObject->Get_AllDynamicComponent();
			auto& staticComponent = *gameObject->gameObject->Get_AllStaticComponent();
			// Get All Component

			// Save dynamicComponent
			std::vector<nlohmann::json> dynamicComponents;
			for (auto& component : dynamicComponent)
			{
				nlohmann::json componentJSON;
				componentJSON["ComponentName"] = To_String(component.first);

				// Save Variables
				std::vector<nlohmann::json> variables = {};
				for (auto& variable : *component.second->Link_VariableList())
				{
					variables.push_back(Variable_Serialize(variable));
				}
				componentJSON["varibaleList"] = variables;
				dynamicComponents.push_back(componentJSON);
				// Save Variables
			}
			gameObjectJSON["DynamicComponent"] = dynamicComponents;
			// Save dynamicComponent

			// Save staticComponent
			std::vector<nlohmann::json> staticComponents;
			for (auto& component : staticComponent)
			{
				nlohmann::json componentJSON;
				componentJSON["ComponentName"] = To_String(component.first);

				// Save Variables
				std::vector<nlohmann::json> variables = {};
				for (auto& variable : *component.second->Link_VariableList())
				{

					variables.push_back(Variable_Serialize(variable));
				}
				componentJSON["varibaleList"] = variables;
				staticComponents.push_back(componentJSON);
				// Save Variables
			}

			gameObjectJSON["StaticComponent"] = staticComponents;
			// Save staticComponent


			gameObjects.push_back(gameObjectJSON);
		}

	}
	catch (const std::exception& ex)
	{
		Console::WriteLine(std::string("Error parsing JSON: ") + ex.what());
		return E_FAIL;
	}
	JSONObject["GameObjectList"] = gameObjects;

	return S_OK;
}

HRESULT CJsonIO::Save_UIObject(std::list<GameObjectMetaData*>* targetList, nlohmann::json& JSONObject)
{
	std::vector<nlohmann::json> gameObjects;

	try
	{
		for (auto& gameObject : *targetList)
		{
			nlohmann::json gameObjectJSON;

			// Save GameObjectMetaData
			gameObjectJSON["Preset"] = To_String(gameObject->presetName);
			gameObjectJSON["GameObjectName"] = gameObject->objectName;
			gameObjectJSON["Layer"] = gameObject->layerName;
			// Save GameObjectMetaData

			// Get All Component
			auto& dynamicComponent = *gameObject->gameObject->Get_AllDynamicComponent();
			auto& staticComponent = *gameObject->gameObject->Get_AllStaticComponent();
			// Get All Component

			// Save dynamicComponent
			std::vector<nlohmann::json> dynamicComponents;
			for (auto& component : dynamicComponent)
			{
				nlohmann::json componentJSON;
				componentJSON["ComponentName"] = To_String(component.first);

				std::vector<nlohmann::json> variables = {};
				for (auto& variable : *component.second->Link_VariableList())
				{
					variables.push_back(Variable_Serialize(variable));
				}
				componentJSON["varibaleList"] = variables;
				dynamicComponents.push_back(componentJSON);
			}
			gameObjectJSON["DynamicComponent"] = dynamicComponents;
			// Save dynamicComponent

			// Save staticComponent
			std::vector<nlohmann::json> staticComponents;
			for (auto& component : staticComponent)
			{
				nlohmann::json componentJSON;
				componentJSON["ComponentName"] = To_String(component.first);

				// Save Variables
				std::vector<nlohmann::json> variables = {};
				for (auto& variable : *component.second->Link_VariableList())
				{

					variables.push_back(Variable_Serialize(variable));
				}
				componentJSON["varibaleList"] = variables;
				staticComponents.push_back(componentJSON);
				// Save Variables
			}
			gameObjectJSON["StaticComponent"] = staticComponents;
			// Save staticComponent


			gameObjects.push_back(gameObjectJSON);
		}

	}
	catch (const std::exception& ex)
	{
		Console::WriteLine(std::string("Error parsing JSON: ") + ex.what());
		return E_FAIL;
	}
	JSONObject["UIObjectList"] = gameObjects;

	return S_OK;
}

HRESULT CJsonIO::Load_Scene(std::list<GameObjectMetaData*>* targetList, std::list<GameObjectMetaData*>* uiObjectList, std::wstring saveFilePath)
{
	if (!targetList)
		return E_FAIL;

	std::ifstream inFile(saveFilePath);
	if (!inFile.is_open())
		return E_FAIL;

	nlohmann::json loadedData;
	try
	{
		for (auto& data : (*targetList))
		{
			if (data->gameObject)
				data->gameObject->Release();
			delete data;
		}
		targetList->clear();

		inFile >> loadedData;

		std::vector<nlohmann::json> gameObjects = loadedData["GameObjectList"];

		Load_GameObject(targetList, gameObjects);
		
		std::vector<nlohmann::json> uiObjects = loadedData["UIObjectList"];
		Load_UIObject(uiObjectList, uiObjects);
	}
	catch (const std::exception& e)
	{
		Console::WriteLine(std::string("Error parsing JSON: ") + e.what());
		inFile.close();

		for (auto& data : (*targetList))
		{
			if (data->gameObject)
				data->gameObject->Release();
			delete data;
		}
		targetList->clear();
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CJsonIO::OverrideLoad_Scene(std::list<GameObjectMetaData*>* targetList, std::list<GameObjectMetaData*>* uiObjectList, std::wstring saveFilePath)
{
	if (!targetList)
		return E_FAIL;

	std::ifstream inFile(saveFilePath);
	if (!inFile.is_open())
		return E_FAIL;

	nlohmann::json loadedData;
	try
	{
		inFile >> loadedData;

		std::vector<nlohmann::json> gameObjects = loadedData["GameObjectList"];

		OverrideLoad_GameObject(targetList, gameObjects);

		std::vector<nlohmann::json> uiObjects = loadedData["UIObjectList"];
		OverrideLoad_UIObject(uiObjectList, uiObjects);
	}
	catch (const std::exception& e)
	{
		Console::WriteLine(std::string("Error parsing JSON: ") + e.what());

		inFile.close();

		for (auto& data : (*targetList))
		{
			if (data->gameObject)
				data->gameObject->Release();
			delete data;
		}
		targetList->clear();

		return E_FAIL;
	}

	return S_OK;
}

HRESULT CJsonIO::OverrideLoad_GameObject(std::list<GameObjectMetaData*>* targetList, std::vector<nlohmann::json>& gameObjectList)
{
	LPDIRECT3DDEVICE9 device = CGraphicDev::GetInstance()->Get_GrphicDev();
	bool newLayer = true;
	bool duplicate = false;
	try
	{
		for (auto& gameObject : gameObjectList)
		{
			bool isPresetObject = false;
			GameObjectMetaData* newMetaData = new GameObjectMetaData();

			newMetaData->belongingLayer = nullptr;

			std::string preset = gameObject["Preset"];
			std::string gameObjectName = gameObject["GameObjectName"];
			std::string layerName = gameObject["Layer"];
			newMetaData->presetName = To_WString(preset);
			newMetaData->objectName = gameObjectName;
			newMetaData->layerName = layerName;
			newMetaData->gameObject = nullptr;

			duplicate = false;
			for (auto& gameObject : *targetList)
			{
				if (gameObject->objectName == newMetaData->objectName && gameObject->presetName == newMetaData->presetName && gameObject->layerName == newMetaData->layerName)
				{
					duplicate = true;
					break;
				}
			}

			if (duplicate)
			{
				delete newMetaData;
				continue;
			}



			bool newLayer = true;
			std::wstring wLayerName = To_WString(layerName);

			if (wLayerName != L"")
			{
				for (auto& item : *globalLayerList)
				{
					if (item.first == wLayerName)
						newLayer = false;
				}
				if (newLayer)
					(*globalLayerList).push_back({ wLayerName, nullptr });


			}


			for (auto& GameObject : gameObjectPreset)
			{
				if (newMetaData->presetName == GameObject.first)
				{
					newMetaData->gameObject = GameObject.second(device);
					isPresetObject = true;
					break;
				}
			}
			if (!newMetaData->gameObject)
				newMetaData->gameObject = Engine::CGameObject::Create(device);

			CGameObject* loadedObject = newMetaData->gameObject;

			std::vector<nlohmann::json> dynamicComponents = gameObject["DynamicComponent"];
			std::vector<nlohmann::json> staticComponents = gameObject["StaticComponent"];

			for (auto& component : dynamicComponents)
			{
				std::string componentName = component["ComponentName"];
				std::wstring convertedName = To_WString(componentName);
				CComponent* loadedComponent = nullptr;
				if (isPresetObject)
				{


					for (auto& targetComponent : *newMetaData->gameObject->Get_AllDynamicComponent())
					{
						if (targetComponent.first != convertedName)
							continue;

						if (targetComponent.first == transformComponentKey)
						{
							CTransform* component = dynamic_cast<CTransform*>(targetComponent.second);
							transformList.push_back(&component);
						}
						else if (targetComponent.first == colliderComponentKey)
						{
							CCollider* component = dynamic_cast<CCollider*>(targetComponent.second);
							colliderList.push_back(&component);
						}

						loadedComponent = targetComponent.second;
						break;
					}
					if (!isPresetObject)
						continue;
				}
				else
				{
					loadedComponent = CProtoMgr::GetInstance()->Clone_Prototype(convertedName);
					if (!loadedComponent)
						continue;
					ApplyRenderComponent(convertedName, *loadedObject, loadedComponent);
					(*loadedObject->Get_AllDynamicComponent())[convertedName] = loadedComponent;
				}

				Component_Deserialize(loadedComponent, component);
			}

			for (auto& component : staticComponents)
			{
				std::string componentName = component["ComponentName"];
				std::wstring convertedName = To_WString(componentName);
				CComponent* loadedComponent = nullptr;
				if (isPresetObject)
				{
					for (auto& targetComponent : *newMetaData->gameObject->Get_AllStaticComponent())
					{
						if (targetComponent.first != convertedName)
							continue;

						if (targetComponent.first == transformComponentKey)
						{
							CTransform* component = dynamic_cast<CTransform*>(targetComponent.second);
							transformList.push_back(&component);
						}
						else if (targetComponent.first == colliderComponentKey)
						{
							CCollider* component = dynamic_cast<CCollider*>(targetComponent.second);
							colliderList.push_back(&component);
						}

						loadedComponent = targetComponent.second;
						break;
					}
					if (!isPresetObject)
						continue;
				}
				else
				{
					loadedComponent = CProtoMgr::GetInstance()->Clone_Prototype(convertedName);
					if (!loadedComponent)
						continue;
					ApplyRenderComponent(convertedName, *loadedObject, loadedComponent);
					(*loadedObject->Get_AllStaticComponent())[convertedName] = loadedComponent;
				}

				Component_Deserialize(loadedComponent, component);
			}

			(*targetList).push_back(newMetaData);
		}

	}
	catch (const std::exception& ex)
	{
		Console::WriteLine(std::string("Error parsing JSON: ") + ex.what());

		return  E_FAIL;
	}

	return S_OK;

}

HRESULT CJsonIO::OverrideLoad_UIObject(std::list<GameObjectMetaData*>* targetList, std::vector<nlohmann::json>& UIObjectList)
{
	LPDIRECT3DDEVICE9 device = CGraphicDev::GetInstance()->Get_GrphicDev();

	bool duplicate = false;
	try
	{
		for (auto& gameObject : UIObjectList)
		{
			GameObjectMetaData* newMetaData = new GameObjectMetaData();

			newMetaData->belongingLayer = nullptr;

			std::string preset = gameObject["Preset"];
			std::string gameObjectName = gameObject["GameObjectName"];
			std::string layerName = gameObject["Layer"];
			newMetaData->presetName = To_WString(preset);
			newMetaData->objectName = gameObjectName;
			newMetaData->layerName = layerName;
			newMetaData->gameObject = nullptr;


			duplicate = false;
			for (auto& gameObject : *targetList)
			{
				if (gameObject->objectName == newMetaData->objectName && gameObject->presetName == newMetaData->presetName && gameObject->layerName == newMetaData->layerName)
				{
					duplicate = true;
					break;
				}
			}

			if (duplicate)
			{
				delete newMetaData;
				continue;
			}





			for (auto& GameObject : gameObjectPreset)
			{
				if (newMetaData->presetName == GameObject.first)
				{
					newMetaData->gameObject = GameObject.second(device);
					break;
				}
			}
			if (newMetaData->gameObject)
			{
				(*targetList).push_back(newMetaData);
				continue;
			}

			CGameObject* loadedObject = newMetaData->gameObject = Engine::CGameObject::Create(device);

			std::vector<nlohmann::json> dynamicComponents = gameObject["DynamicComponent"];
			std::vector<nlohmann::json> staticComponents = gameObject["StaticComponent"];

			for (auto& component : dynamicComponents)
			{
				std::string componentName = component["ComponentName"];
				std::wstring convertedName = To_WString(componentName);
				CComponent* loadedComponent = CProtoMgr::GetInstance()->Clone_Prototype(convertedName);
				if (!loadedComponent)
					continue;

				ApplyRenderComponent(convertedName, *loadedObject, loadedComponent);
				(*loadedObject->Get_AllDynamicComponent())[convertedName] = loadedComponent;
				Component_Deserialize(loadedComponent, component);
			}

			for (auto& component : staticComponents)
			{
				std::string componentName = component["ComponentName"];
				std::wstring convertedName = To_WString(componentName);
				CComponent* loadedComponent = CProtoMgr::GetInstance()->Clone_Prototype(convertedName);
				if (!loadedComponent)
					continue;

				ApplyRenderComponent(convertedName, *loadedObject, loadedComponent);
				(*loadedObject->Get_AllDynamicComponent())[convertedName] = loadedComponent;
				Component_Deserialize(loadedComponent, component);
			}

			(*targetList).push_back(newMetaData);
		}

	}
	catch (const std::exception& ex)
	{
		Console::WriteLine(std::string("Error parsing JSON: ") + ex.what());

		return  E_FAIL;
	}

	return S_OK;
}

HRESULT CJsonIO::Load_GameObject(std::list<GameObjectMetaData*>* targetList, std::vector<nlohmann::json>& gameObjectList)
{
	LPDIRECT3DDEVICE9 device = CGraphicDev::GetInstance()->Get_GrphicDev();
	
	try
	{
		for (auto& gameObject : gameObjectList)
		{
			bool isPresetObject = false;
			GameObjectMetaData* newMetaData = new GameObjectMetaData();

			newMetaData->belongingLayer = nullptr;

			std::string preset = gameObject["Preset"];
			std::string gameObjectName = gameObject["GameObjectName"];
			std::string layerName = gameObject["Layer"];
			newMetaData->presetName = To_WString(preset);
			newMetaData->objectName = gameObjectName;
			newMetaData->layerName = layerName;
			newMetaData->gameObject = nullptr;

			bool newLayer = true;
			std::wstring wLayerName = To_WString(layerName);

			if (wLayerName != L"")
			{
				for (auto& item : *globalLayerList)
				{
					if (item.first == wLayerName)
						newLayer = false;
				}
				if (newLayer)
					(*globalLayerList).push_back({ wLayerName, nullptr });


			}


			for (auto& GameObject : gameObjectPreset)
			{
				if (newMetaData->presetName == GameObject.first)
				{
					newMetaData->gameObject = GameObject.second(device);
					isPresetObject = true;
					break;
				}
			}
			if (!newMetaData->gameObject)
				newMetaData->gameObject = Engine::CGameObject::Create(device);

			CGameObject* loadedObject = newMetaData->gameObject;

			std::vector<nlohmann::json> dynamicComponents = gameObject["DynamicComponent"];
			std::vector<nlohmann::json> staticComponents = gameObject["StaticComponent"];

			for (auto& component : dynamicComponents)
			{
				std::string componentName = component["ComponentName"];
				std::wstring convertedName = To_WString(componentName);
				CComponent* loadedComponent = nullptr;
				if (isPresetObject)
				{


					for (auto& targetComponent : *newMetaData->gameObject->Get_AllDynamicComponent())
					{
						if (targetComponent.first != convertedName)
							continue;

						if (targetComponent.first == transformComponentKey)
						{
							CTransform* component = dynamic_cast<CTransform*>(targetComponent.second);
							transformList.push_back(&component);
						}
						else if (targetComponent.first == colliderComponentKey)
						{
							CCollider* component = dynamic_cast<CCollider*>(targetComponent.second);
							colliderList.push_back(&component);
						}

						loadedComponent = targetComponent.second;
						break;
					}
					if (!isPresetObject)
						continue;
				}
				else
				{
					loadedComponent = CProtoMgr::GetInstance()->Clone_Prototype(convertedName);
					if (!loadedComponent)
						continue;
					ApplyRenderComponent(convertedName, *loadedObject, loadedComponent);
					(*loadedObject->Get_AllDynamicComponent())[convertedName] = loadedComponent;
				}

				Component_Deserialize(loadedComponent, component);
			}

			for (auto& component : staticComponents)
			{
				std::string componentName = component["ComponentName"];
				std::wstring convertedName = To_WString(componentName);
				CComponent* loadedComponent = nullptr;
				if (isPresetObject)
				{
					for (auto& targetComponent : *newMetaData->gameObject->Get_AllStaticComponent())
					{
						if (targetComponent.first != convertedName)
							continue;

						if (targetComponent.first == transformComponentKey)
						{
							CTransform* component = dynamic_cast<CTransform*>(targetComponent.second);
							transformList.push_back(&component);
						}
						else if (targetComponent.first == colliderComponentKey)
						{
							CCollider* component = dynamic_cast<CCollider*>(targetComponent.second);
							colliderList.push_back(&component);
						}

						loadedComponent = targetComponent.second;
						break;
					}
					if (!isPresetObject)
						continue;
				}
				else
				{
					loadedComponent = CProtoMgr::GetInstance()->Clone_Prototype(convertedName);
					if (!loadedComponent)
						continue;
					ApplyRenderComponent(convertedName, *loadedObject, loadedComponent);
					(*loadedObject->Get_AllStaticComponent())[convertedName] = loadedComponent;
				}

				Component_Deserialize(loadedComponent, component);
			}

			(*targetList).push_back(newMetaData);
		}

	}
	catch (const std::exception& ex)
	{
		Console::WriteLine(std::string("Error parsing JSON: ") + ex.what());

		return  E_FAIL;
	}

	return S_OK;


}

HRESULT CJsonIO::Load_UIObject(std::list<GameObjectMetaData*>* targetList, std::vector<nlohmann::json>& UIObjectList)
{
	LPDIRECT3DDEVICE9 device = CGraphicDev::GetInstance()->Get_GrphicDev();

	try
	{
		for (auto& gameObject : UIObjectList)
		{
			GameObjectMetaData* newMetaData = new GameObjectMetaData();

			newMetaData->belongingLayer = nullptr;

			std::string preset = gameObject["Preset"];
			std::string gameObjectName = gameObject["GameObjectName"];
			std::string layerName = gameObject["Layer"];
			newMetaData->presetName = To_WString(preset);
			newMetaData->objectName = gameObjectName;
			newMetaData->layerName = layerName;
			newMetaData->gameObject = nullptr;
			for (auto& GameObject : gameObjectPreset)
			{
				if (newMetaData->presetName == GameObject.first)
				{
					newMetaData->gameObject = GameObject.second(device);
					break;
				}
			}
			if (newMetaData->gameObject)
			{
				(*targetList).push_back(newMetaData);
				continue;
			}

			CGameObject* loadedObject = newMetaData->gameObject = Engine::CGameObject::Create(device);

			std::vector<nlohmann::json> dynamicComponents = gameObject["DynamicComponent"];
			std::vector<nlohmann::json> staticComponents = gameObject["StaticComponent"];

			for (auto& component : dynamicComponents)
			{
				std::string componentName = component["ComponentName"];
				std::wstring convertedName = To_WString(componentName);
				CComponent* loadedComponent = CProtoMgr::GetInstance()->Clone_Prototype(convertedName);
				if (!loadedComponent)
					continue;

				ApplyRenderComponent(convertedName, *loadedObject, loadedComponent);
				(*loadedObject->Get_AllDynamicComponent())[convertedName] = loadedComponent;
				Component_Deserialize(loadedComponent, component);
			}

			for (auto& component : staticComponents)
			{
				std::string componentName = component["ComponentName"];
				std::wstring convertedName = To_WString(componentName);
				CComponent* loadedComponent = CProtoMgr::GetInstance()->Clone_Prototype(convertedName);
				if (!loadedComponent)
					continue;

				ApplyRenderComponent(convertedName, *loadedObject, loadedComponent);
				(*loadedObject->Get_AllDynamicComponent())[convertedName] = loadedComponent;
				Component_Deserialize(loadedComponent, component);
			}

			(*targetList).push_back(newMetaData);
		}

	}
	catch (const std::exception& ex)
	{
		Console::WriteLine(std::string("Error parsing JSON: ") + ex.what());

		return  E_FAIL;
	}

	return S_OK;
}

std::string CJsonIO::LoadJson()
{
	return "";
	std::ifstream inFile("Test2.txt");
	if (!inFile.is_open())
		return "";

	nlohmann::json loadedData;
	try
	{
		inFile >> loadedData;

	}
	catch (const std::exception& e)
	{
		inFile.close();
		return  std::string("Error parsing JSON: ") + e.what();
	}
	return std::string(loadedData.dump(4));
}

nlohmann::json CJsonIO::Variable_Serialize(Engine::Variable variable)
{
	nlohmann::json variableJSON;
	variableJSON["variableName"] = variable.name;
	variableJSON["variableType"] = (int)variable.type;

	switch (variable.type)
	{
	case Engine::VariableType::string:
		variableJSON["variableData"] = *(std::string*)variable.data;
		break;
	case Engine::VariableType::wstring:
		variableJSON["variableData"] = To_String(*(std::wstring*)variable.data);
		break;
	case Engine::VariableType::Bool:
		variableJSON["variableData"] = *(bool*)variable.data;
		break;
	case Engine::VariableType::Byte:
		variableJSON["variableData"] = (int)(*(char*)variable.data);
		break;
	case Engine::VariableType::UByte:
		variableJSON["variableData"] = (unsigned int)(*(unsigned char*)variable.data);
		break;
	case Engine::VariableType::Tchar:
		variableJSON["variableData"] = (int)(*(wchar_t*)variable.data);
		break;
	case Engine::VariableType::Short:
		variableJSON["variableData"] = *(short*)variable.data;
		break;
	case Engine::VariableType::UShort:
		variableJSON["variableData"] = *(unsigned short*)variable.data;
		break;
	case Engine::VariableType::Int:
		variableJSON["variableData"] = *(int*)variable.data;
		break;
	case Engine::VariableType::UInt:
		variableJSON["variableData"] = *(unsigned int*)variable.data;
		break;
	case Engine::VariableType::Long:
		variableJSON["variableData"] = *(long*)variable.data;
		break;
	case Engine::VariableType::ULong:
		variableJSON["variableData"] = *(unsigned long*)variable.data;
		break;
	case Engine::VariableType::Float:
		variableJSON["variableData"] = *(float*)variable.data;
		break;
	case Engine::VariableType::Double:
		variableJSON["variableData"] = *(double*)variable.data;
		break;
	case Engine::VariableType::Vec2:
	{
		nlohmann::json dataJSON;
		vec2& data = *(vec2*)variable.data;

		dataJSON["x"] = data.x;
		dataJSON["y"] = data.y;
		variableJSON["variableData"] = dataJSON;
	}
		break;
	case Engine::VariableType::Vec3:
	{
		nlohmann::json dataJSON;
		vec3& data = *(vec3*)variable.data;
		
		dataJSON["x"] = data.x;
		dataJSON["y"] = data.y;
		dataJSON["z"] = data.z;
		variableJSON["variableData"] = dataJSON;
	}
		break;
	case Engine::VariableType::Vec4:
	{
		nlohmann::json dataJSON;
		vec4& data = *(vec4*)variable.data;

		dataJSON["x"] = data.x;
		dataJSON["y"] = data.y;
		dataJSON["z"] = data.z;
		dataJSON["w"] = data.w;

		variableJSON["variableData"] = dataJSON;
	}
		break;
	case Engine::VariableType::Matrix:
	{
		nlohmann::json dataJSON;
		matrix& data = *(matrix*)variable.data;

		dataJSON["_11"] = data._11;
		dataJSON["_12"] = data._12;
		dataJSON["_13"] = data._13;
		dataJSON["_14"] = data._14;
		dataJSON["_21"] = data._21;
		dataJSON["_22"] = data._22;
		dataJSON["_23"] = data._23;
		dataJSON["_24"] = data._24;
		dataJSON["_31"] = data._31;
		dataJSON["_32"] = data._32;
		dataJSON["_33"] = data._33;
		dataJSON["_34"] = data._34;
		dataJSON["_41"] = data._41;
		dataJSON["_42"] = data._42;
		dataJSON["_43"] = data._43;
		dataJSON["_44"] = data._44;

		variableJSON["variableData"] = dataJSON;
	}
		break;
	case Engine::VariableType::Color:
		variableJSON["variableData"] = *(unsigned long*)variable.data;
		break;
	case Engine::VariableType::vertexSet:
	{
		nlohmann::json dataJSON;
		Engine::VertexSet& vertexSetData = *(Engine::VertexSet*)variable.data;

		dataJSON["rhw"] = vertexSetData.rhw;
		dataJSON["triangleCount"] = vertexSetData.triangleCount;


		std::vector<nlohmann::json> indexData;
		for (auto& listItem : vertexSetData.index)
		{
			nlohmann::json innerDataJSON;
			innerDataJSON["_0"] = listItem._0;
			innerDataJSON["_1"] = listItem._1;
			innerDataJSON["_2"] = listItem._2;
			indexData.push_back(innerDataJSON);
		}
		dataJSON["indexData"] = indexData;

		std::vector<nlohmann::json> simpleVertex;
		for (auto& listItem : vertexSetData.simpleVertex)
		{
			nlohmann::json innerDataJSON;
			innerDataJSON["color"] = listItem.dwColor;
			innerDataJSON["positionX"] = listItem.vPosition.x;
			innerDataJSON["positionY"] = listItem.vPosition.y;
			innerDataJSON["positionZ"] = listItem.vPosition.z;
			simpleVertex.push_back(innerDataJSON);
		}
		dataJSON["simpleVertexData"] = simpleVertex;

		std::vector<nlohmann::json> textureVertex;
		for (auto& listItem : vertexSetData.textureVertex)
		{
			nlohmann::json innerDataJSON;
			innerDataJSON["positionX"] = listItem.vPosition.x;
			innerDataJSON["positionY"] = listItem.vPosition.y;
			innerDataJSON["positionZ"] = listItem.vPosition.z;
			innerDataJSON["textureU"] = listItem.vTexUV.x;
			innerDataJSON["textureV"] = listItem.vTexUV.y;
			textureVertex.push_back(innerDataJSON);
		}
		dataJSON["textureVertexData"] = textureVertex;

		std::vector<nlohmann::json> cubeVertex;
		for (auto& listItem : vertexSetData.cubeVertex)
		{
			nlohmann::json innerDataJSON;
			innerDataJSON["positionX"] = listItem.vPosition.x;
			innerDataJSON["positionY"] = listItem.vPosition.y;
			innerDataJSON["positionZ"] = listItem.vPosition.z;
			innerDataJSON["textureU"] = listItem.vTexUV.x;
			innerDataJSON["textureV"] = listItem.vTexUV.y;
			innerDataJSON["textureW"] = listItem.vTexUV.z;
			cubeVertex.push_back(innerDataJSON);
		}
		dataJSON["cubeVertexData"] = cubeVertex;

		variableJSON["variableData"] = dataJSON;
	}
	break;
	default:
		break;
	}



	return variableJSON;
}

void CJsonIO::ApplyRenderComponent(std::wstring componentName, Engine::CGameObject& gameObject, Engine::CComponent* component)
{
	if (componentName == transformComponentKey)
	{
		gameObject.Add_Render(RenderOrder::World, component);
		CCollider* collider = (CCollider*)(*gameObject.Get_RenderingComponent())[(int)RenderOrder::Debug_Collider];
		if (collider)
		{
			collider->LinkPosition(&((CTransform*)component)->Position());
		}

		auto rigidbody = gameObject.Get_AllDynamicComponent()->find(L"Proto_Rigidbody");
		if (rigidbody == gameObject.Get_AllDynamicComponent()->end())
		{
			auto rigidbody = gameObject.Get_AllStaticComponent()->find(L"Proto_Rigidbody");
			if (rigidbody == gameObject.Get_AllStaticComponent()->end())
				return;
		}
		((CRigidbody*)(*rigidbody).second)->Set_Transform((CTransform*)component);

		return;
	}
	if (componentName == L"Proto_Rigidbody")
	{
		CTransform* transform = (CTransform*)(*gameObject.Get_RenderingComponent())[(int)RenderOrder::World];
		if (transform)
		{
			((CRigidbody*)component)->Set_Transform(transform);
		}
	}

	if (componentName == textureComponentKey)
	{
		gameObject.Add_Render(RenderOrder::Texture, component);
		return;
	}
	
	if (componentName == colliderComponentKey)
	{
		gameObject.Add_Render(RenderOrder::Debug_Collider, component);
		CTransform* transform = (CTransform*)(*gameObject.Get_RenderingComponent())[(int)RenderOrder::World];
		if (transform)
		{
			((CCollider*)component)->LinkPosition(&transform->Position());
		}
		return;
	}

	for (auto& string : vertex)
	{
		if (componentName == string)
		{
			gameObject.Add_Render(RenderOrder::Vertex, component);
			return;
		}
	}

	for (auto& string : textureKey)
	{
		if (To_String(componentName) == string.first || componentName == string.second)
		{
			gameObject.Add_Render(RenderOrder::Texture, component);
			((CTexture*)component)->Set_Texture(string.second);
			return;
		}
	}

}


nlohmann::json CJsonIO::SaveRenderComponentInfomation(void)
{
	nlohmann::json metaData;
	std::vector<nlohmann::json> texturesMetaData;
	std::vector<nlohmann::json> vertexesMetaData;


	for (auto& string : textureKey)
	{
		nlohmann::json texture;
		std::string key = string.first;
		std::string value = To_String(string.second);
		texture["Texture"] = key;
		texture[key] = value;
		texturesMetaData.push_back(texture);
	}

	for (auto& string : vertex)
	{
		nlohmann::json vertex;
		std::string key = To_String(string);
		vertex["Vertex"] = key;
		vertexesMetaData.push_back(vertex);
	}

	metaData["Transform"] = To_String(transformComponentKey);
	metaData["Texture"] = To_String(textureComponentKey);
	metaData["Collider"] = To_String(colliderComponentKey);
	metaData["Textures"] = texturesMetaData;
	metaData["Vertex"] = vertexesMetaData;

	Console::WriteLine(metaData.dump(4));

	return metaData;
}







HRESULT CJsonIO::Component_Deserialize(Engine::CComponent* component, nlohmann::json& jsonFile)
{
	try
	{
		if (component == nullptr)
			return E_FAIL;

		std::vector<nlohmann::json> loadedVariableList = jsonFile["varibaleList"];
		for (auto& variable : (*component->Link_VariableList()))
		{
			for (auto& loadedVariable : loadedVariableList)
			{
				std::string loadedName = loadedVariable["variableName"];
				Engine::VariableType loadedType = (Engine::VariableType)((int)loadedVariable["variableType"]);
				if (variable.name == loadedName && variable.type == loadedType)
					Variable_Deserialize(variable, loadedVariable);
			}
		}
		component->Refresh_Component();
	}
	catch (const std::exception& e)
	{
		Console::WriteLine(std::string("Error parsing JSON: ") + e.what());

		return E_FAIL;
	}

	return S_OK;
}


HRESULT CJsonIO::Variable_Deserialize(Engine::Variable& variable, nlohmann::json& variableJSON)
{
	auto variableData = variableJSON.find("variableData");
	if (variableData == variableJSON.end())
		return E_FAIL;


	switch (variable.type)
	{
	case Engine::VariableType::string:
	{
		std::string storedData = variableJSON["variableData"];
		*(std::string*)variable.data = storedData;
	}
		break;
	case Engine::VariableType::wstring:
	{
		std::string storedData = variableJSON["variableData"];
		*(std::wstring*)variable.data = To_WString(storedData);
	}
		break;
	case Engine::VariableType::Bool:
		*(bool*)variable.data = (bool)(*variableData);
		break;
	case Engine::VariableType::Byte:
		*((char*)variable.data) = (char)((int)(*variableData));
		break;
	case Engine::VariableType::UByte:
		*((char*)variable.data) = (unsigned char)((unsigned int)(*variableData));
		break;
	case Engine::VariableType::Tchar:
		*((wchar_t*)variable.data) = (wchar_t)((int)(*variableData));
		break;
	case Engine::VariableType::Short:
		*((short*)variable.data) = (short)(*variableData);
		break;
	case Engine::VariableType::UShort:
		*((unsigned short*)variable.data) = (unsigned short)(*variableData);
		break;
	case Engine::VariableType::Int:
		*((int*)variable.data) = (int)(*variableData);
		break;
	case Engine::VariableType::UInt:
		*((unsigned int*)variable.data) = (unsigned int)(*variableData);
		break;
	case Engine::VariableType::Long:
		*((long*)variable.data) = (long)(*variableData);
		break;
	case Engine::VariableType::ULong:
		*((unsigned long*)variable.data) = (unsigned long)(*variableData);
		break;
	case Engine::VariableType::Float:
		*((float*)variable.data) = (float)(*variableData);
		break;
	case Engine::VariableType::Double:
		*((double*)variable.data) = (double)(*variableData);
		break;
	case Engine::VariableType::Vec2:
	{
		nlohmann::json dataContainer = (*variableData);
		Engine::vec2& data = *(Engine::vec2*)(variable.data);

		auto xIterator = dataContainer.find("x");
		if(xIterator == dataContainer.end())
			return E_FAIL;
		auto yIterator = dataContainer.find("y");
		if (yIterator == dataContainer.end())
			return E_FAIL;

		data.x = (float)(*xIterator);
		data.y = (float)(*yIterator);
	}
	break;
	case Engine::VariableType::Vec3:
	{
		nlohmann::json dataContainer = (*variableData);
		Engine::vec3& data = *(Engine::vec3*)(variable.data);

		auto xIterator = dataContainer.find("x");
		if (xIterator == dataContainer.end())
			return E_FAIL;
		auto yIterator = dataContainer.find("y");
		if (yIterator == dataContainer.end())
			return E_FAIL;
		auto zIterator = dataContainer.find("z");
		if (zIterator == dataContainer.end())
			return E_FAIL;

		data.x = (float)(*xIterator);
		data.y = (float)(*yIterator);
		data.z = (float)(*zIterator);
	}
	break;
	case Engine::VariableType::Vec4:
	{
		nlohmann::json dataContainer = (*variableData);
		Engine::vec4& data = *(Engine::vec4*)(variable.data);

		auto xIterator = dataContainer.find("x");
		if (xIterator == dataContainer.end())
			return E_FAIL;
		auto yIterator = dataContainer.find("y");
		if (yIterator == dataContainer.end())
			return E_FAIL;
		auto zIterator = dataContainer.find("z");
		if (zIterator == dataContainer.end())
			return E_FAIL;
		auto wIterator = dataContainer.find("w");
		if (wIterator == dataContainer.end())
			return E_FAIL;

		data.x = (float)(*xIterator);
		data.y = (float)(*yIterator);
		data.z = (float)(*zIterator);
		data.w = (float)(*wIterator);
	}
	break;
	case Engine::VariableType::Matrix:
	{
		nlohmann::json dataContainer = (*variableData);

		if (!dataContainer.contains("_11") || !dataContainer.contains("_44"))
			return E_FAIL;

		matrix& data = *(matrix*)(variable.data);

		data._11 = (float)dataContainer["_11"];
		data._12 = (float)dataContainer["_12"];
		data._13 = (float)dataContainer["_13"];
		data._14 = (float)dataContainer["_14"];
		data._21 = (float)dataContainer["_21"];
		data._22 = (float)dataContainer["_22"];
		data._23 = (float)dataContainer["_23"];
		data._24 = (float)dataContainer["_24"];
		data._31 = (float)dataContainer["_31"];
		data._32 = (float)dataContainer["_32"];
		data._33 = (float)dataContainer["_33"];
		data._34 = (float)dataContainer["_34"];
		data._41 = (float)dataContainer["_41"];
		data._42 = (float)dataContainer["_42"];
		data._43 = (float)dataContainer["_43"];
		data._44 = (float)dataContainer["_44"];
	}
	break;
	case Engine::VariableType::Color:
		*((DWORD*)variable.data) = (DWORD)(*variableData);
		break;
	case Engine::VariableType::vertexSet:
	{
		nlohmann::json dataJSON = *variableData;
		Engine::VertexSet& vertexSetData = *(Engine::VertexSet*)variable.data;

		vertexSetData.rhw = dataJSON["rhw"];
		vertexSetData.triangleCount = dataJSON["triangleCount"];

		std::vector<nlohmann::json> indexData = dataJSON["indexData"];
		for (auto& listItem : indexData)
		{
			vertexSetData.index.push_back({ listItem["_0"], listItem["_1"], listItem["_2"] });
		}

		std::vector<nlohmann::json> simpleVertex = dataJSON["simpleVertexData"];
		for (auto& listItem : simpleVertex)
		{
			Engine::tagVertexColor vertex = { {listItem["positionX"],listItem["positionY"],listItem["positionZ"]}, listItem["color"] };
			vertexSetData.simpleVertex.push_back(vertex);
		}

		std::vector<nlohmann::json> textureVertex = dataJSON["textureVertexData"];
		for (auto& listItem : textureVertex)
		{
			Engine::tagVertexTexture vertex;
			vertex.vPosition = { listItem["positionX"] ,listItem["positionY"] ,listItem["positionZ"] };
			vertex.vNormal = {0.0f,0.0f ,0.0f };
			vertex.vTexUV = { listItem["textureU"],listItem["textureV"] };
			vertexSetData.textureVertex.push_back(vertex);
		}

		std::vector<nlohmann::json> cubeVertex = dataJSON["cubeVertexData"];
		for (auto& listItem : cubeVertex)
		{
			Engine::tagVertexCubeTexture vertex;
			vertex.vPosition = { listItem["positionX"],listItem["positionY"],listItem["positionZ"] };
			vertex.vTexUV = { listItem["textureU"],listItem["textureV"],listItem["textureW"] };
			vertexSetData.cubeVertex.push_back(vertex);
		}



	}
		break;
	default:
		break;
	}

	return S_OK;
}


std::wstring CJsonIO::selectSaveFilePath(void)
{
	wchar_t currentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, currentDir);

	OPENFILENAME ofn;
	wchar_t szFile[260] = { 0 };
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = nullptr;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	//ofn.lpstrFilter = L"All Files\0*.*\0Text Files\0*.TXT\0";
	ofn.lpstrFilter = L"All File(*.*)|*.*||";
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;


	wchar_t initialDirectory[260] = { 0 };
	GetCurrentDirectory(MAX_PATH, initialDirectory);
	//PathRemoveFileSpec(initialDirectory);
	//lstrcat(initialDirectory, L"\\Data");
	ofn.lpstrInitialDir = initialDirectory;

	if (GetSaveFileName(&ofn))
	{
		std::wstring selected = ofn.lpstrFile;

		SetCurrentDirectory(currentDir);
		return selected;
	}
	SetCurrentDirectory(currentDir);
	return std::wstring();
}

std::wstring CJsonIO::selectLoadFilePath(void)
{
	wchar_t currentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, currentDir);

	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));

	wchar_t szFile[260] = { 0 };
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = nullptr;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	//ofn.lpstrFilter = L"All Files\0*.*\0Text Files\0*.TXT\0";
	ofn.lpstrFilter = L"All File(*.*)|*.*||";
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;


	wchar_t initialDirectory[260] = { 0 };
	GetCurrentDirectory(MAX_PATH, initialDirectory);
	//PathRemoveFileSpec(initialDirectory);
	//lstrcat(initialDirectory, L"\\Data");
	//PathRemoveExtension(initialDirectory); => Ȯ���ڸ� ����(shlwapi.h)
	PathRemoveExtension(initialDirectory);
	ofn.lpstrInitialDir = initialDirectory;

	if (GetOpenFileName(&ofn))
	{
		std::wstring selected = ofn.lpstrFile;

		SetCurrentDirectory(currentDir);
		return selected;
	}
	SetCurrentDirectory(currentDir);
	return std::wstring();
}
