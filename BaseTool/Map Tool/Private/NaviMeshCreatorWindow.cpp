#include "pch.h"
#include "NaviMeshCreatorWindow.h"
#include "JsonIO.h"
#include "ToolCamera.h"
#include "ObjectManager.h"
#include "PointDrawer.h"
#include "LineDrawer.h"

void NaviMeshCreatorWindow::Free(void)
{
	Base::DestroyInstance(pointDrawer);
	Base::DestroyInstance(lineDrawer);
	Base::DestroyInstance(naviMesh);
}

NaviMeshCreatorWindow* NaviMeshCreatorWindow::Create(ObjectManager* manager)
{
	NaviMeshCreatorWindow* newInstance = new NaviMeshCreatorWindow();
	newInstance->manager = manager;
	newInstance->Start();
	return newInstance;
}

void NaviMeshCreatorWindow::Start(void)
{
	device = DxDevice();
	context = DxDeviceContext();
	naviMesh = Engine::NavMesh::Create(device, context);
	viewPort = &Device()->ViewPortInfomation();
	pointDrawer = Engine::PointDrawer::Create(device, context, 1.0f);
	lineDrawer = Engine::LineDrawer::Create(device, context);

	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;  // 깊이 테스트 끔
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;

	device->CreateDepthStencilState(&dsDesc, &depthDisabledState);
}

void NaviMeshCreatorWindow::Update(void)
{
	if (!active)
		return;

	if (VertexSetMode)
	{
		if (::Input()->getMouseButtonDown(MouseKey::LButton))
		{
			{
				POINT pt = ::Input()->getMousePosition();
				RECT clientRect;
				::GetClientRect(hWnd, &clientRect);
				if (!::PtInRect(&clientRect, pt))
					return;
			}

			// variable setter
			DirectX::XMVECTOR rayOrigin, rayDir;
			// variable setter

			DirectX::XMVECTOR rayView = DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(((2.0f * ::Input()->getMousePosition().x) / viewPort->Width - 1.0f), ((-2.0f * ::Input()->getMousePosition().y) / viewPort->Height + 1.0f), 1.0f, 1.0f), DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&::EngineInstance()->RenderManager()->ProjectionMatrix())));

			rayOrigin = DirectX::XMVectorSet(0, 0, 0, 1);
			rayDir = DirectX::XMVector3Normalize(rayView);

			DirectX::XMMATRIX invView = DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&::EngineInstance()->RenderManager()->ViewMatrix()));
			rayOrigin = DirectX::XMVector3TransformCoord(rayOrigin, invView);
			rayDir = DirectX::XMVector3Normalize(DirectX::XMVector3TransformNormal(rayDir, invView));

			// variable setter
			Vector3 origin, direction, point, result;
			float shortestDistance = FLT_MAX, calculatedDistance;
			DirectX::XMStoreFloat3(&origin, rayOrigin);
			DirectX::XMStoreFloat3(&direction, rayDir);

			Engine::Component* component = nullptr;
			//Engine::Model* model = nullptr;
			Engine::StaticModel* staticModel = nullptr;
			std::wstring modelString = L"Model", staticModelString = L"StaticModel";
			// variable setter


			for (auto& object : manager->GameObjects())
			{
				if (object->gameObject == nullptr)
					continue;
				//component = object->gameObject->GetComponent(modelString);
				//
				//if (component != nullptr)
				//{
				//	model = static_cast<Engine::Model*>(component);
				//	if (model->RayToMeshCollision(origin, direction, point))
				//	{
				//		calculatedDistance = (origin - point).magnitude();
				//		if (shortestDistance > calculatedDistance)
				//		{
				//			result = point;
				//			shortestDistance = calculatedDistance;
				//		}
				//	}
				//	continue;
				//}
				component = object->gameObject->GetComponent(staticModelString);
				if (component != nullptr)
				{
					staticModel = static_cast<Engine::StaticModel*>(component);

					if (staticModel->RayToMeshCollision(origin, direction, point))
					{
						calculatedDistance = (origin - point).magnitude();
						if (shortestDistance > calculatedDistance)
						{
							result = point;
							shortestDistance = calculatedDistance;
						}
					}
					continue;
				}

			}

			if (shortestDistance == FLT_MAX)
				return;


			// variable setter
			INT index;
			Vector3 snapPosition;
			// variable setter
			if (CheckSnap(result, snapPosition, index))
			{
				if (selectedPointIndex == -1 || selectedPointIndex == index)
				{
					selectedPoint = snapPosition;
					selectedPointIndex = index;
					return;
				}


				if (!CheckExsist(selectedPointIndex, index))
				{
					lines.push_back({ static_cast<UINT>(selectedPointIndex), static_cast<UINT>(index) });

				}

				selectedPoint = Vector3::one() * -1.0f;
				selectedPointIndex = -1;
			}
			else
			{
				result.y += 0.1f;
				vertices.push_back(result);
				selectedPoint = result;
				selectedPointIndex = static_cast<UINT>(vertices.size()) - 1;

			}
		}
		else if (::Input()->getMouseButtonDown(MouseKey::RButton))
		{
			{
				POINT pt = ::Input()->getMousePosition();
				RECT clientRect;
				::GetClientRect(hWnd, &clientRect);
				if (!::PtInRect(&clientRect, pt))
					return;
			}

			// variable setter
			DirectX::XMVECTOR rayOrigin, rayDir;
			// variable setter

			DirectX::XMVECTOR rayView = DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(((2.0f * ::Input()->getMousePosition().x) / viewPort->Width - 1.0f), ((-2.0f * ::Input()->getMousePosition().y) / viewPort->Height + 1.0f), 1.0f, 1.0f), DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&::EngineInstance()->RenderManager()->ProjectionMatrix())));

			rayOrigin = DirectX::XMVectorSet(0, 0, 0, 1);
			rayDir = DirectX::XMVector3Normalize(rayView);

			DirectX::XMMATRIX invView = DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&::EngineInstance()->RenderManager()->ViewMatrix()));
			rayOrigin = DirectX::XMVector3TransformCoord(rayOrigin, invView);
			rayDir = DirectX::XMVector3Normalize(DirectX::XMVector3TransformNormal(rayDir, invView));

			// variable setter
			Vector3 origin, direction, point, result;
			float shortestDistance = FLT_MAX, calculatedDistance;
			DirectX::XMStoreFloat3(&origin, rayOrigin);
			DirectX::XMStoreFloat3(&direction, rayDir);

			Engine::Component* component = nullptr;
			//Engine::Model* model = nullptr;
			Engine::StaticModel* staticModel = nullptr;
			std::wstring modelString = L"Model", staticModelString = L"StaticModel";
			// variable setter


			for (auto& object : manager->GameObjects())
			{
				if (object->gameObject == nullptr)
					continue;
				//component = object->gameObject->GetComponent(modelString);
				//
				//if (component != nullptr)
				//{
				//	model = static_cast<Engine::Model*>(component);
				//	if (model->RayToMeshCollision(origin, direction, point))
				//	{
				//		calculatedDistance = (origin - point).magnitude();
				//		if (shortestDistance > calculatedDistance)
				//		{
				//			result = point;
				//			shortestDistance = calculatedDistance;
				//		}
				//	}
				//	continue;
				//}
				component = object->gameObject->GetComponent(staticModelString);
				if (component != nullptr)
				{
					staticModel = static_cast<Engine::StaticModel*>(component);

					if (staticModel->RayToMeshCollision(origin, direction, point))
					{
						calculatedDistance = (origin - point).magnitude();
						if (shortestDistance > calculatedDistance)
						{
							result = point;
							shortestDistance = calculatedDistance;
						}
					}
					continue;
				}

			}

			if (shortestDistance == FLT_MAX)
				return;

			// variable setter
			INT index = -1;
			bool deleted = false;
			Vector3 snapPosition;
			// variable setter

			if (CheckSnap(result, snapPosition, index))
			{
				for (auto iterator = lines.begin(); iterator != lines.end(); )
				{
					if (iterator->first == index || iterator->second == index)
					{
						iterator = lines.erase(iterator);
						deleted = true;
						continue;
					}

					iterator++;
				}

				selectedPoint = Vector3::one() * -1.0f;
				selectedPointIndex = -1;
				//vertices.erase(selectedIterator);
				//*selectedIterator = Vector3::zero();
			}

		}


	}
}

void NaviMeshCreatorWindow::Render(void)
{
	if (!active)
		return;
	Vector3 position;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> oldState;
	UINT oldStencilRef;
	context->OMGetDepthStencilState(&oldState, &oldStencilRef);
	context->OMSetDepthStencilState(depthDisabledState.Get(), 0);
	if (render)
	{
		pointDrawer->SetColor(DirectX::Colors::Green);
		lineDrawer->SetColor(DirectX::Colors::Green);
		if (selectedPointIndex != -1)
		{
			for (auto& point : vertices)
			{
				if (point == selectedPoint)
				{
					position = point + Vector3(0.0f, 0.5f, 0.0f);
					pointDrawer->SetColor(float4(1.0f, 1.0f, 0.0f, 1.0f));
					pointDrawer->SetPosition(position);
					pointDrawer->Render();
					pointDrawer->SetColor(DirectX::Colors::Green);
				}
				else
				{
					position = point + Vector3(0.0f, 0.5f, 0.0f);
					pointDrawer->SetPosition(position);
					pointDrawer->Render();
				}
			}
		}
		else
		{
			for (auto& point : vertices)
			{
				position = point + Vector3(0.0f, 0.5f, 0.0f);
				pointDrawer->SetPosition(position);
				pointDrawer->Render();
			}
		}
		for (auto& line : lines)
		{
			lineDrawer->ClearLines();
			//position = vertices[line.first] + Vector3(0.0f, 0.5f, 0.0f);
			//lineDrawer->AddPosition(position);
			lineDrawer->AddPosition(vertices[line.first]);
			//position = vertices[line.second] + Vector3(0.0f, 0.5f, 0.0f);
			//lineDrawer->AddPosition(position);
			lineDrawer->AddPosition(vertices[line.second]);
			lineDrawer->Render();
		}
	}

	if (naviRender)
	{
		lineDrawer->SetColor(DirectX::Colors::Red);
		pointDrawer->SetColor(DirectX::Colors::Red);
		for (auto& node : naviMesh->GetAllNodes())
		{
			lineDrawer->ClearLines();
			lineDrawer->AddPosition(node.vertices[0]);
			lineDrawer->AddPosition(node.vertices[1]);
			//lineDrawer->Render();
			//lineDrawer->ClearLines();
			//lineDrawer->AddPosition(node.vertices[1]);
			//lineDrawer->AddPosition(node.vertices[2]);
			//lineDrawer->Render();
			//lineDrawer->ClearLines();
			lineDrawer->AddPosition(node.vertices[2]);
			lineDrawer->AddPosition(node.vertices[0]);
			lineDrawer->Render();

			pointDrawer->SetPosition(node.vertices[0]);
			pointDrawer->Render();
			pointDrawer->SetPosition(node.vertices[1]);
			pointDrawer->Render();
			pointDrawer->SetPosition(node.vertices[2]);
			pointDrawer->Render();
		}
	}

	context->OMSetDepthStencilState(oldState.Get(), oldStencilRef);
}

HRESULT NaviMeshCreatorWindow::CreateFrame(void)
{
	ImGui::Begin("NaviMesh", &active, ImGuiWindowFlags_MenuBar);
	CreateBasicMenu();

	ImGui::Text("SnapDistance"); ImGui::SameLine();
	ImGui::InputFloat("##SnapDistance::NaviMesh", &snapDistance);


	ImGui::End();
	return S_OK;
}

void NaviMeshCreatorWindow::CreateBasicMenu(void)
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Status##NaviMesh"))
		{
			ImGui::Selectable("VertexSetMode", &VertexSetMode);
			ImGui::Selectable("Render", &render);
			ImGui::Selectable("RenderBaked", &naviRender);
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("LoadNavMesh"))
		{
			std::wstring path = CJsonIO::selectLoadFilePath();
			if (path.size() != 0)
				naviMesh->LoadMesh(path);
		}
		if (ImGui::BeginMenu("Save/Load##NaviMesh"))
		{
			if (ImGui::MenuItem("Save"))
			{
				std::wstring path = CJsonIO::selectSaveFilePath();
				if (path.size() != 0)
					naviMesh->SaveMesh(path, verticesAfterBaked, indicesAfterBaked);
			}
			if (ImGui::MenuItem("Load"))
			{

				std::wstring path = CJsonIO::selectLoadFilePath();
				if (path.size() != 0)
				{
					verticesAfterBaked.clear();
					indicesAfterBaked.clear();
					naviMesh->LoadMesh(path, verticesAfterBaked, indicesAfterBaked);

					vertices.clear();
					for (auto& point : verticesAfterBaked)
					{
						vertices.push_back(point);
					}
					for (size_t i = 0; i < indicesAfterBaked.size(); i += 3)
					{
						lines.push_back({ indicesAfterBaked[i],		indicesAfterBaked[ i + 1 ] });
						lines.push_back({ indicesAfterBaked[i + 1],	indicesAfterBaked[ i + 2] });
						lines.push_back({ indicesAfterBaked[i + 2],	indicesAfterBaked[i] });
					}

				}
			}
			if (ImGui::MenuItem("Temp"))
			{
				std::wstring path = CJsonIO::selectSaveFilePath();

				if (path.size() != 0)
				{
					std::vector<UINT> indexTempStorage = { 0 ,9 ,11 ,0 ,36 ,9 ,0 ,11 ,10 ,0 ,10 ,41 ,0 ,33 ,36 ,0 ,41 ,33 ,1 ,30 ,29 ,1 ,29 ,32 ,1 ,33 ,30 ,1 ,32 ,34 ,1 ,34 ,33 ,2 ,3 ,6 ,2 ,65 ,3 ,2 ,6 ,32 ,2 ,29 ,28 ,2 ,28 ,65 ,2 ,32 ,29 ,3 ,8 ,6 ,3 ,62 ,8 ,3 ,61 ,62 ,3 ,65 ,61 ,4 ,5 ,6 ,4 ,7 ,5 ,4 ,6 ,8 ,4 ,79 ,7 ,4 ,8 ,99 ,4 ,99 ,79 ,5 ,32 ,6 ,5 ,7 ,70 ,5 ,34 ,32 ,5 ,35 ,34 ,5 ,70 ,35 ,7 ,73 ,70 ,7 ,72 ,73 ,7 ,83 ,72 ,7 ,79 ,83 ,8 ,62 ,63 ,8 ,63 ,113 ,8 ,112 ,99 ,8 ,113 ,112 ,9 ,43 ,11 ,9 ,36 ,13 ,9 ,13 ,43 ,10 ,11 ,31 ,10 ,31 ,41 ,11 ,37 ,31 ,11 ,42 ,37 ,11 ,43 ,42 ,12 ,13 ,36 ,12 ,36 ,38 ,12 ,38 ,96 ,12 ,96 ,97 ,13 ,98 ,43 ,14 ,26 ,23 ,14 ,23 ,67 ,14 ,29 ,26 ,14 ,28 ,29 ,14 ,68 ,28 ,14 ,67 ,68 ,15 ,16 ,47 ,15 ,25 ,23 ,15 ,23 ,26 ,15 ,47 ,25 ,15 ,26 ,37 ,15 ,37 ,39 ,16 ,19 ,17 ,16 ,17 ,47 ,16 ,20 ,19 ,16 ,40 ,20 ,17 ,19 ,18 ,17 ,18 ,92 ,17 ,92 ,47 ,18 ,19 ,45 ,18 ,164 ,92 ,18 ,162 ,164 ,19 ,20 ,45 ,20 ,40 ,39 ,20 ,39 ,44 ,20 ,44 ,45 ,21 ,25 ,47 ,21 ,69 ,25 ,21 ,47 ,93 ,21 ,159 ,69 ,21 ,93 ,134 ,21 ,134 ,159 ,22 ,23 ,25 ,22 ,67 ,23 ,22 ,25 ,69 ,22 ,66 ,67 ,22 ,123 ,66 ,22 ,69 ,124 ,22 ,124 ,123 ,24 ,27 ,68 ,24 ,122 ,27 ,24 ,68 ,66 ,24 ,66 ,123 ,24 ,123 ,122 ,26 ,29 ,30 ,26 ,30 ,31 ,26 ,31 ,37 ,27 ,60 ,28 ,27 ,28 ,68 ,27 ,64 ,60 ,27 ,122 ,64 ,28 ,60 ,65 ,30 ,41 ,31 ,30 ,33 ,41 ,33 ,34 ,35 ,33 ,35 ,36 ,35 ,38 ,36 ,35 ,96 ,38 ,35 ,70 ,95 ,35 ,95 ,96 ,37 ,42 ,39 ,39 ,42 ,44 ,42 ,43 ,46 ,42 ,46 ,44 ,43 ,161 ,46 ,43 ,98 ,160 ,43 ,160 ,161 ,44 ,46 ,45 ,45 ,46 ,161 ,47 ,92 ,91 ,47 ,91 ,93 ,48 ,54 ,49 ,48 ,50 ,54 ,48 ,75 ,50 ,48 ,51 ,75 ,48 ,76 ,51 ,50 ,53 ,54 ,50 ,104 ,53 ,50 ,75 ,105 ,50 ,105 ,104 ,51 ,74 ,52 ,51 ,52 ,75 ,51 ,84 ,74 ,51 ,76 ,84 ,52 ,74 ,78 ,52 ,105 ,75 ,52 ,78 ,107 ,52 ,106 ,105 ,52 ,107 ,106 ,53 ,55 ,54 ,53 ,59 ,55 ,53 ,56 ,59 ,53 ,104 ,56 ,55 ,59 ,57 ,55 ,57 ,89 ,55 ,89 ,77 ,56 ,58 ,59 ,56 ,108 ,58 ,56 ,104 ,100 ,56 ,100 ,108 ,57 ,59 ,58 ,57 ,173 ,89 ,58 ,108 ,172 ,60 ,62 ,61 ,60 ,61 ,65 ,60 ,63 ,62 ,60 ,64 ,63 ,63 ,64 ,129 ,64 ,122 ,121 ,64 ,121 ,129 ,66 ,68 ,67 ,69 ,159 ,124 ,70 ,73 ,223 ,70 ,224 ,95 ,70 ,223 ,224 ,71 ,86 ,87 ,71 ,175 ,86 ,71 ,87 ,88 ,71 ,88 ,89 ,71 ,89 ,174 ,71 ,174 ,175 ,72 ,90 ,73 ,72 ,83 ,82 ,72 ,82 ,90 ,73 ,90 ,223 ,74 ,81 ,78 ,74 ,84 ,81 ,76 ,85 ,84 ,76 ,87 ,85 ,76 ,88 ,87 ,77 ,89 ,88 ,78 ,81 ,80 ,78 ,80 ,111 ,78 ,111 ,107 ,79 ,80 ,83 ,79 ,99 ,80 ,80 ,81 ,83 ,80 ,99 ,111 ,81 ,82 ,83 ,81 ,84 ,82 ,82 ,84 ,85 ,82 ,85 ,90 ,85 ,87 ,86 ,85 ,86 ,90 ,86 ,175 ,90 ,89 ,173 ,174 ,90 ,175 ,220 ,90 ,220 ,223 ,91 ,92 ,165 ,91 ,176 ,93 ,91 ,165 ,185 ,91 ,185 ,176 ,92 ,164 ,165 ,93 ,176 ,134 ,94 ,164 ,163 ,94 ,163 ,198 ,94 ,166 ,184 ,94 ,196 ,166 ,94 ,198 ,196 ,95 ,224 ,96 ,96 ,225 ,97 ,96 ,224 ,225 ,97 ,160 ,98 ,97 ,225 ,160 ,99 ,112 ,111 ,100 ,101 ,103 ,100 ,106 ,101 ,100 ,103 ,109 ,100 ,104 ,105 ,100 ,105 ,106 ,100 ,109 ,108 ,101 ,102 ,103 ,101 ,119 ,102 ,101 ,106 ,110 ,101 ,110 ,119 ,102 ,169 ,103 ,102 ,119 ,116 ,102 ,116 ,168 ,102 ,168 ,169 ,103 ,170 ,109 ,103 ,169 ,170 ,106 ,107 ,110 ,107 ,111 ,110 ,108 ,109 ,171 ,108 ,171 ,172 ,109 ,170 ,171 ,110 ,111 ,119 ,111 ,112 ,120 ,111 ,120 ,119 ,112 ,113 ,120 ,113 ,117 ,120 ,113 ,130 ,117 ,114 ,118 ,117 ,114 ,117 ,130 ,114 ,131 ,118 ,114 ,129 ,128 ,114 ,128 ,131 ,114 ,130 ,129 ,115 ,116 ,118 ,115 ,168 ,116 ,115 ,118 ,131 ,115 ,131 ,142 ,115 ,142 ,140 ,115 ,140 ,167 ,115 ,167 ,168 ,116 ,117 ,118 ,116 ,119 ,117 ,117 ,119 ,120 ,121 ,122 ,145 ,121 ,128 ,129 ,121 ,144 ,128 ,121 ,145 ,144 ,122 ,123 ,125 ,122 ,125 ,145 ,123 ,124 ,149 ,123 ,149 ,125 ,124 ,158 ,149 ,124 ,159 ,158 ,125 ,126 ,146 ,125 ,147 ,126 ,125 ,146 ,145 ,125 ,149 ,147 ,126 ,143 ,127 ,126 ,127 ,146 ,126 ,141 ,143 ,126 ,148 ,141 ,126 ,147 ,148 ,127 ,143 ,128 ,127 ,128 ,144 ,127 ,144 ,146 ,128 ,143 ,131 ,131 ,143 ,142 ,132 ,135 ,154 ,132 ,180 ,135 ,132 ,157 ,138 ,132 ,138 ,179 ,132 ,154 ,157 ,132 ,179 ,180 ,133 ,158 ,134 ,133 ,134 ,177 ,133 ,138 ,157 ,133 ,178 ,138 ,133 ,157 ,158 ,133 ,177 ,178 ,134 ,158 ,159 ,134 ,176 ,177 ,135 ,137 ,153 ,135 ,153 ,154 ,136 ,153 ,137 ,136 ,139 ,148 ,136 ,148 ,152 ,136 ,152 ,153 ,137 ,182 ,181 ,138 ,178 ,179 ,139 ,140 ,141 ,139 ,167 ,140 ,139 ,141 ,148 ,139 ,181 ,167 ,140 ,142 ,141 ,141 ,142 ,143 ,144 ,145 ,146 ,147 ,151 ,148 ,147 ,149 ,150 ,147 ,150 ,151 ,148 ,151 ,152 ,149 ,155 ,150 ,149 ,158 ,155 ,150 ,156 ,151 ,150 ,155 ,156 ,151 ,156 ,152 ,152 ,154 ,153 ,152 ,156 ,154 ,154 ,156 ,157 ,155 ,157 ,156 ,155 ,158 ,157 ,160 ,226 ,161 ,160 ,225 ,226 ,161 ,226 ,162 ,162 ,163 ,164 ,162 ,226 ,163 ,163 ,227 ,198 ,163 ,226 ,227 ,165 ,166 ,186 ,165 ,186 ,185 ,166 ,196 ,186 ,167 ,193 ,168 ,167 ,181 ,183 ,167 ,183 ,193 ,168 ,204 ,169 ,168 ,193 ,205 ,168 ,205 ,204 ,169 ,214 ,170 ,169 ,204 ,214 ,170 ,214 ,171 ,171 ,216 ,172 ,171 ,214 ,216 ,172 ,217 ,173 ,172 ,216 ,217 ,173 ,218 ,174 ,173 ,217 ,218 ,174 ,220 ,175 ,174 ,218 ,220 ,176 ,185 ,177 ,177 ,199 ,178 ,177 ,185 ,199 ,178 ,188 ,179 ,178 ,199 ,188 ,179 ,187 ,180 ,179 ,188 ,187 ,180 ,189 ,182 ,180 ,187 ,189 ,181 ,182 ,183 ,182 ,191 ,183 ,182 ,189 ,191 ,183 ,191 ,194 ,183 ,194 ,193 ,185 ,186 ,200 ,185 ,200 ,199 ,186 ,196 ,197 ,186 ,197 ,200 ,189 ,190 ,191 ,190 ,202 ,191 ,190 ,207 ,202 ,191 ,202 ,194 ,193 ,194 ,203 ,193 ,203 ,205 ,194 ,202 ,203 ,195 ,201 ,197 ,195 ,198 ,227 ,195 ,237 ,201 ,195 ,227 ,237 ,197 ,201 ,200 ,202 ,209 ,203 ,202 ,207 ,209 ,203 ,206 ,205 ,203 ,209 ,206 ,204 ,205 ,206 ,204 ,206 ,208 ,204 ,208 ,234 ,204 ,215 ,214 ,204 ,234 ,215 ,206 ,209 ,208 ,207 ,208 ,209 ,210 ,216 ,215 ,210 ,215 ,234 ,210 ,217 ,216 ,210 ,235 ,217 ,210 ,234 ,235 ,214 ,215 ,216 ,217 ,233 ,218 ,217 ,235 ,233 ,218 ,233 ,220 ,220 ,232 ,223 ,220 ,233 ,232 ,223 ,231 ,224 ,223 ,232 ,231 ,224 ,230 ,225 ,224 ,231 ,230 ,225 ,229 ,226 ,225 ,230 ,229 ,226 ,228 ,227 ,226 ,229 ,228 ,228 ,229 ,239 ,228 ,239 ,238 ,229 ,230 ,236 ,229 ,236 ,239 ,230 ,231 ,236 ,231 ,232 ,241 ,231 ,241 ,236 ,232 ,233 ,241 ,236 ,240 ,239 ,238 ,239 ,240 };
					std::vector<Vector3> tempStorage = { {299.104767f ,19.7080765f ,1773.01843f } ,{285.735260f ,23.0645676f ,1771.29529f } ,{268.904327f ,28.0051441f ,1763.87183f } ,{257.735260f ,29.0460129f ,1774.02625f } ,{248.055313f ,16.4215965f ,1800.01184f } ,{269.486023f ,12.2314548f ,1799.09131f } ,{258.254578f ,20.7827168f ,1785.26196f } ,{247.398972f ,12.3684568f ,1805.26733f } ,{242.129898f ,25.3067970f ,1783.10413f } ,{304.410278f ,17.9844189f ,1774.33557f } ,{301.806274f ,19.3282547f ,1767.62158f } ,{304.080627f ,16.5853176f ,1769.51013f } ,{302.389374f ,11.3613043f ,1781.80408f } ,{304.910370f ,15.3373222f ,1776.41370f } ,{277.529541f ,18.4362526f ,1748.79358f } ,{293.675293f ,10.1777000f ,1743.20422f } ,{308.600342f ,4.92870092f ,1741.45044f } ,{321.624786f ,1.96864259f ,1736.24451f } ,{331.414276f ,0.487177730f ,1738.19751f } ,{323.607544f ,5.59520006f ,1742.58228f } ,{316.392517f ,8.64998531f ,1747.04443f } ,{288.270447f ,6.48669672f ,1719.62964f } ,{271.570709f ,21.3283405f ,1733.31641f } ,{277.378815f ,18.9286575f ,1738.61731f } ,{266.436462f ,22.8234158f ,1740.30444f } ,{287.205475f ,11.1037598f ,1738.22375f } ,{285.389191f ,15.8884010f ,1750.90601f } ,{260.853363f ,28.4099483f ,1750.41931f } ,{266.169617f ,28.0464630f ,1757.22498f } ,{276.209381f ,24.4161034f ,1765.01526f } ,{286.901062f ,22.5738335f ,1764.83154f } ,{298.139191f ,20.2147160f ,1760.78467f } ,{271.852692f ,20.6744099f ,1777.16699f } ,{291.517059f ,21.4011154f ,1774.04639f } ,{277.400513f ,14.1705465f ,1787.87061f } ,{284.904419f ,6.60638905f ,1800.60938f } ,{296.168579f ,16.0409069f ,1780.25977f } ,{301.650238f ,17.0258560f ,1760.11975f } ,{296.628876f ,10.6554422f ,1788.13281f } ,{306.628021f ,10.7579832f ,1753.04517f } ,{307.533539f ,9.59673309f ,1747.91516f } ,{294.755035f ,21.7299213f ,1768.65991f } ,{312.530670f ,6.52411127f ,1762.87683f } ,{314.092621f ,5.68632317f ,1771.18311f } ,{315.401184f ,4.92928934f ,1755.87720f } ,{324.274414f ,2.40849590f ,1750.91919f } ,{321.319519f ,2.38100815f ,1762.38818f } ,{305.889069f ,3.36498022f ,1725.99731f } ,{206.669449f ,14.1022539f ,1823.20654f } ,{204.938751f ,11.2144699f ,1828.49451f } ,{198.614075f ,12.8814945f ,1821.51306f } ,{210.072968f ,13.0121269f ,1821.57434f } ,{207.638733f ,11.5231085f ,1813.54138f } ,{193.705383f ,8.90176010f ,1825.64185f } ,{202.328506f ,12.5313034f ,1825.98999f } ,{194.158234f ,4.98072243f ,1832.96985f } ,{182.411865f ,2.59225559f ,1823.53735f } ,{185.380234f ,1.60425532f ,1836.82202f } ,{178.651077f ,1.01394045f ,1832.08826f } ,{185.830856f ,3.69332266f ,1830.63196f } ,{256.374695f ,28.3874264f ,1762.67834f } ,{257.205750f ,28.9113312f ,1769.03528f } ,{250.320633f ,26.8827343f ,1773.59460f } ,{242.539627f ,29.7130756f ,1769.43701f } ,{248.494293f ,28.9639435f ,1760.34314f } ,{264.147858f ,28.7817554f ,1763.55762f } ,{268.600311f ,22.1942120f ,1736.76929f } ,{271.688751f ,20.9660740f ,1739.34961f } ,{268.106781f ,22.5163155f ,1745.40259f } ,{277.214325f ,14.4746456f ,1727.86719f } ,{267.793640f ,1.27102053f ,1819.62793f } ,{207.171600f ,0.524531245f ,1840.54858f } ,{235.792206f ,3.28937483f ,1823.41479f } ,{252.858994f ,0.886513650f ,1826.55017f } ,{213.241989f ,12.3486357f ,1815.60229f } ,{203.384216f ,13.3125029f ,1818.51111f } ,{208.095001f ,11.7646751f ,1827.04553f } ,{202.875076f ,8.19966793f ,1831.35388f } ,{216.971863f ,13.1307449f ,1810.82837f } ,{234.904114f ,16.3286762f ,1805.40247f } ,{226.416092f ,14.4236593f ,1805.74829f } ,{225.758896f ,8.95875740f ,1813.42542f } ,{228.045578f ,3.98826146f ,1822.69763f } ,{231.458847f ,11.3814402f ,1812.27124f } ,{219.114807f ,6.60079813f ,1823.00806f } ,{214.953445f ,1.33357668f ,1833.99182f } ,{213.477570f ,0.102941893f ,1840.29077f } ,{210.624802f ,4.26167727f ,1833.11768f } ,{205.785431f ,6.76620340f ,1832.41028f } ,{194.996536f ,1.03693354f ,1840.44897f } ,{230.048431f ,0.101635747f ,1837.63281f } ,{318.327728f ,0.459111303f ,1711.69727f } ,{324.713074f ,0.600166023f ,1726.19482f } ,{300.243805f ,0.969648421f ,1704.04126f } ,{360.065552f ,0.0999218747f ,1722.00610f } ,{285.846863f ,0.462275386f ,1808.74170f } ,{302.993927f ,2.02565432f ,1799.96729f } ,{313.190369f ,1.69799316f ,1788.25867f } ,{314.495819f ,3.95093727f ,1779.52979f } ,{233.440094f ,18.3591747f ,1795.33252f } ,{183.669678f ,2.26662588f ,1811.18469f } ,{196.147202f ,6.40299273f ,1796.87573f } ,{185.404877f ,2.07182121f ,1787.95544f } ,{179.812958f ,1.26009274f ,1799.57617f } ,{191.958435f ,8.57629395f ,1817.99548f } ,{199.156418f ,10.0037870f ,1814.04492f } ,{205.169617f ,9.36827946f ,1805.60889f } ,{213.770813f ,11.8444138f ,1806.15381f } ,{175.322998f ,0.788403332f ,1822.75806f } ,{177.459549f ,0.891098619f ,1811.34631f } ,{208.700577f ,9.60770035f ,1798.89990f } ,{222.152618f ,14.2821636f ,1796.94104f } ,{231.113510f ,21.7080173f ,1788.09814f } ,{231.354553f ,25.3892040f ,1773.29822f } ,{228.149216f ,24.9058151f ,1752.51782f } ,{201.231461f ,5.06549788f ,1749.25586f } ,{200.349915f ,5.91759253f ,1765.63647f } ,{221.121536f ,19.8017139f ,1763.56213f } ,{212.187881f ,12.8766651f ,1760.16016f } ,{207.495972f ,12.5352783f ,1776.89648f } ,{222.419525f ,18.3375931f ,1780.02234f } ,{237.457886f ,30.0056572f ,1744.82214f } ,{251.829727f ,25.9474144f ,1744.35205f } ,{253.739822f ,19.3041115f ,1730.35657f } ,{264.258698f ,15.5592728f ,1725.81775f } ,{241.846024f ,20.6481743f ,1728.91479f } ,{229.182709f ,19.7483292f ,1730.30322f } ,{228.754181f ,24.3845749f ,1741.54761f } ,{228.267471f ,26.3679638f ,1749.37012f } ,{234.649368f ,28.3726177f ,1753.79297f } ,{231.018829f ,26.6720123f ,1764.35913f } ,{217.539032f ,16.0858688f ,1750.14038f } ,{246.107727f ,1.24622071f ,1676.80042f } ,{266.697937f ,1.22311032f ,1681.78540f } ,{284.485504f ,2.50095677f ,1694.72620f } ,{229.582581f ,1.32107425f ,1681.25671f } ,{220.154434f ,3.45932102f ,1700.50928f } ,{212.107391f ,0.156611323f ,1687.83337f } ,{258.422638f ,1.56841302f ,1679.16309f } ,{214.173035f ,2.94426751f ,1714.82642f } ,{201.005157f ,2.81114244f ,1735.37170f } ,{216.768967f ,9.55991745f ,1732.63293f } ,{212.179962f ,9.60241222f ,1741.84741f } ,{223.347000f ,21.8181152f ,1742.01733f } ,{233.188660f ,27.8318481f ,1741.92126f } ,{239.288361f ,29.7017479f ,1741.85156f } ,{233.893387f ,25.3223038f ,1737.11829f } ,{238.576797f ,17.5337791f ,1721.22876f } ,{229.941559f ,12.2104740f ,1716.51868f } ,{250.757828f ,16.1960697f ,1716.96570f } ,{246.341736f ,15.6503906f ,1709.69727f } ,{237.605301f ,15.1337938f ,1714.17700f } ,{233.735443f ,8.50101089f ,1701.42200f } ,{228.045029f ,3.68969703f ,1692.83826f } ,{241.597672f ,6.62750483f ,1690.63574f } ,{249.812256f ,13.8798199f ,1703.54211f } ,{243.735458f ,13.1634274f ,1704.23340f } ,{251.871399f ,9.28566933f ,1694.74915f } ,{262.177826f ,10.5823050f ,1708.77795f } ,{275.733734f ,6.26938963f ,1712.67432f } ,{323.774475f ,0.642836928f ,1781.70386f } ,{332.216980f ,0.157724604f ,1767.62463f } ,{342.304901f ,0.0999218747f ,1752.62366f } ,{358.689209f ,0.0999267623f ,1740.11462f } ,{347.320068f ,0.0999267623f ,1728.94702f } ,{337.553223f ,0.0999267623f ,1704.30249f } ,{352.481018f ,0.0999218747f ,1707.86963f } ,{172.327316f ,0.0999218747f ,1729.31750f } ,{169.704956f ,0.132636726f ,1758.21228f } ,{162.933487f ,0.196142584f ,1785.50061f } ,{161.548004f ,0.0999267623f ,1802.67810f } ,{159.019791f ,0.0999243185f ,1821.42603f } ,{161.997925f ,0.0999145508f ,1838.65369f } ,{179.888184f ,0.0999267623f ,1851.49011f } ,{198.086456f ,0.0999194384f ,1851.33264f } ,{213.819260f ,0.100131840f ,1849.58154f } ,{306.988220f ,0.0999218747f ,1691.69604f } ,{293.247406f ,0.0999267623f ,1676.97388f } ,{272.694458f ,0.100297853f ,1663.77124f } ,{250.958633f ,0.109780274f ,1657.87280f } ,{218.763733f ,0.0999316424f ,1663.28601f } ,{196.585693f ,0.0999267623f ,1701.73840f } ,{194.925781f ,0.0999169946f ,1677.08032f } ,{170.440323f ,0.0999365225f ,1698.88855f } ,{343.354828f ,0.0999218747f ,1717.99194f } ,{331.864929f ,0.0999267623f ,1678.07996f } ,{362.198517f ,0.0999316424f ,1680.93982f } ,{223.386597f ,0.102514647f ,1631.92664f } ,{253.067459f ,0.126220703f ,1631.09399f } ,{180.924622f ,0.0999267623f ,1630.20081f } ,{128.146515f ,0.0999267623f ,1625.79016f } ,{160.401459f ,0.0999218747f ,1668.54346f } ,{173.271454f ,0.0999267623f ,1677.78015f } ,{152.599213f ,0.0999218747f ,1725.24585f } ,{141.502274f ,0.0999267623f ,1691.68445f } ,{417.532928f ,0.0999267623f ,1706.59741f } ,{376.518341f ,0.0999267623f ,1702.70386f } ,{391.429108f ,0.0999316424f ,1680.68872f } ,{386.813477f ,0.0999316424f ,1717.20483f } ,{300.438477f ,0.100673832f ,1630.98950f } ,{365.191132f ,0.0999267623f ,1634.06580f } ,{431.655640f ,0.0999316424f ,1637.98547f } ,{123.016884f ,0.0999316424f ,1669.68665f } ,{116.608536f ,0.0999365225f ,1718.88745f } ,{105.573761f ,0.0999316424f ,1786.09338f } ,{127.282028f ,0.0999218747f ,1749.31262f } ,{95.2547379f ,0.0999169946f ,1750.14978f } ,{70.0284882f ,0.0999267623f ,1670.31140f } ,{51.1201591f ,0.0999218747f ,1753.18616f } ,{88.2909470f ,0.0999267623f ,1702.96924f } ,{97.4224014f ,0.0999218747f ,1850.18945f } ,{131.077881f ,0.0999169946f ,1817.70984f } ,{151.995010f ,0.0999169946f ,1802.00171f } ,{134.069473f ,0.0999218747f ,1795.57605f } ,{140.599518f ,0.0999169946f ,1803.41138f } ,{103.114082f ,0.0999218747f ,1819.95715f } ,{135.260056f ,0.0999218747f ,1830.84766f } ,{139.904678f ,0.0999218747f ,1868.79944f } ,{180.856506f ,0.0999267623f ,1882.67834f } ,{179.506638f ,0.0999169946f ,1885.46997f } ,{220.516968f ,0.0999169946f ,1880.87170f } ,{181.710800f ,0.0999169946f ,1879.60571f } ,{183.584839f ,0.0999182165f ,1884.16589f } ,{276.744812f ,0.0999121144f ,1865.17664f } ,{309.100494f ,0.0999316424f ,1845.73157f } ,{338.089172f ,0.0999169946f ,1822.45715f } ,{368.449219f ,0.0999218747f ,1787.96350f } ,{401.194763f ,0.0999169946f ,1748.13525f } ,{401.632416f ,0.0999218747f ,1792.93225f } ,{369.746643f ,0.0999121144f ,1827.19641f } ,{337.704803f ,0.0999218747f ,1857.37781f } ,{301.226776f ,0.0999121144f ,1879.79858f } ,{251.185822f ,0.0999121144f ,1908.47498f } ,{175.220367f ,0.0999316424f ,1937.80933f } ,{42.1233177f ,0.0999218747f ,1827.89087f } ,{34.5817833f ,0.0999121144f ,1966.27625f } ,{377.647675f ,0.0999121144f ,1917.38281f } ,{481.886810f ,0.0999218747f ,1709.74060f } ,{476.017761f ,0.0999267623f ,1800.40137f } ,{430.367798f ,0.0999218747f ,1853.59509f } ,{471.041077f ,0.0999121144f ,1932.16016f } ,{289.749268f ,0.0999121144f ,1993.78369f } };

					naviMesh->SaveMesh(path, tempStorage, indexTempStorage);
				}




			}
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Bake"))
		{
			BakeNavMesh();
		}
		ImGui::EndMenuBar();
	}

}

bool NaviMeshCreatorWindow::CheckSnap(const Vector3& position, Vector3& outPosition, INT& index)
{
	float distance = 0.0f, shortestDistance = FLT_MAX;
	index = -1;
	UINT currentIndex = 0;
	for (auto& point : vertices)
	{
		distance = (point - position).magnitude();
		if (snapDistance < distance)
		{
			currentIndex++;
			continue;
		}

		if (shortestDistance > distance)
		{
			index = currentIndex;
			memcpy(&outPosition ,&point, sizeof(Vector3));
		}
		currentIndex++;
	}
	return index != -1;
}

void NaviMeshCreatorWindow::BakeNavMesh(void)
{
	verticesAfterBaked.clear();
	indicesAfterBaked.clear();
	verticesAfterBaked.resize(vertices.size());
	memcpy(verticesAfterBaked.data(), vertices.data(), sizeof(Vector3) * vertices.size());
	GenerateTriangles(vertices, lines, indicesAfterBaked);
	if (FAILED(naviMesh->LoadMesh(vertices, indicesAfterBaked)))
	{
		Console::WriteLine("Bake failed");
	}
	Console::WriteLine("Bake complete");
}

bool NaviMeshCreatorWindow::CheckExsist(UINT indexA, UINT indexB)
{
	for (auto& line : lines)
	{
		if ((indexA == line.first && indexB == line.second))
			return true;

		if ((indexA == line.second && indexB == line.first))
			return true;
	}


	return false;
}

void NaviMeshCreatorWindow::GenerateTriangles(const std::vector<Vector3>& vertices, const std::vector<std::pair<UINT, UINT>>& lineIndices, std::vector<UINT>& triangleIndices)
{
	if (!triangleIndices.empty())
		triangleIndices.clear();

	std::set<std::pair<UINT, UINT>> edgeSet;

	auto MakeEdge = [](UINT a, UINT b) { return std::make_pair(std::min(a, b), std::max(a, b)); };

	for (auto& edge : lineIndices)
	{
		edgeSet.insert(MakeEdge(edge.first, edge.second));
	}

	// 중복 방지를 위한 set of sorted triplets
	std::set<std::tuple<UINT, UINT, UINT>> triangleSet;

	UINT vertexCount = static_cast<UINT>(vertices.size());
	for (UINT x = 0; x < vertexCount; ++x)
	{
		for (UINT y = x + 1; y < vertexCount; ++y)
		{
			for (UINT z = y + 1; z < vertexCount; ++z)
			{
				if (edgeSet.count(MakeEdge(x, y)) && edgeSet.count(MakeEdge(y, z)) && edgeSet.count(MakeEdge(z, x)))
				{
					const Vector3& vertexA = vertices[x];
					const Vector3& vertexB = vertices[y];
					const Vector3& vertexC = vertices[z];

					auto sortedTri = std::make_tuple(x, y, z);
					if (triangleSet.insert(sortedTri).second)
					{
						// 원하는 Up 벡터와 비교
						if (DirectX::XMVectorGetY(GetTriangleNormal(vertexA, vertexB, vertexC)) >= 0)
						{
							// 반시계 방향이면 그대로 삽입
							triangleIndices.push_back(x);
							triangleIndices.push_back(y);
							triangleIndices.push_back(z);
						}
						else
						{
							// 시계 방향이면 순서 바꿔서 삽입
							triangleIndices.push_back(x);
							triangleIndices.push_back(z);
							triangleIndices.push_back(y);
						}
					}
				}

			}
		}
	}

}
