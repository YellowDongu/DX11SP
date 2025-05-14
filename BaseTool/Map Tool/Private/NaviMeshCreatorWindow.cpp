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
			Console::WriteLine(std::to_string(result.x) + ":" + std::to_string(result.y) + ":" + std::to_string(result.z));
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

			Console::WriteLine(std::to_string(result.x) + ":" + std::to_string(result.y) + ":" + std::to_string(result.z));
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
