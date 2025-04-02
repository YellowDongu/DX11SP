#include "Foundation.h"
#include "ModelLoader.h"

using namespace Engine;


HRESULT ModelLoader::LoadFile(const std::wstring filePath, ModelConverter::FullModel& model)
{
	std::ifstream fileStream(filePath, std::ios::binary);
	HRESULT result = E_FAIL;
	if (!fileStream.is_open())
	{
		ErrMsg((std::wstring(L"File cannot open : ") + filePath).c_str());
		return result;
	}

	try
	{
		result = LoadModel(model, fileStream);
	}
	catch (...)
	{
		fileStream.close();
		ErrMsg((std::wstring(L"File currupted : ") + filePath).c_str());
		return result;
	}

	return result;
}


void ModelLoader::BinaryToString(std::string& text, std::ifstream& fileStream)
{
	size_t length;
	fileStream.read(reinterpret_cast<char*>(&length), sizeof(size_t));
	text.resize(length);
	fileStream.read(&text[0], length);
}

HRESULT ModelLoader::LoadModel(ModelConverter::FullModel& model, std::ifstream& fileStream)
{
	size_t size;
	fileStream.read(reinterpret_cast<char*>(&size), sizeof(size_t));
	model.subMeshes.resize(size);
	for (auto& item : model.subMeshes)
		LoadMesh(item, fileStream);

	fileStream.read(reinterpret_cast<char*>(&size), sizeof(size_t));
	model.materials.resize(size);
	for (auto& item : model.materials)
		LoadMaterial(item, fileStream);

	fileStream.read(reinterpret_cast<char*>(&size), sizeof(size_t));
	model.bones.resize(size);
	for (auto& item : model.bones)
		LoadBone(item, fileStream);

	fileStream.read(reinterpret_cast<char*>(&size), sizeof(size_t));
	model.nodes.resize(size);
	for (auto& item : model.nodes)
		LoadNode(item, fileStream);

	fileStream.read(reinterpret_cast<char*>(&size), sizeof(size_t));
	model.animations.resize(size);
	for (auto& item : model.animations)
		LoadAnimation(item, fileStream);

	fileStream.read(reinterpret_cast<char*>(&model.modelGlobalOffset), sizeof(DirectX::XMFLOAT4X4));
	return S_OK;
}

HRESULT ModelLoader::LoadMesh(ModelConverter::FullModelMesh& mesh, std::ifstream& fileStream)
{
	size_t length;
	fileStream.read(reinterpret_cast<char*>(&length), sizeof(size_t));
	mesh.vertex.resize(length);
	fileStream.read(reinterpret_cast<char*>(mesh.vertex.data()), sizeof(ModelConverter::FullModelVertex) * length);

	fileStream.read(reinterpret_cast<char*>(&length), sizeof(size_t));
	mesh.index.resize(length);
	fileStream.read(reinterpret_cast<char*>(mesh.index.data()), sizeof(UINT) * length);

	fileStream.read(reinterpret_cast<char*>(&mesh.materialIndex), sizeof(UINT));
	return S_OK;
}

HRESULT ModelLoader::LoadMaterial(ModelConverter::FullModelMaterial& material, std::ifstream& fileStream)
{
	fileStream.read(reinterpret_cast<char*>(&material.diffuseColor), sizeof(DirectX::XMFLOAT4));
	fileStream.read(reinterpret_cast<char*>(&material.ambientColor), sizeof(DirectX::XMFLOAT4));
	fileStream.read(reinterpret_cast<char*>(&material.specularColor), sizeof(DirectX::XMFLOAT4));
	fileStream.read(reinterpret_cast<char*>(&material.emissiveColor), sizeof(DirectX::XMFLOAT4));
	fileStream.read(reinterpret_cast<char*>(&material.transparentColor), sizeof(DirectX::XMFLOAT4));
	fileStream.read(reinterpret_cast<char*>(&material.reflectiveColor), sizeof(DirectX::XMFLOAT4));

	fileStream.read(reinterpret_cast<char*>(&material.shininess), sizeof(FLOAT));

	material.texturePath->resize(AI_TEXTURE_TYPE_MAX);
	for (UINT i = 0; i < AI_TEXTURE_TYPE_MAX; i++)
		BinaryToString(material.texturePath[i], fileStream);

	return S_OK;
}

HRESULT ModelLoader::LoadBone(ModelConverter::FullModelBone& bone, std::ifstream& fileStream)
{
	BinaryToString(bone.name, fileStream);

	fileStream.read(reinterpret_cast<char*>(&bone.offsetMatrix), sizeof(DirectX::XMFLOAT4X4));
	fileStream.read(reinterpret_cast<char*>(&bone.index), sizeof(UINT));
	return S_OK;
}

HRESULT ModelLoader::LoadNode(ModelConverter::FullModelNode& node, std::ifstream& fileStream)
{
	BinaryToString(node.name, fileStream);

	fileStream.read(reinterpret_cast<char*>(&node.defaultTransformationMatrix), sizeof(DirectX::XMFLOAT4X4));
	fileStream.read(reinterpret_cast<char*>(&node.index), sizeof(UINT));
	fileStream.read(reinterpret_cast<char*>(&node.boneIndex), sizeof(UINT));
	fileStream.read(reinterpret_cast<char*>(&node.parentIndex), sizeof(UINT));
	size_t size;
	fileStream.read(reinterpret_cast<char*>(&size), sizeof(size_t));
	node.childIndex.resize(size);
	fileStream.read(reinterpret_cast<char*>(node.childIndex.data()), sizeof(UINT) * size);
	return S_OK;
}

HRESULT ModelLoader::LoadAnimation(ModelConverter::FullModelAnimation& animation, std::ifstream& fileStream)
{
	BinaryToString(animation.name, fileStream);

	fileStream.read(reinterpret_cast<char*>(&animation.ticksPerSecond), sizeof(FLOAT));
	fileStream.read(reinterpret_cast<char*>(&animation.duration), sizeof(FLOAT));

	size_t size;
	fileStream.read(reinterpret_cast<char*>(&size), sizeof(size_t));
	animation.channels.resize(size);
	for (auto& channel : animation.channels)
	{
		LoadChannel(channel, fileStream);
	}

	return S_OK;
}

HRESULT ModelLoader::LoadChannel(ModelConverter::FullModelChannel& channel, std::ifstream& fileStream)
{
	BinaryToString(channel.boneName, fileStream);

	size_t size;
	fileStream.read(reinterpret_cast<char*>(&size), sizeof(size_t));
	channel.positions.resize(size);
	fileStream.read(reinterpret_cast<char*>(channel.positions.data()), (sizeof(ModelConverter::KeyFramePair)) * size);

	fileStream.read(reinterpret_cast<char*>(&size), sizeof(size_t));
	channel.quaternions.resize(size);
	fileStream.read(reinterpret_cast<char*>(channel.quaternions.data()), (sizeof(ModelConverter::QuaternionKeyFramePair)) * size);

	fileStream.read(reinterpret_cast<char*>(&size), sizeof(size_t));
	channel.scales.resize(size);
	fileStream.read(reinterpret_cast<char*>(channel.scales.data()), (sizeof(ModelConverter::KeyFramePair)) * size);

	return S_OK;
}
