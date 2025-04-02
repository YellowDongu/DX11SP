#pragma once
#include "BaseObject.h"

namespace Engine
{
	class Model;
	class StaticMesh : public BaseObject
	{
	private:
		StaticMesh(void) = delete;
		StaticMesh(const StaticMesh& other) = delete;
		StaticMesh(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
		virtual ~StaticMesh(void);
		virtual void Free(void);
		static StaticMesh* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, aiMesh* mesh);
	public:
		void Render(void);

		void LinkModel(Model* _model) { model = _model; }
		int MaterialIndex(void) { return materialIndex; }
	private:
		HRESULT LoadMesh(aiMesh* mesh);
		HRESULT LoadMeshData(aiMesh* mesh, std::vector<StaticModelVertex>& vertices, std::vector<UINT>& indices);
		HRESULT CreateBuffer(ID3D11Device* dxDevice, ID3D11Buffer*& vertexBuffer, ID3D11Buffer*& indexBuffer, std::vector<StaticModelVertex>& vertices, std::vector<UINT>& indices);

		ID3D11Buffer* vertexBuffer;
		ID3D11Buffer* indexBuffer;
		Model* model;

		UINT materialIndex;
		UINT stride;
		UINT offset;
		UINT indexCount;
	};
}