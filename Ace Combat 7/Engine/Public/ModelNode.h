#pragma once
#include "Base.h"

namespace Engine
{
	class Bone;
	class ModelNode : public Base
	{
	public:
		ModelNode(void);
		virtual ~ModelNode(void);
		virtual void Free(void) override {};
	public:
		static ModelNode* Create(void);
		static ModelNode* Create(const std::vector<ModelConverter::FullModelNode>& nodeList);

		HRESULT LoadNode(const ModelConverter::FullModelNode& node);
		void LinkBone(Bone* bone);
		void pairingBones(std::map<std::string, Bone*>& bones);
		HRESULT CreateNode(const std::vector<ModelConverter::FullModelNode>& nodeList);

		Matrix& FinalTransformMatrix(void) { return finalTransform; }
		const Matrix& DefaultTransformMatrix(void) const { return defaultTransform; }
		Matrix& TransformMatrix(void) { return transform; }
		void FinalTransformMatrix(fxmmat matrix) { DirectX::XMStoreFloat4x4(&finalTransform, matrix); }
		void TransformMatrix(fxmmat matrix) { DirectX::XMStoreFloat4x4(&transform, matrix); }
		void DefaultTransformMatrix(fxmmat matrix) { return DirectX::XMStoreFloat4x4(&defaultTransform, matrix); }

		const std::string& Name(void) const { return name; }
		ModelNode* ParentNode(void) { return parentNode; }
		//Bone* LinkBone(void) { return bone; }
		INT BoneIndex(void) { return boneIndex; }
		std::vector<ModelNode>& ChildNode(void) { return childNode; }
		ModelNode* ChildNodes(void) { return &childNode.front(); }
		UINT ChildNumber(void) { return static_cast<UINT>(childNode.size()); }

		#ifdef useAssimp
		static ModelNode* Create(aiNode* rootNode);
		HRESULT LoadNode(aiNode* node);
		HRESULT CreateNode(aiNode* rootNode);
		#endif // useAssimp


	private:
		ModelNode* parentNode;
		Bone* bone;
		INT boneIndex;
		Matrix defaultTransform;
		Matrix transform;
		Matrix finalTransform;
		std::string name;
		std::vector<ModelNode> childNode;

		//std::map<std::string, ModelNode*> nodes;
	};
}