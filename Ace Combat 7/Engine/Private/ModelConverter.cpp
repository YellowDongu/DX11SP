#include "Foundation.h"
#include "ModelConverter.h"

#ifdef useAssimp

HRESULT ModelConverter::ConvertFBX(const std::string filePath, const std::wstring savePath, const DirectX::XMFLOAT4X4& modelGlobalOffset, bool staticModel)
{
	HRESULT result = S_OK;
	Assimp::Importer importer;

	UINT flags = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;
	if(staticModel)
		flags |= aiProcess_PreTransformVertices;
	importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, true);
	const aiScene* scene = importer.ReadFile(filePath, flags);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::string ErrMessage = "Assimp Error: %s\n" + std::string(importer.GetErrorString());
		MessageBoxA(NULL, ErrMessage.c_str(), "ERROR", MB_OK);
		return E_FAIL;
	}
	ModelConverter::FullModel loadedModel;
	memcpy(&loadedModel.modelGlobalOffset, &modelGlobalOffset, sizeof(DirectX::XMFLOAT4X4));

	if (FAILED(LoadModel(scene, loadedModel)))
	{
		ErrMsg((std::wstring(L"Data Load Failed") + To_WString(filePath)).c_str());

		if (scene)
			importer.FreeScene();

		return E_FAIL;
	}

	std::ofstream file;
	file.open(savePath, std::ios::binary);
	if (!file.is_open())
		return E_FAIL;

	if (FAILED(SaveModel(loadedModel, file)))
	{
		ErrMsg((std::wstring(L"Data Save Failed") + savePath).c_str());
		result = E_FAIL;
	}

	file.close();

	if (scene)
		importer.FreeScene();

	return result;
}

HRESULT ModelConverter::LoadModel(const aiScene* scene, ModelConverter::FullModel& model)
{
	ModelConverter::FullModelMesh mesh;
	ModelConverter::FullModelMaterial material;
	ModelConverter::FullModelBone bone;
	ModelConverter::FullModelAnimation animation;
	ModelConverter::FullModelChannel channel;
	aiMesh* loadedMesh = nullptr;
	std::map<std::string, UINT> boneMap;
	std::map<std::string, UINT>::iterator boneIterator;
	for (UINT i = 0; i < scene->mNumMeshes; i++)
	{
		loadedMesh = scene->mMeshes[i];

		for (UINT j = 0; j < loadedMesh->mNumBones; j++)
		{
			LoadBone(loadedMesh->mBones[j], bone);
			boneIterator = boneMap.find(bone.name);
			if (boneIterator == boneMap.end())
			{
				bone.index = static_cast<UINT>(model.bones.size());
				boneMap[bone.name] = bone.index;
				model.bones.push_back(bone);
			}
		}

		mesh.vertex.clear();
		mesh.index.clear();
		LoadMesh(loadedMesh, mesh, model.bones, boneMap);
		model.subMeshes.push_back(mesh);
	}
	for (UINT i = 0; i < scene->mNumMaterials; i++)
	{
		LoadMaterial(scene->mMaterials[i], material);
		model.materials.push_back(material);
	}

	for (UINT i = 0; i < scene->mNumAnimations; i++)
	{
		LoadAnimation(scene->mAnimations[i], animation);
		model.animations.push_back(animation);
	}

	LoadNode(scene->mRootNode, model.nodes, model.bones);
	return S_OK;
}

HRESULT ModelConverter::LoadMesh(const aiMesh* aimesh, ModelConverter::FullModelMesh& mesh, std::vector<FullModelBone>& bones, std::map<std::string, UINT>& boneMap)
{
	std::vector<ModelConverter::FullModelVertex>& vertices = mesh.vertex;
	std::vector<UINT>& indices = mesh.index;


	int boneID = 0;
	aiBone* bone = nullptr;
	ModelConverter::FullModelVertex vertex;
	aiFace face;
	UINT i = 0, boneIdx = 0, weightIdx = 0;
	size_t j = 0;
	std::map<std::string, UINT>::iterator boneIterator;
	bool safety = false;
	float weightValue = 0.0f;

	mesh.materialIndex = aimesh->mMaterialIndex;

	std::vector<std::vector<std::pair<int, float>>> vertexBoneData(aimesh->mNumVertices);

	// 본 데이터 가져오기
	for (boneIdx = 0; boneIdx < aimesh->mNumBones; boneIdx++)
	{
		bone = aimesh->mBones[boneIdx];

		boneIterator = boneMap.find(bone->mName.C_Str());
		boneID = boneIterator->second;  // Set boneID

		// Set bone infomation
		for (weightIdx = 0; weightIdx < bone->mNumWeights; weightIdx++)
		{
			for (auto& boneInfo : vertexBoneData[bone->mWeights[weightIdx].mVertexId])
			{
				if (boneInfo.first == boneID)
				{
					safety = true;
					break;
				}
				weightValue += static_cast<float>(boneInfo.second);
			}

			if (weightValue == 1.0f)
				safety = true;

			weightValue = 0.0f;

			if (!safety)
				vertexBoneData[bone->mWeights[weightIdx].mVertexId].push_back({ boneID,  bone->mWeights[weightIdx].mWeight });
			else
				safety = false;
		}
	}

	for (i = 0; i < aimesh->mNumVertices; i++)
	{
		ZeroMemory(&vertex, sizeof(ModelConverter::FullModelVertex));
		memcpy(&vertex.position, &aimesh->mVertices[i], static_cast<size_t>(sizeof(FLOAT)) * 3);

		if (aimesh->HasNormals())
		{
			memcpy(&vertex.normal, &aimesh->mNormals[i], static_cast<size_t>(sizeof(FLOAT)) * 3);
		}

		if (aimesh->HasTangentsAndBitangents())
		{
			memcpy(&vertex.tangent, &aimesh->mTangents[i], static_cast<size_t>(sizeof(FLOAT)) * 3);
			memcpy(&vertex.bitTangent, &aimesh->mBitangents[i], static_cast<size_t>(sizeof(FLOAT)) * 3);
		}
		for (UINT j = 0; j < 8; j++)
		{
			if (aimesh->HasTextureCoords(j))
			{
				memcpy(&vertex.texcoord[j], &aimesh->mTextureCoords[j][i], static_cast<size_t>(sizeof(FLOAT)) * 2);
			}
		}

		vertices.push_back(vertex);
	}

	// Set bone infomation at vertices
	for (i = 0; i < aimesh->mNumVertices; i++)
	{
		for (j = 0; j < vertexBoneData[i].size() && j < 4; j++)
		{
			vertices[i].boneIDs[j] = vertexBoneData[i][j].first;
			vertices[i].weights[j] = vertexBoneData[i][j].second;
		}

		if (j != 4)
		{
			for (; j < 4; j++)
			{
				vertices[i].boneIDs[j] = 0;
				vertices[i].weights[j] = 0.0f;
			}
		}
	}

	// load indices
	for (i = 0; i < aimesh->mNumFaces; i++)
	{
		face = aimesh->mFaces[i];
		for (j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}
	return S_OK;
}

HRESULT ModelConverter::LoadMaterial(const aiMaterial* aimaterial, ModelConverter::FullModelMaterial& material)
{
	ZeroMemory(&material.diffuseColor, sizeof(DirectX::XMFLOAT4));
	ZeroMemory(&material.ambientColor, sizeof(DirectX::XMFLOAT4));
	ZeroMemory(&material.specularColor, sizeof(DirectX::XMFLOAT4));
	ZeroMemory(&material.emissiveColor, sizeof(DirectX::XMFLOAT4));
	ZeroMemory(&material.transparentColor, sizeof(DirectX::XMFLOAT4));
	ZeroMemory(&material.reflectiveColor, sizeof(DirectX::XMFLOAT4));
	ZeroMemory(&material.shininess, sizeof(FLOAT));

	for (UINT i = 0; i < AI_TEXTURE_TYPE_MAX; i++)
	{
		material.texturePath[i].clear();
	}

	aiString texturePath;
	std::string texPath;
	aiColor4D loadedColor;
	ai_real aiShininess;

	// 다중 디퓨즈 텍스처 로드
	//for (UINT i = 0; i < aiMaterial->GetTextureCount(aiTextureType_DIFFUSE); i++)
	//{
	//
	//    if (aiMaterial->GetTexture(aiTextureType_DIFFUSE, i, &texturePath) == AI_SUCCESS)
	//    {
	//        texPath = std::wstring(texturePath.C_Str(), texturePath.C_Str() + strlen(texturePath.C_Str()));
	//        std::wstring fullPath = getAbsolutePathFromFile(modelPath, texPath);
	//        if (FAILED(GetTexture(fullPath, fullPath, diffuseTexture)))
	//        {
	//            std::wstring errMsg = L"TextureLoadError : " + fullPath;
	//            ErrMsg(errMsg.c_str());
	//            result = E_FAIL;
	//        }
	//    }
	//
	//}

	// texture path load
	for (UINT i = 0; i < AI_TEXTURE_TYPE_MAX; i++)
	{
		if (aimaterial->GetTexture(static_cast<aiTextureType>(i), 0, &texturePath) == AI_SUCCESS)
		{
			material.texturePath[i] = texturePath.C_Str();
		}
	}

	// additional setting load
	if (aimaterial->Get(AI_MATKEY_COLOR_DIFFUSE, loadedColor) == AI_SUCCESS)
		material.diffuseColor = DirectX::XMFLOAT4(loadedColor.r, loadedColor.g, loadedColor.b, loadedColor.a);
	if (aimaterial->Get(AI_MATKEY_COLOR_AMBIENT, loadedColor) == AI_SUCCESS)
		material.ambientColor = DirectX::XMFLOAT4(loadedColor.r, loadedColor.g, loadedColor.b, loadedColor.a);
	if (aimaterial->Get(AI_MATKEY_COLOR_SPECULAR, loadedColor) == AI_SUCCESS)
		material.specularColor = DirectX::XMFLOAT4(loadedColor.r, loadedColor.g, loadedColor.b, loadedColor.a);
	if (aimaterial->Get(AI_MATKEY_COLOR_EMISSIVE, loadedColor) == AI_SUCCESS)
		material.emissiveColor = DirectX::XMFLOAT4(loadedColor.r, loadedColor.g, loadedColor.b, loadedColor.a);
	if (aimaterial->Get(AI_MATKEY_COLOR_REFLECTIVE, loadedColor) == AI_SUCCESS)
		material.reflectiveColor = DirectX::XMFLOAT4(loadedColor.r, loadedColor.g, loadedColor.b, loadedColor.a);
	if (aimaterial->Get(AI_MATKEY_SHININESS, aiShininess) == AI_SUCCESS)
		material.shininess = static_cast<FLOAT>(aiShininess);

	return S_OK;
}

HRESULT ModelConverter::LoadBone(const aiBone* aibone, ModelConverter::FullModelBone& bone)
{
	bone.name = aibone->mName.C_Str();
	ConvertMatrix(bone.offsetMatrix, aibone->mOffsetMatrix);
	bone.index = -1;
	return S_OK;
}

HRESULT ModelConverter::LoadNode(aiNode* rootNode, std::vector<ModelConverter::FullModelNode>& nodes, std::vector<ModelConverter::FullModelBone>& bones)
{
	aiNode* currentNode = nullptr;
	std::list<std::pair<aiNode*, UINT>> nodeQueue;
	nodeQueue.push_back({ rootNode, -1 });
	ModelConverter::FullModelNode node;

	while (!nodeQueue.empty())
	{
		node.name.clear();
		ZeroMemory(&node.index, sizeof(UINT));
		ZeroMemory(&node.parentIndex, sizeof(INT));
		node.boneIndex = -1;
		ZeroMemory(&node.defaultTransformationMatrix, sizeof(DirectX::XMFLOAT4X4));
		node.childIndex.clear();


		currentNode = nodeQueue.front().first;
		node.parentIndex = nodeQueue.front().second;
		nodeQueue.pop_front();


		node.name = currentNode->mName.C_Str();
		ConvertMatrix(node.defaultTransformationMatrix, currentNode->mTransformation);

		for (auto& bone : bones)
		{
			if (bone.name == node.name)
			{
				node.boneIndex = bone.index;
				break;
			}
		}

		node.index = static_cast<UINT>(nodes.size());
		nodes.push_back(node);

		for (UINT i = 0; i < currentNode->mNumChildren; i++)
		{
			nodeQueue.push_back({ currentNode->mChildren[i], node.index });
		}
	}

	for (auto& node : nodes)
	{
		for (auto& childNode : nodes)
		{
			if (node.index == childNode.parentIndex)
				node.childIndex.push_back(childNode.index);
		}
	}



	return S_OK;
}

HRESULT ModelConverter::LoadAnimation(const aiAnimation* aianimation, ModelConverter::FullModelAnimation& animation)
{
	ModelConverter::FullModelChannel channel;
	for (UINT i = 0; i < aianimation->mNumChannels; i++)
	{
		channel.boneName.clear();
		channel.positions.clear();
		channel.quaternions.clear();
		channel.scales.clear();

		animation.name = aianimation->mName.C_Str();
		animation.ticksPerSecond = static_cast<FLOAT>(aianimation->mTicksPerSecond);
		animation.duration = static_cast<FLOAT>(aianimation->mDuration);

		LoadChannel(aianimation->mChannels[i], channel);
		animation.channels.push_back(channel);
	}

	return S_OK;
}

HRESULT ModelConverter::LoadChannel(const aiNodeAnim* aiChannel, ModelConverter::FullModelChannel& channel)
{
	channel.boneName = aiChannel->mNodeName.C_Str();

	for (UINT key = 0; key < aiChannel->mNumPositionKeys; ++key)
	{
		channel.positions.push_back({ (float)aiChannel->mPositionKeys[key].mTime, { (float)aiChannel->mPositionKeys[key].mValue.x, (float)aiChannel->mPositionKeys[key].mValue.y, (float)aiChannel->mPositionKeys[key].mValue.z } });
	}

	for (UINT key = 0; key < aiChannel->mNumRotationKeys; ++key)
	{
		channel.quaternions.push_back({ (float)aiChannel->mRotationKeys[key].mTime , { (float)aiChannel->mRotationKeys[key].mValue.x, (float)aiChannel->mRotationKeys[key].mValue.y, (float)aiChannel->mRotationKeys[key].mValue.z, (float)aiChannel->mRotationKeys[key].mValue.w } });
	}

	for (UINT key = 0; key < aiChannel->mNumScalingKeys; ++key)
	{
		channel.scales.push_back({ (float)aiChannel->mScalingKeys[key].mTime , { (float)aiChannel->mScalingKeys[key].mValue.x, (float)aiChannel->mScalingKeys[key].mValue.y, (float)aiChannel->mScalingKeys[key].mValue.z } });
	}

	return S_OK;
}

void ModelConverter::ConvertMatrix(Matrix& out, const aiMatrix4x4& in)
{
	memcpy(&out, &in, sizeof(Matrix));
	DirectX::XMStoreFloat4x4(&out, DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&out)));
}

Matrix ModelConverter::ConvertMatrix(const aiMatrix4x4& in)
{
	Matrix out;
	memcpy(&out, &in, sizeof(Matrix));
	DirectX::XMStoreFloat4x4(&out, DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&out)));
	return out;
}

void ModelConverter::StringToBinary(const std::string& text, std::ofstream& fileStream)
{
	size_t size = text.length();
	fileStream.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
	fileStream.write(text.c_str(), sizeof(char) * text.length());
}

HRESULT ModelConverter::SaveModel(ModelConverter::FullModel& model, std::ofstream& fileStream)
{
	size_t size = model.subMeshes.size();
	fileStream.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
	for (auto& item : model.subMeshes)
		SaveMesh(item, fileStream);

	size = model.materials.size();
	fileStream.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
	for (auto& item : model.materials)
		SaveMaterial(item, fileStream);

	size = model.bones.size();
	fileStream.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
	for (auto& item : model.bones)
		SaveBone(item, fileStream);

	size = model.nodes.size();
	fileStream.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
	for (auto& item : model.nodes)
		SaveNode(item, fileStream);

	size = model.animations.size();
	fileStream.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
	for (auto& item : model.animations)
		SaveAnimation(item, fileStream);

	fileStream.write(reinterpret_cast<const char*>(&model.modelGlobalOffset), sizeof(DirectX::XMFLOAT4X4));
	return S_OK;
}

HRESULT ModelConverter::SaveMesh(ModelConverter::FullModelMesh& mesh, std::ofstream& fileStream)
{
	size_t size = mesh.vertex.size();
	fileStream.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
	fileStream.write(reinterpret_cast<const char*>(mesh.vertex.data()), sizeof(ModelConverter::FullModelVertex) * mesh.vertex.size());

	size = mesh.index.size();
	fileStream.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
	fileStream.write(reinterpret_cast<const char*>(mesh.index.data()), sizeof(UINT) * mesh.index.size());

	fileStream.write(reinterpret_cast<const char*>(&mesh.materialIndex), sizeof(UINT));
	return S_OK;
}

HRESULT ModelConverter::SaveMaterial(ModelConverter::FullModelMaterial& material, std::ofstream& fileStream)
{
	fileStream.write(reinterpret_cast<const char*>(&material.diffuseColor), sizeof(DirectX::XMFLOAT4));
	fileStream.write(reinterpret_cast<const char*>(&material.ambientColor), sizeof(DirectX::XMFLOAT4));
	fileStream.write(reinterpret_cast<const char*>(&material.specularColor), sizeof(DirectX::XMFLOAT4));
	fileStream.write(reinterpret_cast<const char*>(&material.emissiveColor), sizeof(DirectX::XMFLOAT4));
	fileStream.write(reinterpret_cast<const char*>(&material.transparentColor), sizeof(DirectX::XMFLOAT4));
	fileStream.write(reinterpret_cast<const char*>(&material.reflectiveColor), sizeof(DirectX::XMFLOAT4));

	fileStream.write(reinterpret_cast<const char*>(&material.shininess), sizeof(FLOAT));

	for (UINT i = 0; i < AI_TEXTURE_TYPE_MAX; i++)
		StringToBinary(material.texturePath[i], fileStream);

	return S_OK;
}

HRESULT ModelConverter::SaveBone(ModelConverter::FullModelBone& bone, std::ofstream& fileStream)
{
	StringToBinary(bone.name, fileStream);

	fileStream.write(reinterpret_cast<const char*>(&bone.offsetMatrix), sizeof(DirectX::XMFLOAT4X4));
	fileStream.write(reinterpret_cast<const char*>(&bone.index), sizeof(UINT));
	return S_OK;
}

HRESULT ModelConverter::SaveNode(ModelConverter::FullModelNode& node, std::ofstream& fileStream)
{
	StringToBinary(node.name, fileStream);

	fileStream.write(reinterpret_cast<const char*>(&node.defaultTransformationMatrix), sizeof(DirectX::XMFLOAT4X4));
	fileStream.write(reinterpret_cast<const char*>(&node.index), sizeof(UINT));
	fileStream.write(reinterpret_cast<const char*>(&node.boneIndex), sizeof(UINT));
	fileStream.write(reinterpret_cast<const char*>(&node.parentIndex), sizeof(UINT));

	size_t size = node.childIndex.size();
	fileStream.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
	fileStream.write(reinterpret_cast<const char*>(node.childIndex.data()), sizeof(UINT) * node.childIndex.size());
	return S_OK;
}

HRESULT ModelConverter::SaveAnimation(ModelConverter::FullModelAnimation& material, std::ofstream& fileStream)
{
	StringToBinary(material.name, fileStream);

	fileStream.write(reinterpret_cast<const char*>(&material.ticksPerSecond), sizeof(FLOAT));
	fileStream.write(reinterpret_cast<const char*>(&material.duration), sizeof(FLOAT));

	size_t size = material.channels.size();
	fileStream.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
	for (auto& channel : material.channels)
	{
		SaveChannel(channel, fileStream);
	}

	return S_OK;
}

HRESULT ModelConverter::SaveChannel(ModelConverter::FullModelChannel& channel, std::ofstream& fileStream)
{
	StringToBinary(channel.boneName, fileStream);

	size_t size = channel.positions.size();
	fileStream.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
	fileStream.write(reinterpret_cast<const char*>(channel.positions.data()), (sizeof(ModelConverter::KeyFramePair)) * channel.positions.size());

	size = channel.quaternions.size();
	fileStream.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
	fileStream.write(reinterpret_cast<const char*>(channel.quaternions.data()), (sizeof(ModelConverter::QuaternionKeyFramePair)) * channel.quaternions.size());

	size = channel.scales.size();
	fileStream.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
	fileStream.write(reinterpret_cast<const char*>(channel.scales.data()), (sizeof(ModelConverter::KeyFramePair)) * channel.scales.size());
	return S_OK;
}

#endif