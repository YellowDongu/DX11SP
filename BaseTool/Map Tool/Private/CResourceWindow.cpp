#include "pch.h"
#include "CResourceWindow.h"



CResourceWindow::CResourceWindow(void) : prototypeManager(nullptr)
{
}

CResourceWindow::~CResourceWindow(void)
{
}

void CResourceWindow::Start(void)
{
	prototypeManager = CProtoMgr::GetInstance();
}

HRESULT CResourceWindow::CreateFrame(void)
{
	ImGui::Begin("Resources", &active);

    if (ImGui::BeginPopupContextWindow())
    {
        if (ImGui::BeginMenu("Load GameObject"))
        {
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Load Component"))
        {
            ImGui::EndMenu();
        }

        ImGui::EndPopup();
    }
    

    for (auto& component : *prototypeManager->Link_List())
    {
        ImGui::MenuItem(To_String(component.first).c_str());
    }

	ImGui::End();
    return S_OK;
}

CResourceWindow* CResourceWindow::Create(void)
{
	CResourceWindow* newWindow = new CResourceWindow();
	newWindow->Start();
	return newWindow;
}
