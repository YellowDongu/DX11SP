#pragma once
#include "Base.h"

namespace Engine
{
	class GameObject;
	class ClonePod final : public Base
	{
	private:
		ClonePod(void) = default;
		ClonePod(const ClonePod&) = delete;
		virtual ~ClonePod(void) = default;

	public:
		static ClonePod* Create(void);
		HRESULT SetPrefab(std::wstring& key, GameObject* gameObject);

		GameObject* ClonePrefab(std::wstring& key);
		void ClearPrefab(void);
		void ClearAliveObject(void);
	private:
		virtual void Free(void);

		std::map<std::wstring, GameObject*> prefabs;
		std::map<std::wstring, GameObject*> liveObjects;
		std::map<std::wstring, GameObject*> nonDestroyObjects;
	};

}
