#include "pch.h"
#include "CTextureWindow.h"



CTextureWindow::CTextureWindow()
{
}

CTextureWindow::~CTextureWindow()
{
}

HRESULT CTextureWindow::CreateFrame(void)
{
	ImGui::Begin("Texture");

	for (auto& item : textureKey)
	{
		if(ImGui::Selectable(item.first.c_str()))
			ImGui::SetClipboardText(To_String(item.second).c_str());
		//ImGui::SameLine();
		//ImGui::Text(To_String(item.second).c_str());
	}

	ImGui::End();
	return S_OK;
}
