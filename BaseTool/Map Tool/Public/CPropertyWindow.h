#pragma once
#include "CComponent.h"
#include "CLayer.h"
#include "CBasicWindow.h"

class CPropertyWindow : public BasicWindow
{
public:
	CPropertyWindow(void);
	virtual ~CPropertyWindow(void);

	static CPropertyWindow* Create(void);

	void Start(void);
	void update(void);
	void Release(void);

	HRESULT CreateFrame(void) override;

	std::list<std::pair<std::wstring, Engine::CLayer*>>* LinkLayerList(void) { return &layerList; }
	GameObjectMetaData*& LinkSelectedItem(void) { return *selectedItem; }

	void LinkSelectedItem(GameObjectMetaData*& target) { selectedItem = &target; }
private:
	void FilterComponent(std::string query);
	Engine::CComponent* AddComponent(void);
	void ShowComponent(void);
	void ShowLayer(void);

	bool isCreatingLayer;
	std::string layer;

	GameObjectMetaData** selectedItem;
	Engine::CProtoMgr* cloneManager;

	std::list<std::pair<const std::wstring, Engine::CComponent*>*> componentList;
	std::list<std::pair<std::wstring, Engine::CLayer*>> layerList;
};