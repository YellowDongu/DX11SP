#pragma once
#include "BasicWindow.h"


class MonitoringWindow : public BasicWindow
{
private:
	MonitoringWindow(void) = default;
	virtual ~MonitoringWindow(void) = default;
	virtual void Free(void) {}
public:
	static MonitoringWindow* Create(void) { return new MonitoringWindow(); }

	HRESULT CreateFrame(void) override;

	void LinkCamera(class ToolCamera* object) { viewCamera = object; }
private:
	class ToolCamera* viewCamera;


};
