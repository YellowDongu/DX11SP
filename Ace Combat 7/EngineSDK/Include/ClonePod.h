#pragma once
#include "Base.h"

namespace Engine
{
	class GameObject;
	class Component;
	class ClonePod final : public Base
	{
	private:
		ClonePod(void) = default;
		ClonePod(const ClonePod&) = delete;
		virtual ~ClonePod(void) = default;
		virtual void Free(void) override;
	public:
		static ClonePod* Create(void);
		HRESULT SetPrefab(std::wstring& key, GameObject* gameObject);
		HRESULT SetPrefab(std::wstring& key, Component* component);

		GameObject* CloneGameObjectPrefab(std::wstring& key);
		Component* CloneComponentPrefab(std::wstring& key);
		void ClearPrefab(void);
	private:

		std::map<std::wstring, GameObject*> gameObjectPrefabs;
		std::map<std::wstring, Component*> componentPrefabs;

	};

}
