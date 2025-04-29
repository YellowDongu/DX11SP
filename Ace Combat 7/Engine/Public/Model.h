#pragma once
#include "Component.h"
#include "Bone.h"
#include "Mesh.h"
#include "ModelNode.h"
#include "Material.h"
#include "ModelAnimation.h"

namespace Engine
{
	class ModelLoader;
	class ENGINEDLL Model : public Component
	{
	private:
		Model(void) = delete;
		Model(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
		Model(const Model& other);
		virtual ~Model(void) = default;
		virtual void Free(void) override;
	public:
		static Model* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, const std::wstring filePath, ModelLoader modelLoader, bool saveVertex = false);
		virtual Component* Clone(void) override;

		//void Start(void);
		//void Update(void);
		virtual void LateUpdate(void) override;
		//virtual void Render(void) override;
		virtual void Render(void);

		void CalculateBoneMatrix(void);
		void SetBoneMatrix(void);
		void ReadNodeHierarchy(ModelNode* rootNode);
		void RefineBoneMatrix(ModelNode* rootNode);
		HRESULT LoadModel(const std::wstring& filePath, ModelLoader& modelLoader, bool saveVertex = false);
		UINT GetBoneNumber(void) { return static_cast<UINT>(boneByIndex.size()); }
		Bone* GetBone(const std::string& key);
		Bone* GetBone(UINT index) { if (index >= boneByIndex.size())return nullptr; return boneByIndex[index]; }
		Animation* GetAnimation(const std::string key) { return modelAnimation->GetAnimation(key); }
		Animation* DefaultAnimation(void) { return modelAnimation->DefaultAnimation(); }
		Animation* CurrentAnimation(void) { return currentAnimation; }
		ModelNode* RootNode(void) { return rootNode; }
		ModelNode* GetNode(std::string name);

		void PlayAnimation(FLOAT keyframe);
		void SetAnimation(UINT keyframe);

		void SetCurrentAnimation(const std::string key);
		void SetDefaultAnimation(const std::string key);

		void SetGlobalOffset(const Matrix& matrix) { globalOffset = matrix; }
		void SetGlobalOffset(fxmMatrix matrix) { DirectX::XMStoreFloat4x4(&globalOffset, matrix); }
		bool RayToMeshCollision(const Vector3& rayPosition, const Vector3& rayDirection, Vector3& outHitPoint);

		#ifdef useAssimp
		static Model* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, const std::string filePath);
		HRESULT LoadModel(const std::string& filePath);
		#endif


	private:
		bool origin, vertexSaved;
		std::map<std::string, Bone*> bones;
		std::vector<Bone*> boneByIndex;
		ModelNode* rootNode;
		std::vector<Mesh*> submesh;
		std::vector<Material*> materials;
		ModelAnimation* modelAnimation;
		Matrix globalOffset;
		Animation* currentAnimation;
	};
}