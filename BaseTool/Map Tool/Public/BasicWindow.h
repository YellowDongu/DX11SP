#pragma once
#include "Base.h"


class BasicWindow abstract : public Base
{
protected:
	BasicWindow(void);
	virtual ~BasicWindow(void) = default;
public:

	HRESULT RenderGUI(void) { if (active) { return CreateFrame(); } return S_OK; }

	bool IsActive(void) { return active; }
	void SetActive(bool value) { active = value; }

protected:
	virtual HRESULT CreateFrame(void) abstract;

	bool active{true};
};