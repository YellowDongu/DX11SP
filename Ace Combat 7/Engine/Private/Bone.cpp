#include "Foundation.h"
#include "Bone.h"
#include "ModelNode.h"
//
//using namespace Engine;
//
//Bone::Bone(void) : name{}, boneMatrix{}, offsetMatrix{}, node(nullptr), index(0)
//{
//	DirectX::XMStoreFloat4x4(&boneMatrix, DirectX::XMMatrixIdentity());
//	DirectX::XMStoreFloat4x4(&offsetMatrix, DirectX::XMMatrixIdentity());
//	DirectX::XMStoreFloat4x4(&finalMatrix, DirectX::XMMatrixIdentity());
//}
//
//Bone::Bone(const Bone& other) : name(other.name), boneMatrix(other.boneMatrix), offsetMatrix(other.offsetMatrix), finalMatrix(other.finalMatrix), node(other.node), index(other.index)
//{
//}
//
//void Bone::Free(void)
//{
//}
//Bone* Bone::Create(aiBone* bone)
//{
//	Bone* newInstance = new Bone();
//	if (FAILED(newInstance->GetBoneData(bone)))
//	{
//		Base::Destroy(newInstance);
//		return nullptr;
//	}
//	return newInstance;
//}
//Bone* Bone::Create(void)
//{
//	Bone* newInstance = new Bone();
//	newInstance->name = "Root";
//	return newInstance;
//}
//// for Custom Bone
//Bone* Bone::Create(const std::string& name, UINT index, DirectX::FXMMATRIX offset)
//{
//	Bone* newInstance = new Bone();
//	newInstance->name = name;
//	newInstance->index = index;
//	DirectX::XMStoreFloat4x4(&newInstance->offsetMatrix, offset);
//	return newInstance;
//}
//
//Bone* Bone::Create(const ModelConverter::FullModelBone& bone)
//{
//	Bone* newInstance = new Bone();
//	if (FAILED(newInstance->GetBoneData(bone)))
//	{
//		Base::Destroy(newInstance);
//		return nullptr;
//	}
//	return newInstance;
//}
//
//Bone* Bone::Clone(void)
//{
//	return new Bone(*this);
//}
//
//HRESULT Bone::GetBoneData(aiBone* bone)
//{
//	ContvertMatrix(offsetMatrix, bone->mOffsetMatrix);
//	DirectX::XMStoreFloat4x4(&offsetMatrix, DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&offsetMatrix)));
//	name = bone->mName.C_Str();
//	DirectX::XMStoreFloat4x4(&boneMatrix, DirectX::XMMatrixIdentity());
//	DirectX::XMStoreFloat4x4(&finalMatrix, DirectX::XMMatrixIdentity());
//	return S_OK;
//}
//HRESULT Bone::GetBoneData(const ModelConverter::FullModelBone& bone)
//{
//	name = bone.name;
//	memcpy(&offsetMatrix, &bone.offsetMatrix, sizeof(DirectX::XMFLOAT4X4));
//	memcpy(&index, &bone.index, sizeof(UINT));
//	return S_OK;
//}
//
//ModelNode* Bone::GetNode(void)
//{
//	//if (node != nullptr)
//	//{
//	//	return nullptr;
//	//}
//	return node;
//}
//void Bone::ContvertMatrix(Matrix& out, const aiMatrix4x4& in)
//{
//	memcpy(&out, &in, sizeof(Matrix)); return;
//	out._11 = in.a1; out._12 = in.a2; out._13 = in.a3; out._14 = in.a4;
//	out._21 = in.b1; out._22 = in.b2; out._23 = in.b3; out._24 = in.b4;
//	out._31 = in.c1; out._32 = in.c2; out._33 = in.c3; out._34 = in.c4;
//	out._41 = in.d1; out._42 = in.d2; out._43 = in.d3; out._44 = in.d4;
//}
//Matrix Bone::ContvertMatrix(const aiMatrix4x4& in)
//{
//	Matrix out;
//	memcpy(&out, &in, sizeof(Matrix)); return out;
//	out._11 = in.a1; out._12 = in.a2; out._13 = in.a3; out._14 = in.a4;
//	out._21 = in.b1; out._22 = in.b2; out._23 = in.b3; out._24 = in.b4;
//	out._31 = in.c1; out._32 = in.c2; out._33 = in.c3; out._34 = in.c4;
//	out._41 = in.d1; out._42 = in.d2; out._43 = in.d3; out._44 = in.d4;
//}
//
//void Bone::BindNode(ModelNode* _node)
//{
//	//if (name !=_node.Name())
//	//{
//	//	return;
//	//}
//	node = _node;
//}

using namespace Engine;

Bone::Bone(void) : name{}, boneMatrix{}, offsetMatrix{}, node(nullptr), index(0)
{
	DirectX::XMStoreFloat4x4(&boneMatrix, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&offsetMatrix, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&finalMatrix, DirectX::XMMatrixIdentity());
}

Bone::Bone(const Bone& other) : name(other.name), boneMatrix(other.boneMatrix), offsetMatrix(other.offsetMatrix), finalMatrix(other.finalMatrix), node(other.node), index(other.index)
{
}

void Bone::Free(void)
{
}
Bone* Bone::Create(aiBone* bone)
{
	Bone* newInstance = new Bone();
	if (FAILED(newInstance->GetBoneData(bone)))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
	return newInstance;
}
Bone* Bone::Create(void)
{
	Bone* newInstance = new Bone();
	newInstance->name = "Root";
	return newInstance;
}
Bone* Bone::Create(const ModelConverter::FullModelBone& bone)
{
	Bone* newInstance = new Bone();
	if (FAILED(newInstance->GetBoneData(bone)))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
	return newInstance;
}

// for Custom Bone
Bone* Bone::Create(const std::string& name, UINT index, DirectX::FXMMATRIX offset)
{
	Bone* newInstance = new Bone();
	newInstance->name = name;
	newInstance->index = index;
	DirectX::XMStoreFloat4x4(&newInstance->offsetMatrix, offset);
	return newInstance;
}

Bone* Bone::Clone(void)
{
	return new Bone(*this);
}

HRESULT Bone::GetBoneData(aiBone* bone)
{
	ContvertMatrix(offsetMatrix, bone->mOffsetMatrix);
	DirectX::XMStoreFloat4x4(&offsetMatrix, DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&offsetMatrix)));
	name = bone->mName.C_Str();
	DirectX::XMStoreFloat4x4(&boneMatrix, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&finalMatrix, DirectX::XMMatrixIdentity());
	return S_OK;
}
HRESULT Bone::GetBoneData(const ModelConverter::FullModelBone& bone)
{
	name = bone.name;
	memcpy(&offsetMatrix, &bone.offsetMatrix, sizeof(DirectX::XMFLOAT4X4));
	memcpy(&index, &bone.index, sizeof(UINT));
	return S_OK;
}

ModelNode* Bone::GetNode(void)
{
	//if (node != nullptr)
	//{
	//	return nullptr;
	//}
	return node;
}
void Bone::ContvertMatrix(Matrix& out, const aiMatrix4x4& in)
{
	memcpy(&out, &in, sizeof(Matrix)); return;
	out._11 = in.a1; out._12 = in.a2; out._13 = in.a3; out._14 = in.a4;
	out._21 = in.b1; out._22 = in.b2; out._23 = in.b3; out._24 = in.b4;
	out._31 = in.c1; out._32 = in.c2; out._33 = in.c3; out._34 = in.c4;
	out._41 = in.d1; out._42 = in.d2; out._43 = in.d3; out._44 = in.d4;
}
Matrix Bone::ContvertMatrix(const aiMatrix4x4& in)
{
	Matrix out;
	memcpy(&out, &in, sizeof(Matrix)); return out;
	out._11 = in.a1; out._12 = in.a2; out._13 = in.a3; out._14 = in.a4;
	out._21 = in.b1; out._22 = in.b2; out._23 = in.b3; out._24 = in.b4;
	out._31 = in.c1; out._32 = in.c2; out._33 = in.c3; out._34 = in.c4;
	out._41 = in.d1; out._42 = in.d2; out._43 = in.d3; out._44 = in.d4;
}

void Bone::BindNode(ModelNode* _node)
{
	//if (name !=_node.Name())
	//{
	//	return;
	//}
	node = _node;
}
