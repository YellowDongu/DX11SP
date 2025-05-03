#pragma once
#include "GameObject.h"


namespace Engine
{
	struct ENGINEDLL NaviNode
	{
		enum class LINE { LINE_AB, LINE_BC, LINE_CA, LINE_END }; // for guide, a=0 b=1 c=2
		Vector3 vertices[3];
		Vector3 normal[3];

		Vector3 lineVector[3];
		float4 plane;

		UINT index = -1;
		INT neighborCellIndices[3] = { -1, -1, -1 };

		inline void SetNode(void)
		{
			Vector3 directionVector = vertices[1] - vertices[0];
			DirectX::XMStoreFloat3(&lineVector[static_cast<UINT>(NaviNode::LINE::LINE_AB)], DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&directionVector)));
			directionVector = vertices[2] - vertices[1];
			DirectX::XMStoreFloat3(&lineVector[static_cast<UINT>(NaviNode::LINE::LINE_BC)], DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&directionVector)));
			directionVector = vertices[0] - vertices[2];
			DirectX::XMStoreFloat3(&lineVector[static_cast<UINT>(NaviNode::LINE::LINE_CA)], DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&directionVector)));

			for (size_t i = 0; i < 3; i++)
				normal[i] = float3(-lineVector[i].z, 0.f, lineVector[i].x);

			DirectX::XMStoreFloat4(&plane, DirectX::XMPlaneFromPoints(DirectX::XMVectorSetW(XMLoadFloat3(&vertices[0]), 1.f), DirectX::XMVectorSetW(XMLoadFloat3(&vertices[1]), 1.f), DirectX::XMVectorSetW(XMLoadFloat3(&vertices[2]), 1.f)));
		}

		inline bool Inside(fxmVector position, INT& cellIndex)
		{
			using namespace DirectX;
			xmVector direction;
			for (size_t i = 0; i < 3; i++)
			{
				direction = position - XMLoadFloat3(&vertices[i]);

				if (0 < XMVectorGetX(XMVector3Dot(XMVector3Normalize(direction), XMLoadFloat3(&normal[i]))))
				{
					cellIndex = neighborCellIndices[i];
					return false;
				}
			}
			return true;
		}
		inline bool Inside(Vector3 position, INT& cellIndex)
		{
			Vector3 direction;
			for (size_t i = 0; i < 3; i++)
			{
				direction = position - vertices[i];

				if (0 < DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&direction)), DirectX::XMLoadFloat3(&normal[i]))))
				{
					cellIndex = neighborCellIndices[i];
					return false;
				}
			}
			return true;
		}
		inline float ImpactAngle(const Vector3& direction)
		{
			float dot = DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&direction)), DirectX::XMVectorSetW(DirectX::XMLoadFloat4(&plane), 0.f)));
			dot = clamp(dot, -1.0f, 1.0f);

			return DirectX::XMConvertToDegrees(std::acos(std::abs(dot)));
		}
		inline xmVector ReflectionDirection(const Vector3& direction) { return DirectX::XMVector3Reflect(DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&direction)), DirectX::XMVectorSetW(DirectX::XMLoadFloat4(&plane), 0.f)); }

		inline FLOAT getHeight(fxmVector position) { return (-plane.x * DirectX::XMVectorGetX(position) - plane.z * DirectX::XMVectorGetZ(position) - plane.w) / plane.y; }
		inline FLOAT getHeight(Vector3 position) { return (-plane.x * position.x - plane.z * position.z - plane.w) / plane.y; }
	};

	class ENGINEDLL NavMesh : public Engine::GameObject
	{
	private:
		struct Triangle
		{
			INT triangleIndex[3] = { -1, -1, -1 };
			UINT index = -1;
			INT adjacency[3] = { -1, -1, -1 };
		};
	public:
		NavMesh(void) = delete;
		NavMesh(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
		NavMesh(const NavMesh& other);
		virtual ~NavMesh(void) = default;
		virtual void Free(void) override;
		static NavMesh* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, const std::vector<Vector3>* vertices = nullptr, const std::vector<UINT>* indices = nullptr);
		static NavMesh* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, std::wstring savedFilePath);
		virtual Engine::GameObject* Clone(void) override;

		bool Inside(const Vector3& position, INT& cellIndex);
		HRESULT LoadMesh(std::wstring& path);
		HRESULT LoadMesh(std::wstring& path, std::vector<Vector3>& vertices, std::vector<UINT>& indices);
		HRESULT SaveMesh(std::wstring& path, std::vector<Vector3> vertices, std::vector<UINT> indices);
		INT GetTriangleIndex(Vector3 position);
		NaviNode* GetNode(INT index) { if (index == -1) return nullptr; return &nodes[index]; }

		HRESULT ReadMeshFile(std::ifstream& fileStream, std::vector<Vector3>& verticesOut, std::vector<UINT>& indicesOut);
		HRESULT WriteMeshFile(std::ofstream& fileStream, std::vector<Vector3>& verticesIn, std::vector<UINT>& indicesIn);
		HRESULT LoadMesh(const std::vector<Vector3>& vertices, const std::vector<UINT>& indices) { return LoadMesh(vertices, indices, nodes); }
		HRESULT LoadMesh(const std::vector<Vector3>& vertices, const std::vector<UINT>& indices, std::vector<NaviNode>& node);
		void TriangleAdjacency(const std::vector<UINT>& indices, std::vector<Triangle>& nodes);
		std::vector<NaviNode>& GetAllNodes(void) { return nodes; }
	private:
		std::vector<NaviNode> nodes;
	};

}