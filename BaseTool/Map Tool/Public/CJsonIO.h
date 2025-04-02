#pragma once


class CJsonIO
{
private:
	CJsonIO();
	~CJsonIO();

public:

	static HRESULT TESTSAVE(void);
	static HRESULT Save_Scene(std::list<GameObjectMetaData*>* gameObjects, std::list<GameObjectMetaData*>* UIObjects, std::wstring saveFilePath);
	static HRESULT Save_GameObject(std::list<GameObjectMetaData*>* targetList, nlohmann::json& JSONObject);
	static HRESULT Save_UIObject(std::list<GameObjectMetaData*>* targetList, nlohmann::json& JSONObject);

	static HRESULT OverrideLoad_Scene(std::list<GameObjectMetaData*>* targetList, std::list<GameObjectMetaData*>* uiObjectList, std::wstring saveFilePath);
	static HRESULT OverrideLoad_GameObject(std::list<GameObjectMetaData*>* targetList, std::vector<nlohmann::json>& JSONObject);
	static HRESULT OverrideLoad_UIObject(std::list<GameObjectMetaData*>* targetList, std::vector<nlohmann::json>& JSONObject);


	static HRESULT Load_Scene(std::list<GameObjectMetaData*>* targetList, std::list<GameObjectMetaData*>* uiObjectList, std::wstring saveFilePath);
	static HRESULT Load_GameObject(std::list<GameObjectMetaData*>* targetList, std::vector<nlohmann::json>& JSONObject);
	static HRESULT Load_UIObject(std::list<GameObjectMetaData*>* targetList, std::vector<nlohmann::json>& JSONObject);
	static void ApplyRenderComponent(std::wstring componentName, Engine::CGameObject& gameObject, Engine::CComponent* component);
	static std::string LoadJson();

	static std::wstring selectLoadFilePath(void);
	static std::wstring selectSaveFilePath(void);
private:

	static nlohmann::json SaveRenderComponentInfomation(void);

	static nlohmann::json Variable_Serialize(Engine::Variable);

	static HRESULT Component_Deserialize(Engine::CComponent* component, nlohmann::json& jsonFile);
	static HRESULT Variable_Deserialize(Engine::Variable& variable, nlohmann::json& variableJSON);
};
