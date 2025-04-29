#pragma once


struct GameObjectMetaData;
class ObjectManager;

class CJsonIO
{
private:
	CJsonIO(void) = default;
	~CJsonIO(void) = default;

public:
	static HRESULT Save(ObjectManager* objectManager);
	static HRESULT SaveScene(ObjectManager* objectManager, std::wstring saveFilePath);
	static HRESULT SaveGameObjects(std::list<GameObjectMetaData*>* targetList, nlohmann::json& JSONObject);
	static HRESULT SaveGameObjectMetaData(GameObjectMetaData* object, nlohmann::json& JSONObject);

	//static HRESULT OverrideLoad_Scene(std::list<GameObjectMetaData*>* targetList, std::list<GameObjectMetaData*>* uiObjectList, std::wstring saveFilePath);
	//static HRESULT OverrideLoad_GameObject(std::list<GameObjectMetaData*>* targetList, std::vector<nlohmann::json>& JSONObject);
	//static HRESULT OverrideLoad_UIObject(std::list<GameObjectMetaData*>* targetList, std::vector<nlohmann::json>& JSONObject);


	static HRESULT Load(ObjectManager* objectManager);
	static HRESULT Load_Scene(ObjectManager* objectManager, std::wstring saveFilePath);
	static HRESULT LoadGameObjects(ObjectManager* objectManager, std::list<GameObjectMetaData*>& targetList, nlohmann::json& JSONObject);
	static HRESULT LoadGameObject(ObjectManager* objectManager, std::list<GameObjectMetaData*>& targetList, nlohmann::json& JSONObject);


	static std::wstring selectLoadFilePath(void);
	static std::wstring selectSaveFilePath(void);
private:
	static HRESULT savefloat3(const Vector3& vector, std::string variableName, nlohmann::json& JSONObject);
	static HRESULT savefloat4(const float4& vector, std::string variableName, nlohmann::json& JSONObject);
	static HRESULT Loadfloat3(Vector3& vector, std::string variableName, nlohmann::json& JSONObject);
	static HRESULT Loadfloat4(float4& vector, std::string variableName, nlohmann::json& JSONObject);
	static void LoadJsonString(std::string& target, std::string variableName, nlohmann::json& JSONObject);
};
