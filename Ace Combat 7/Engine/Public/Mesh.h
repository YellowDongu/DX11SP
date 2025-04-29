#pragma once
#include "BaseObject.h"


namespace Engine
{
	class Model;
	class Bone;
	class Mesh : public BaseObject
	{
	private:
		Mesh(void) = delete;
		Mesh(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
		Mesh(const Mesh& other) = delete;
		virtual ~Mesh(void) = default;
		virtual void Free(void) override;
    public:
		static Mesh* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, ModelConverter::FullModelMesh& subMesh, bool saveVertex = false);

		void Render(void);

        void LinkModel(Model* _model) { model = _model; }
		int MaterialIndex(void) { return materialIndex; }

		bool RayToMeshCollision(const Vector3& rayPosition, const Vector3& rayDirection, Vector3& outHitPoint);
	private:
		HRESULT LoadMesh(ModelConverter::FullModelMesh& subMesh, bool saveVertex = false);
		HRESULT ConvertVertexData(std::vector<ModelVertex>& vertices, std::vector<ModelConverter::FullModelVertex>& otherVertices);
		HRESULT CreateBuffer(ID3D11Device* dxDevice, ID3D11Buffer*& vertexBuffer, ID3D11Buffer*& indexBuffer, std::vector<ModelVertex>& vertices, std::vector<UINT>& indices);

		#ifdef useAssimp
	public:
		static Mesh* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, aiMesh* mesh);
		static Mesh* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, aiMesh* mesh, std::map<std::string, Bone*>& bones);
	private:
		HRESULT LoadMesh(aiMesh* mesh, std::map<std::string, Bone*>& bones);
		HRESULT LoadMeshData(aiMesh* mesh, std::vector<ModelVertex>& vertices, std::vector<UINT>& indices, std::map<std::string, Bone*>& bones);
		#endif

		ID3D11Buffer* vertexBuffer;
		ID3D11Buffer* indexBuffer;
		Model* model;

		UINT materialIndex;
		UINT stride;
		UINT offset;
		UINT indexCount;
		std::vector<ModelVertex> savedVertices;
		std::vector<UINT> savedIndices;
	};

}