#include "pch.h"
#include "MonitoringWindow.h"
#include "ToolCamera.h"

HRESULT MonitoringWindow::CreateFrame(void)
{
	ImGui::Begin("Debug_Monitoring", &active);

	ImGui::Text(("FPS : " + std::to_string(Time()->FPS())).c_str());
	ImGui::Text(("DeltaTime : " + std::to_string(Time()->deltaTime())).c_str());

	if (viewCamera != nullptr)
	{
		ImGui::Text("CameraInfo");
		ImGui::Text("Position");
		Vector3& position = viewCamera->transform()->Position();
		ImGui::Text(("x : " + std::to_string(position.x)).c_str());
		ImGui::Text(("y : " + std::to_string(position.y)).c_str());
		ImGui::Text(("z : " + std::to_string(position.z)).c_str());
	}

	ImGui::End();

	return S_OK;
}
