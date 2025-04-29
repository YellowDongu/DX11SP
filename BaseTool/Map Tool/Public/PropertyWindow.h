#pragma once
#include "BasicWindow.h"
#include "ObjectManager.h"

class PropertyWindow : public BasicWindow
{
private:
	PropertyWindow(void) = default;
	virtual ~PropertyWindow(void) = default;
	virtual void Free(void);
public:
	static PropertyWindow* Create(ObjectManager* objectManager);

	void Start(void);
	void update(void);

	HRESULT CreateFrame(void) override;

	void LinkData(ObjectManager* objectManager);
	ObjectManager* LinkData(void) { return objectManager; }
private:
	void FilterComponent(std::string query);
	Engine::Component* AddComponent(void);
	void ShowComponent(void);
	void ShowLayer(void);


	ObjectManager* objectManager{ nullptr };
	std::list<GameObjectMetaData*>* selectedData{ nullptr };
	std::list<GameObjectMetaData*>* gameObjectList{ nullptr };
};