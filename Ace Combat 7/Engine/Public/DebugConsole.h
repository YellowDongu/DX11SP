#pragma once
#include "Base.h"
#include <iostream>


namespace Engine
{
	class ENGINEDLL DebugConsole final : public Base
	{
	private:
		DebugConsole(void) = default;
		virtual ~DebugConsole(void) = default;
		virtual void Free(void) override;
	public:
		static DebugConsole* Create(bool active = false);

		HRESULT Start(void);
		bool IsActive(void) { return active; }
		HRESULT SetActive(bool value);
	private:
		bool active{ false };
		FILE* stream{ nullptr };
	};

}

