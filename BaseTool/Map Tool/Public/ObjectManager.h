#pragma once

struct SavedGameObjectMetaData
{
	std::string name = "";
	std::string layerName = "";
	std::string presetName = "";
	RenderType renderType = RenderType::NonBlend;

	float3 postion = { 0.0f,0.0f ,0.0f };
	float4 rotation = { 0.0f,0.0f ,0.0f ,0.0f };
	float3 scale = { 1.0f,1.0f ,1.0f };
};

struct GameObjectMetaData
{
	std::string nameA = "";
	std::wstring name = L"";
	std::string layerNameA = "";
	std::wstring layerName = L"";
	std::wstring presetName = L"";
	Engine::GameObject* gameObject = nullptr;
	RenderType renderType = RenderType::NonBlend;

	void Free(void)
	{
		Base::DestroyInstance(gameObject);
	}
	GameObjectMetaData* Clone(void)
	{
		GameObjectMetaData* newInstance = new GameObjectMetaData();

		newInstance->nameA = nameA;
		newInstance->name = name;
		newInstance->layerNameA = layerNameA;
		newInstance->layerName = layerName;
		newInstance->presetName = presetName;
		newInstance->gameObject = gameObject->Clone();
		newInstance->renderType = renderType;
		return newInstance;
	}
	void CreateSaveData(SavedGameObjectMetaData& data)
	{
		data.name = nameA;
		data.layerName = layerNameA;
		data.presetName = ::To_String(presetName);
		data.renderType = renderType;

		if (gameObject != nullptr && gameObject->transform() != nullptr)
		{
			data.postion = gameObject->transform()->Position();
			data.rotation = gameObject->transform()->Quaternion();
			data.scale = gameObject->transform()->Scale();
		}


	}
	void LoadSaveData(SavedGameObjectMetaData& data)
	{
		nameA = data.name;
		layerNameA = data.layerName;
		presetName = ::To_WString(data.presetName);
		data.renderType = renderType;


		name = ::To_WString(nameA);
		layerName = ::To_WString(layerNameA);
		if (gameObject != nullptr && gameObject->transform() != nullptr)
		{
			gameObject->transform()->Position() = data.postion;
			gameObject->transform()->SetAngle(data.rotation);
			gameObject->transform()->Scale() = data.scale;
			gameObject->transform()->UpdateWorldMatrix();
		}

	}
};




class ObjectManager : public Base
{
private:
	ObjectManager(void) = delete;
	ObjectManager(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
	ObjectManager(const ObjectManager& other) = delete;
	virtual ~ObjectManager(void) = default;
	virtual void Free(void) override;
public:
	static ObjectManager* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);

	HRESULT Start(void);
	void Update(void);
	void LateUpdate(void);
	void FixedUpdate(void);

	void Delete(GameObjectMetaData* selected);
	void Add(GameObjectMetaData* selected);
	GameObjectMetaData* GetGameObject(std::wstring name);
	HRESULT LoadGameObject(Engine::GameObject*& createdGameObject, std::wstring name, RenderType renderType = RenderType::NonBlend);
	void Unselect(void) { selectedData.clear(); }
	void Unselect(GameObjectMetaData* target);
	void Unselect(std::list<GameObjectMetaData*> target);
	void Select(GameObjectMetaData* selected) { for (auto& data : selectedData) { if (data->gameObject == selected->gameObject && data->name == selected->name) return; } selectedData.push_back(selected); }

	

	std::list<GameObjectMetaData*>& Selected(void) { return selectedData; }
	std::list<GameObjectMetaData*>& GameObjects(void) { return gameObjectData; }
	std::map<std::wstring, GameObjectMetaData*>& LoadedGameObjects(void) { return loadedGameObject; }



private:
	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* context = nullptr;
	std::list<GameObjectMetaData*> selectedData;
	std::list<GameObjectMetaData*> gameObjectData;
	std::map<std::wstring, GameObjectMetaData*> loadedGameObject;
};

