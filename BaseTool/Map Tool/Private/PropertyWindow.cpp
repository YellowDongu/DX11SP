#include "pch.h"
#include "PropertyWindow.h"


void PropertyWindow::Free(void)
{
}

PropertyWindow* PropertyWindow::Create(ObjectManager* objectManager)
{
    PropertyWindow* newInstance = new PropertyWindow();
    newInstance->LinkData(objectManager);
    return newInstance;
}

void PropertyWindow::update(void)
{

}

HRESULT PropertyWindow::CreateFrame(void)
{
    ImGui::Begin("Property", &active);

    if (selectedData->empty())
    {
        ImGui::End();
        return S_OK;
    }
    GameObjectMetaData& data = *selectedData->front();
    Engine::GameObject*& currentObject = data.gameObject;
    Engine::Transform* transform = currentObject->transform();

    char buffer[1024];
    strncpy_s(buffer, data.nameA.c_str(), sizeof(buffer));

    if (ImGui::InputText("##GameObjectProperty::wstring", buffer, sizeof(buffer)))
    {
        data.nameA = std::string(buffer);
        data.name = To_WString(data.nameA);
    }

    ShowLayer();
    ImGui::SeparatorText("##PropertyNameSeperator");

    std::string nameBuffer = "Position";
    ImVec2 textSize;

    textSize = ImGui::CalcTextSize(nameBuffer.c_str());
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetWindowWidth() / 4.0f - textSize.x);

    ImGui::Text(nameBuffer.c_str()); ImGui::SameLine();
    ImGui::InputFloat3("##Transform::Position", reinterpret_cast<float*>(static_cast<DirectX::XMFLOAT3*>(&transform->Position())));


    nameBuffer = "Angle";
    float3 angle = transform->Angle();
    textSize = ImGui::CalcTextSize(nameBuffer.c_str());
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetWindowWidth() / 4.0f - textSize.x);

    ImGui::Text(nameBuffer.c_str()); ImGui::SameLine();
    if (ImGui::InputFloat3("##Transform::Angle", reinterpret_cast<float*>(static_cast<DirectX::XMFLOAT3*>(&angle))))
        transform->SetAngle(angle);


    nameBuffer = "Scale";
    textSize = ImGui::CalcTextSize(nameBuffer.c_str());
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetWindowWidth() / 4.0f - textSize.x);

    ImGui::Text(nameBuffer.c_str()); ImGui::SameLine();
    ImGui::InputFloat3("##Transform::Scale", reinterpret_cast<float*>(static_cast<DirectX::XMFLOAT3*>(&transform->Scale())));










    ImGui::End();
    return S_OK;


    ShowComponent();
    AddComponent();
    ImGui::End();
    return S_OK;
}

void PropertyWindow::LinkData(ObjectManager* target)
{
    objectManager = target;
    selectedData = &target->Selected();
    gameObjectList = &target->GameObjects();
}

void PropertyWindow::FilterComponent(std::string query)
{
    //componentList.clear();
    //
    //if (strlen(query.data()) == 0)
    //{
    //    for (auto& component : (*cloneManager->Link_List()))
    //    {
    //        componentList.push_back(&component);
    //    }
    //    return;
    //}
    //
    //std::wstring wQuery = To_WString(query);
    //
    //for (auto& component : (*cloneManager->Link_List()))
    //{
    //    if (component.first.find(wQuery.c_str()) != std::wstring::npos)
    //    {
    //        componentList.push_back(&component);
    //    }
    //}
}

Engine::Component* PropertyWindow::AddComponent()
{
    return nullptr;
    /*
    if(!cloneManager)
        cloneManager = CProtoMgr::GetInstance();

    static ImVec2 popupSize = ImVec2(200, 150);
    
    float buttonWidth = ImGui::GetContentRegionAvail().x;
    if (ImGui::Button("Add Component", ImVec2(buttonWidth, 0)))
    {
        ImVec2 buttonPos = ImGui::GetItemRectMin();
        ImVec2 buttonSize = ImGui::GetItemRectSize();
    
        ImVec2 popupPos = ImVec2(buttonPos.x + buttonSize.x / 2.0f - popupSize.x / 2.0f, buttonPos.y + buttonSize.y);
    
        ImGui::SetNextWindowPos(popupPos);
        ImGui::OpenPopup("AddComponent");
    }
    
    
    if (ImGui::BeginPopup("AddComponent"))
    {
        char searchBuffer[128] = "";
        ImGui::SetNextWindowSize(popupSize);
    
        ImGui::InputText("##ComponentSearch", searchBuffer, IM_ARRAYSIZE(searchBuffer));
        FilterComponent(searchBuffer);
    
        ImGui::BeginChild("ItemList", ImVec2(0, 0), true);
        for (const auto& item : componentList)
        {
            if (ImGui::Selectable(To_String(item->first).c_str()))
            {
                CComponent* clone = item->second->Clone();
                CJsonIO::ApplyRenderComponent(item->first, *(*selectedItem)->gameObject, clone);
                (*selectedItem)->gameObject->Add_Component(COMPONENTID::ID_DYNAMIC, item->first, clone);

                ImGui::CloseCurrentPopup();
            }
        }
        ImGui::EndChild();
    
        ImGui::EndPopup();
    }
    return nullptr;
}

void PropertyWindow::ShowComponent(void)
{
    auto& dynamicGameObjects = *(*selectedItem)->gameObject->Get_AllDynamicComponent();
    for (auto iterator = dynamicGameObjects.begin(); iterator != dynamicGameObjects.end(); )
    {
        auto& component = (*iterator);

        std::string componentName = typeid(*component.second).name();
        if (ImGui::CollapsingHeader(componentName.c_str()))
        {
            if (ImGui::BeginMenu(("Menu##" + componentName).c_str()))
            {
                if (ImGui::MenuItem(("Delete##" + componentName).c_str()))
                {
                    for (auto& renderComponent : *(*selectedItem)->gameObject->Get_RenderingComponent())
                    {
                        if (component.second == renderComponent)
                            renderComponent = nullptr;
                    }

                    component.second->Release();
                    component.second = nullptr;
                    iterator = dynamicGameObjects.erase(iterator);

                    ImGui::EndMenu();
                    continue;
                }
                if (ImGui::MenuItem(("Refresh##" + componentName).c_str()))
                    component.second->Refresh_Component();

                ImGui::EndMenu();
            }

            CVariableReader::VariableReaderForImgui(component.second);
        }

        if(component.first == transformkey || component.first == colliderkey)
            component.second->Refresh_Component();


        iterator++;

    }

    auto& staticGameOBjects = *(*selectedItem)->gameObject->Get_AllStaticComponent();
    for (auto iterator = staticGameOBjects.begin(); iterator != staticGameOBjects.end(); )
    {
        auto& component = (*iterator);
        std::string componentName = typeid(*component.second).name();

        if (ImGui::CollapsingHeader(componentName.c_str()))
        {
            if (ImGui::BeginMenu(("Menu##" + componentName).c_str()))
            {
                if (ImGui::MenuItem(("Delete##" + componentName).c_str()))
                {
                    for (auto& renderComponent : *(*selectedItem)->gameObject->Get_RenderingComponent())
                    {
                        if (component.second == renderComponent)
                            renderComponent = nullptr;
                    }

                    component.second->Release();
                    component.second = nullptr;
                    iterator = staticGameOBjects.erase(iterator);

                    ImGui::EndMenu();
                }
                if (ImGui::MenuItem(("Refresh##" + componentName).c_str()))
                    component.second->Refresh_Component();

                ImGui::EndMenu();
            }

            CVariableReader::VariableReaderForImgui(component.second);
        }

        if (component.first == transformkey || component.first == colliderkey)
            component.second->Refresh_Component();

        iterator++;
    }*/
}

void PropertyWindow::ShowComponent(void)
{
}

void PropertyWindow::ShowLayer(void)
{
    /*
    ImGui::Text("Layer");
    ImGui::SameLine();

    std::string layerFirst = "None";

    for (auto& layer : layerList)
    {
        if ((*selectedItem)->layerName == To_String(layer.first))
            layerFirst = To_String(layer.first);
    }
    if(layerFirst == "")
        layerFirst = "None";


    if (ImGui::BeginCombo("##Layer", layerFirst.c_str()))
    {
        for (auto& layer : layerList)
        {
            if ((*selectedItem)->belongingLayer == layer.second)
                ImGui::SetItemDefaultFocus();
            if (ImGui::Selectable(To_String(layer.first).c_str(), false))
            {
                (*selectedItem)->belongingLayer = layer.second;
                (*selectedItem)->layerName = To_String(layer.first);
            }
        }


        if (ImGui::Selectable("Create Layer", false, ImGuiSelectableFlags_DontClosePopups))
            isCreatingLayer = true;

        if (isCreatingLayer)
        {
            ImVec2 menuItemPos = ImGui::GetItemRectMin();
            ImVec2 menuItemSize = ImGui::GetItemRectSize();

            ImVec2 inputPos = ImVec2(menuItemPos.x, menuItemPos.y);
            ImGui::SetNextWindowPos(inputPos);
            ImGui::SetNextWindowSize(ImVec2(200, 30));

            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
            if (ImGui::Begin("Rename GameObject", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize))
            {
                if (ImGui::InputText("##Rename", &layer[0], layer.capacity() + 1))
                    int alarm = 0;
            }
            ImGui::End();

            ImGui::PopStyleVar(2);

            if (Input->getButtonDown(KeyType::Escape) || ImGui::IsKeyPressed(ImGuiKey_Escape))
            {
                layer = "";
                isCreatingLayer = false;
            }
            if (Input->getButton(KeyType::Return) || ImGui::IsKeyPressed(ImGuiKey_Enter))
            {
                if (strlen(layer.data()) != 0)
                    layerList.push_back({ To_WString(layer), CLayer::Create() });

                layer = "";
                isCreatingLayer = false;
            }
        }


        ImGui::EndCombo();
    }*/
}
