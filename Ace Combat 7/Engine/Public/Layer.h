#pragma once
#include "Base.h"

namespace Engine
{
	class ENGINEDLL GameObject;
	class ENGINEDLL Layer : public Base
	{
	private:
		Layer(void) = default;
		virtual ~Layer(void) = default;
		virtual void Free(void);
	public:
		static Layer* Create(void) { return new Layer(); }

		virtual void Update(void);
		virtual void LateUpdate(void);
		virtual void FixedUpdate(void);
		// virtual void Render(void);

		void AddGameObject(const std::wstring& key, GameObject* object);
		std::vector<GameObject*> GetGameObjects(const std::wstring& key);
		GameObject* GetGameObject(const std::wstring& key);
		std::multimap<std::wstring, GameObject*>& GameObjectList(void) { return gameObjects; }
	private:
		std::multimap<std::wstring, GameObject*> gameObjects;
	};

}

