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
		static DebugConsole* Create(void);

		HRESULT Start(void);
		BOOL IsActive(void) { return active; }
		void SetActive(BOOL value);
	private:
		BOOL active{ false };
		FILE* stream{ nullptr };
	};

}

