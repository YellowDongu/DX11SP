#pragma once
#include "BasicWindow.h"
#include "ObjectManager.h"


class HierarchyWindow : public BasicWindow
{
private:
	HierarchyWindow(void) = default;
	virtual ~HierarchyWindow(void) = default;
	virtual void Free(void);
public:
    static HierarchyWindow* Create(ObjectManager* objectManager);
	HRESULT CreateFrame(void) override;

    void AddGameObjectMenu(void);
    void CloneGameObject(GameObjectMetaData* metaData, GameObjectMetaData* target);
    
    void ShortcutKey(void);


    void RenderDraggableList(std::list<GameObjectMetaData*>& items);

    ObjectManager* LinkData(void) { return objectManager; }
    void LinkData(ObjectManager* target);
private:
    bool rename{false};
    std::string renameBuffer;
    ObjectManager* objectManager{nullptr};
    std::list<GameObjectMetaData*>* selectedData{nullptr};
    std::list<GameObjectMetaData*>* gameObjectList{nullptr};

};
