#pragma once
#include "CBasicWindow.h"


class CHierarchyWindow : public BasicWindow
{
public:
	CHierarchyWindow(void);
	virtual ~CHierarchyWindow(void);

	HRESULT CreateFrame(void) override;
    static CHierarchyWindow* Create(LPDIRECT3DDEVICE9& _device);

    void AddGameObjectMenu(void);
    void CloneGameObject(GameObjectMetaData* metaData, GameObjectMetaData* target);
    void Release(void);

    void ShortcutKey(void);

    void LinkSelectedItem(GameObjectMetaData* target) { selectedItem = target; }

    GameObjectMetaData*& LinkSelectedItem(void) { return selectedItem; }
    std::list<GameObjectMetaData*>* LinkGameObjectList(void) { return &gameObjectList; }
    std::list<GameObjectMetaData*>* LinkUIObjectList(void) { return &uiObjectList; }

    void RenderDraggableList(std::list<GameObjectMetaData*>& items);
private:
    bool rename;
    bool OptimizationMode;
    std::string renameBuffer;
    LPDIRECT3DDEVICE9 device;

    std::list<GameObjectMetaData*> gameObjectList;
    std::list<GameObjectMetaData*> uiObjectList;
    std::list<std::pair<std::string, GameObjectMetaData*>> gameObjectNameList;
    std::list<std::pair<std::string, GameObjectMetaData*>> uiObjectNameList;
    GameObjectMetaData* selectedItem;

    std::list<GameObjectMetaData*> selectedItems;
};
