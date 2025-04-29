#pragma once
#include "Base.h"

namespace Engine
{
	class Layer;
	class ENGINEDLL Scene abstract : public Base
	{
	protected:
		Scene(void) = default;
		Scene(const Scene&) = delete;
		virtual ~Scene(void) = default;

	public:
		virtual void Start(void) = 0;
		virtual void Awake(void);
		virtual void Update(void);
		virtual void LateUpdate(void);
		virtual void FixedUpdate(void);
		virtual void Render(void);
		virtual void End(void);

		void AddLayer(Layer* layer, const std::wstring& key);
		Layer* FindLayer(const std::wstring& key);

	protected:
		virtual void Free(void);

		std::map<std::wstring, Layer*> layers;
	};

}