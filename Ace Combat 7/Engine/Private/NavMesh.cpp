#include "Foundation.h"
#include "NavMesh.h"

using namespace Engine;


HRESULT NavMesh::LoadMesh(const std::vector<Vector3>& vertices, const std::vector<UINT>& indices, std::vector<NaviNode>& nodes)
{
	nodes.clear();
	std::vector<NavMesh::Triangle> nodeData;

	TriangleAdjacency(indices, nodeData);
	nodes.resize(nodeData.size());
	UINT index = 0;
	for (auto& triangle : nodeData)
	{
		NaviNode& node = nodes[index];
		memcpy(&node.vertices[0], &vertices[triangle.triangleIndex[0]], sizeof(Vector3) * 3);
		memcpy(&node.vertices[1], &vertices[triangle.triangleIndex[1]], sizeof(Vector3) * 3);
		memcpy(&node.vertices[2], &vertices[triangle.triangleIndex[2]], sizeof(Vector3) * 3);

		memcpy(node.neighborCellIndices, triangle.adjacency, sizeof(INT) * 3);
		node.index = triangle.index;
		node.SetNode();

		index++;
	}
	return S_OK;

}

void NavMesh::TriangleAdjacency(const std::vector<UINT>& indices, std::vector<NavMesh::Triangle>& nodes)
{
	INT triangleCount = static_cast<INT>(indices.size()) / 3;
	nodes.clear();
	nodes.resize(triangleCount);

	std::map<std::pair<INT, INT>, std::vector<INT>> edgeMap;

	INT vertex0;
	INT vertex1;
	std::pair<int, int> edge;
	for (INT i = 0; i < triangleCount; ++i)
	{
		NavMesh::Triangle& triangle = nodes[i];
		triangle.index = i;
		triangle.triangleIndex[0] = indices[i * 3 + 0];
		triangle.triangleIndex[1] = indices[i * 3 + 1];
		triangle.triangleIndex[2] = indices[i * 3 + 2];

		for (INT j = 0; j < 3; ++j)
		{
			vertex0 = triangle.triangleIndex[j];
			vertex1 = triangle.triangleIndex[(j + 1) % 3];
			edge = std::minmax(vertex0, vertex1);
			edgeMap[edge].push_back(i);
		}
	}
	for (int i = 0; i < triangleCount; ++i)
	{
		NavMesh::Triangle& triangle = nodes[i];
		for (int j = 0; j < 3; ++j)
		{
			vertex0 = triangle.triangleIndex[j];
			vertex1 = triangle.triangleIndex[(j + 1) % 3];
			edge = std::minmax(vertex0, vertex1);

			const auto& trisSharingEdge = edgeMap[edge];
			for (int neighbor : trisSharingEdge)
			{
				if (neighbor != i)
				{
					triangle.adjacency[j] = neighbor;
					break;
				}
			}
		}
	}
}

NavMesh::NavMesh(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::GameObject(dxDevice, dxDeviceContext)
{
}

NavMesh::NavMesh(const NavMesh& other) : Engine::GameObject(other)
{
	nodes.resize(other.nodes.size());
	memcpy(nodes.data(), other.nodes.data(), sizeof(NaviNode) * other.nodes.size());
}

void NavMesh::Free(void)
{
	nodes.clear();
	Engine::GameObject::Free();
}

NavMesh* NavMesh::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, const std::vector<Vector3>* vertices, const std::vector<UINT>* indices)
{
	NavMesh* newInstnace = new NavMesh(dxDevice, dxDeviceContext);

	if (vertices == nullptr || indices == nullptr)
		return newInstnace;

	if (FAILED(newInstnace->LoadMesh(*vertices, *indices, newInstnace->nodes)))
	{
		Base::Destroy(newInstnace);
		return nullptr;
	}

	return newInstnace;
}

NavMesh* NavMesh::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, std::wstring savedFilePath)
{
	NavMesh* newInstnace = new NavMesh(dxDevice, dxDeviceContext);

	if (savedFilePath.size() == 0)
		return newInstnace;

	if (FAILED(newInstnace->LoadMesh(savedFilePath)))
	{
		Base::Destroy(newInstnace);
		return nullptr;
	}

	return newInstnace;
}

Engine::GameObject* NavMesh::Clone(void)
{
	return new NavMesh(*this);
}

bool NavMesh::Inside(const Vector3& position, INT& cellIndex)
{
	NaviNode& currentNode = nodes[cellIndex];
	bool inside = true;
	while (currentNode.Inside(position, cellIndex))
	{
		inside = false;
		if (cellIndex == -1)
			return inside;
		currentNode = nodes[cellIndex];
	}
	return inside;
}

HRESULT NavMesh::LoadMesh(std::wstring& path)
{
	std::ifstream fileStream(path, std::ios::binary);
	HRESULT result = E_FAIL;
	if (!fileStream.is_open())
	{
		ErrMsg((std::wstring(L"File cannot open : ") + path).c_str());
		return result;
	}

	try
	{
		std::vector<Vector3> vertices;
		std::vector<UINT> indices;
		result = ReadMeshFile(fileStream, vertices, indices);
		fileStream.close();
		if (FAILED(result))
		{
			ErrMsg((std::wstring(L"File currupted : ") + path).c_str());
			return E_FAIL;
		}
		result = LoadMesh(vertices, indices);
		if (FAILED(result))
		{
			ErrMsg((std::wstring(L"File currupted : ") + path).c_str());
			return E_FAIL;
		}

	}
	catch (...)
	{
		fileStream.close();
		ErrMsg((std::wstring(L"File Error : ") + path).c_str());
		return E_FAIL;
	}

	return result;
}

HRESULT NavMesh::SaveMesh(std::wstring& path, std::vector<Vector3> vertices, std::vector<UINT> indices)
{
	std::ofstream file;
	try
	{
		file.open(path, std::ios::binary);
		if (!file.is_open())
		{
			ErrMsg((std::wstring(L"File cannot open : ") + path).c_str());
			return E_FAIL;
		}

		if (FAILED(WriteMeshFile(file, vertices, indices)))
		{
			ErrMsg((std::wstring(L"Data Write Failed") + path).c_str());
			return E_FAIL;
		}


		file.close();
	}
	catch (...)
	{
		file.close();
		ErrMsg((std::wstring(L"Save Error : ") + path).c_str());
		return E_FAIL;
	}
	return S_OK;
}

UINT NavMesh::GetTriangleIndex(Vector3 position)
{
	int index = 0;
	for (auto& triangle : nodes)
	{
		if (triangle.Inside(position, index))
		{
			return triangle.index;
		}

	}

	return -1;
}

HRESULT NavMesh::ReadMeshFile(std::ifstream& fileStream, std::vector<Vector3>& verticesOut, std::vector<UINT>& indicesOut)
{
	size_t size;

	fileStream.read(reinterpret_cast<char*>(&size), sizeof(size_t));
	indicesOut.resize(size);
	fileStream.read(reinterpret_cast<char*>(indicesOut.data()), sizeof(UINT) * size);

	fileStream.read(reinterpret_cast<char*>(&size), sizeof(size_t));
	verticesOut.resize(size);
	fileStream.read(reinterpret_cast<char*>(verticesOut.data()), sizeof(Vector3) * size);

	return S_OK;
}

HRESULT NavMesh::WriteMeshFile(std::ofstream& fileStream, std::vector<Vector3>& verticesIn, std::vector<UINT>& indicesIn)
{
	size_t size;

	size = indicesIn.size();
	fileStream.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
	fileStream.write(reinterpret_cast<const char*>(indicesIn.data()), sizeof(UINT) * size);

	size = verticesIn.size();
	fileStream.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
	fileStream.write(reinterpret_cast<const char*>(verticesIn.data()), sizeof(Vector3) * size);

	return S_OK;
}
