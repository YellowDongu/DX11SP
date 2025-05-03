#pragma once
#include "BasicWindow.h"
#include "NavMesh.h"

namespace Engine
{
	class PointDrawer;
	class LineDrawer;
}
class NaviMeshCreatorWindow : public BasicWindow
{
private:
	NaviMeshCreatorWindow(void) = default;
	virtual ~NaviMeshCreatorWindow(void) = default;
	//CTimeManager(const TimeManager&) = delete;
	//CTimeManager& operator=(const TimeManager&) = delete;
	virtual void Free(void);
public:
	static NaviMeshCreatorWindow* Create(class ObjectManager* manager);

	void Start(void);
	void Update(void);
	void Render(void);

	virtual HRESULT CreateFrame(void) override;
	void CreateBasicMenu(void);
	bool CheckSnap(const Vector3& position, Vector3& out, INT& index);
	void BakeNavMesh(void);
    bool CheckExsist(UINT indexA, UINT indexB);

    inline xmVector GetTriangleNormal(const Vector3& pointA, const Vector3& pointB, const Vector3& pointC) { return DirectX::XMVector3Normalize(DirectX::XMVector3Cross(pointB - pointA, pointC - pointA)); }
    void GenerateTriangles(const std::vector<Vector3>& vertices, const std::vector<std::pair<UINT, UINT>>& lineIndices, std::vector<UINT>& output);

private:
	bool render = true, naviRender = false, VertexSetMode = false;
	float snapDistance = 3.0f;
	Engine::NavMesh* naviMesh{ nullptr };

	INT selectedPointIndex = -1;
	Vector3 selectedPoint = { -1.0f, -1.0f, -1.0f };
	std::vector<Vector3> vertices;
	std::vector<std::pair<UINT, UINT>> lines;

	std::vector<Vector3> verticesAfterBaked;
	std::vector<UINT> indicesAfterBaked;

	const D3D11_VIEWPORT* viewPort{ nullptr };
	class ToolCamera* camera{ nullptr };
	class ObjectManager* manager{ nullptr };
	Engine::PointDrawer* pointDrawer{nullptr};
	Engine::LineDrawer* lineDrawer{nullptr};
	ID3D11Device* device{ nullptr };
	ID3D11DeviceContext* context{ nullptr };
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthDisabledState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> wireframeState;
};
