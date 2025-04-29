#pragma once
#include "Base.h"

namespace Engine
{
	class ModelNode;
	class Model;
	class Bone : public Base
	{
	private:
		Bone(void);
		Bone(const Bone& other);
		virtual ~Bone(void) = default;
		virtual void Free(void);
	public:
		static Bone* Create(aiBone* bone);
		static Bone* Create(void);
		static Bone* Create(const std::string& name, UINT index, DirectX::FXMMATRIX offsetMatrix);
		static Bone* Create(const ModelConverter::FullModelBone& bone);
		Bone* Clone(void);

		HRESULT GetBoneData(aiBone* bone);
		HRESULT GetBoneData(const ModelConverter::FullModelBone& bone);
		void BindNode(ModelNode* node);

		ModelNode* GetNode(void);
		Matrix ContvertMatrix(const aiMatrix4x4& in);
		void ContvertMatrix(Matrix& out, const aiMatrix4x4& in);


		void Index(UINT value) { index = value; }
		void SetBoneMatrix(fxmmat matrix) { DirectX::XMStoreFloat4x4(&boneMatrix, matrix); }
		void SetBoneMatrix(const Matrix& matrix) { memcpy(&boneMatrix, &matrix, sizeof(Matrix)); }
		void SetFinalMatrix(DirectX::FXMMATRIX matrix) { DirectX::XMStoreFloat4x4(&finalMatrix, matrix); }
		void SetFinalMatrix(const Matrix& matrix) { memcpy(&finalMatrix, &matrix, sizeof(Matrix)); finalMatrix = matrix; }

		UINT Index(void) { return index; }
		const std::string& Name(void) const  { return name; }
		const Matrix& GetTransformMatrix(void) const { return boneMatrix; }
		const Matrix& GetOffsetMatrix(void) const { return offsetMatrix; }
		const Matrix& GetFinalMatrix(void) const { return finalMatrix; }

		DirectX::XMMATRIX GetTransformDXMatrix(void) { return DirectX::XMLoadFloat4x4(&boneMatrix); }
		DirectX::XMMATRIX GetOffsetDXMatrix(void) { return DirectX::XMLoadFloat4x4(&offsetMatrix); }
		DirectX::XMMATRIX GetFinalMatrixDX(void) { return DirectX::XMLoadFloat4x4(&finalMatrix); }

	private:
		std::string name;
		UINT index;

		Matrix finalMatrix;
		Matrix boneMatrix;
		Matrix offsetMatrix;

		//Matrix transformationMatrix;

		ModelNode* node;
		//Model* model;
	};
}