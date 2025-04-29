#include "pch.h"
#include "MonitoringWindow.h"


HRESULT MonitoringWindow::CreateFrame(void)
{
	ImGui::Begin("Debug_Monitoring", &active);

	ImGui::Text(("FPS : " + std::to_string(Time()->FPS())).c_str());
	ImGui::Text(("DeltaTime : " + std::to_string(Time()->deltaTime())).c_str());


	ImGui::End();

	return S_OK;
}
