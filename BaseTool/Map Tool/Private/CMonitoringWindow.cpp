#include "pch.h"
#include "CMonitoringWindow.h"


CMonitoringWindow::CMonitoringWindow(void)
{
}

CMonitoringWindow::~CMonitoringWindow(void)
{
}

HRESULT CMonitoringWindow::CreateFrame(void)
{
	ImGui::Begin("Debug_Monitoring", &active);

	ImGui::Text(("FPS : " + to_string(Time->FPS())).c_str());
	ImGui::Text(("DeltaTime : " + to_string(Time->deltaTime())).c_str());


	ImGui::End();

	return S_OK;
}
