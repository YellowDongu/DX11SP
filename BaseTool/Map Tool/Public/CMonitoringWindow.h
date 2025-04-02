#pragma once
#include "CBasicWindow.h"


class CMonitoringWindow : public BasicWindow
{
public:
	CMonitoringWindow(void);
	virtual ~CMonitoringWindow(void);

	// CBasicWindow을(를) 통해 상속됨
	HRESULT CreateFrame(void) override;
private:



};
