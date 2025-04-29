#include "Foundation.h"
#include "Model.h"
#include "GameInstance.h"
#include "ModelLoader.h"
using namespace Engine;

Model::Model(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Component(dxDevice, dxDeviceContext), origin(true), rootNode(nullptr), modelAnimation(nullptr), globalOffset{}, currentAnimation(nullptr)
{
	DirectX::XMStoreFloat4x4(&globalOffset, DirectX::XMMatrixIdentity());
}

Model::Model(const Model& other) : Component(other), origin(false), rootNode(other.rootNode), modelAnimation(other.modelAnimation), globalOffset(other.globalOffset), currentAnimation(nullptr)
{
	for (auto& mesh : other.submesh)
		submesh.push_back(mesh);
	for (auto& material : other.materials)
		materials.push_back(material);

	for (auto& bone : other.bones)
		bones[bone.first] = bone.second->Clone();
	boneByIndex.resize(bones.size());
	for (auto& bone : bones)
		boneByIndex[bone.second->Index()] = bone.second;
}

void Model::Free(void)
{
	for (auto& bone : bones)
	{
		Base::Destroy(bone.second);
	}
	if (!origin)
		return;
	Base::Destroy(modelAnimation);
	modelAnimation = nullptr;
	for (auto& material : materials)
	{
		Base::Destroy(material);
	}

	Base::Destroy(rootNode);
	rootNode = nullptr;

	for (auto& mesh : submesh)
	{
		Base::Destroy(mesh);
	}
}
Model* Model::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, const std::wstring filePath, ModelLoader modelLoader, bool saveVertex)
{
	Model* newInstance = new Model(dxDevice, dxDeviceContext);
	if (FAILED(newInstance->LoadModel(filePath, modelLoader, saveVertex)))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}

	return newInstance;
}


Component* Model::Clone(void)
{
	Model* newInstance = new Model(*this);
	return newInstance;
}

void Model::LateUpdate(void)
{
	CalculateBoneMatrix();
}

void Model::Render(void)
{
	SetShader();
	SetBoneMatrix();
	for (auto& mesh : submesh)
	{
		materials[mesh->MaterialIndex()]->SetMaterial(dxDeviceContext);
		mesh->Render();
	}
}

void Model::CalculateBoneMatrix(void)
{
	ReadNodeHierarchy(rootNode);
	RefineBoneMatrix(rootNode);
}


void Model::SetBoneMatrix(void)
{
	std::vector<DirectX::XMFLOAT4X4> boneMatrix;
	DirectX::XMFLOAT4X4 tempMatrix;

	for (auto& bone : boneByIndex)
	{
		DirectX::XMStoreFloat4x4(&tempMatrix, DirectX::XMMatrixTranspose(bone->GetFinalMatrixDX()));
		//DirectX::XMStoreFloat4x4(&tempMatrix, DirectX::XMMatrixIdentity());
		boneMatrix.push_back(tempMatrix);
	}

	ID3D11Buffer* buffer = GetCurrentShader()->ConstantBuffer(GetCurrentShader()->boneMatrixBuffer);
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	if (SUCCEEDED(dxDeviceContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
	{
		BoneBuffer* data = static_cast<BoneBuffer*>(mappedResource.pData);
		memcpy(data->boneMatrix, boneMatrix.data(), sizeof(DirectX::XMFLOAT4X4) * boneMatrix.size());
		dxDeviceContext->Unmap(buffer, 0);

		BindConstantBuffer(GetCurrentShader()->boneMatrixBufferA, buffer);
	}
}

void Model::ReadNodeHierarchy(ModelNode* rootNode)
{
	std::list<ModelNode*> nodes;
	ModelNode* currentNode = rootNode;
	DirectX::XMMATRIX matrix, tranformation, identityMatrix = DirectX::XMMatrixIdentity();
	while (rootNode->ParentNode() != nullptr)
	{
		currentNode = currentNode->ParentNode();
	}
	nodes.push_back(currentNode);

	while (!nodes.empty())
	{
		matrix = identityMatrix;
		currentNode = nodes.front();
		nodes.pop_front();

		for (auto& childNode : currentNode->ChildNode())
		{
			nodes.push_back(&childNode);
		}

		if (currentNode->ParentNode() != nullptr)
		{
			if (currentNode->BoneIndex() >= 0)
				tranformation = boneByIndex[currentNode->BoneIndex()]->GetTransformDXMatrix();
			else
				tranformation = DirectX::XMLoadFloat4x4(&currentNode->DefaultTransformMatrix());

			matrix = tranformation * DirectX::XMLoadFloat4x4(&currentNode->ParentNode()->TransformMatrix());
		}
		else
		{
			if (currentNode->BoneIndex() >= 0)
				matrix = boneByIndex[currentNode->BoneIndex()]->GetTransformDXMatrix();
			else
				matrix = DirectX::XMLoadFloat4x4(&currentNode->DefaultTransformMatrix());
		}

		currentNode->TransformMatrix(matrix);
		currentNode->FinalTransformMatrix(identityMatrix);

		/*RefineBoneMatrix()*/
		matrix = DirectX::XMLoadFloat4x4(&currentNode->TransformMatrix());
		if (currentNode->BoneIndex() >= 0)
			matrix = boneByIndex[currentNode->BoneIndex()]->GetOffsetDXMatrix() * matrix;

		matrix *= DirectX::XMLoadFloat4x4(&globalOffset);

		currentNode->FinalTransformMatrix(matrix);
		if (currentNode->BoneIndex() >= 0)
			boneByIndex[currentNode->BoneIndex()]->SetFinalMatrix(matrix);

		// 여기서 본 행렬 제작의 모든 작업을 하지 않고 일부러 루프를 여러번 도는 이유 -> 수식 변경을 빠르게 하기 위해서 -> 본 행렬을 만드는 방법을 아직 정확하게 알지 못함 -> 해결함
	}
}

void Model::RefineBoneMatrix(ModelNode* rootNode)
{
	return;
	// 일부러 루프를 여러번 도는 이유 -> 수식 변경을 빠르게 하기 위해서 -> 본 행렬을 만드는 방법을 아직 정확하게 알지 못함 -> 해결은 했지만 혹시 몰라서 남겨놓기는 할거임 함수 호출 4클럭은 괜찮찮어?
	std::list<ModelNode*> nodes;
	ModelNode* currentNode = rootNode;
	DirectX::XMMATRIX matrix;
	while (rootNode->ParentNode() != nullptr)
	{
		currentNode = rootNode->ParentNode();
	}
	nodes.push_back(currentNode);
	while (!nodes.empty())
	{
		matrix = DirectX::XMMatrixIdentity();
		currentNode = nodes.front();
		nodes.pop_front();

		for (auto& childNode : currentNode->ChildNode())
		{
			nodes.push_back(&childNode);
		}


		matrix = DirectX::XMLoadFloat4x4(&currentNode->TransformMatrix());
		if (currentNode->BoneIndex() >= 0)
			matrix = boneByIndex[currentNode->BoneIndex()]->GetOffsetDXMatrix() * matrix;

		matrix *= DirectX::XMLoadFloat4x4(&globalOffset);

		currentNode->FinalTransformMatrix(matrix);
		if (currentNode->BoneIndex() >= 0)
			boneByIndex[currentNode->BoneIndex()]->SetFinalMatrix(matrix);
	}
}


HRESULT Model::LoadModel(const std::wstring& filePath, ModelLoader& modelLoader, bool saveVertex)
{
	HRESULT result = S_OK;
	vertexSaved = saveVertex;
	ModelConverter::FullModel loadedModel;
	result = modelLoader.LoadFile(filePath, loadedModel);
	if (FAILED(result))
		return result;

	Bone* newBone = nullptr;
	boneByIndex.resize(loadedModel.bones.size());
	if (boneByIndex.size() == 0)
	{
		newBone = Bone::Create();

		bones["StaticBone"] = newBone;
		boneByIndex.push_back(newBone);
	}
	for (auto item : loadedModel.bones)
	{
		newBone = Bone::Create(item);
		if (newBone == nullptr)
			continue;

		bones[item.name] = newBone;
		boneByIndex[item.index] = newBone;
	}
	for (auto item : loadedModel.subMeshes)
	{
		submesh.push_back(Mesh::Create(dxDevice, dxDeviceContext, item));
	}
	for (auto item : loadedModel.materials)
	{
		materials.push_back(Material::Create(item, filePath));
	}


	if (loadedModel.nodes.size() == 0)
		rootNode = ModelNode::Create();
	else
		rootNode = ModelNode::Create(loadedModel.nodes);

	if (loadedModel.animations.size() == 0)
		modelAnimation = ModelAnimation::Create(this);
	else
		modelAnimation = ModelAnimation::Create(loadedModel.animations, this);

	memcpy(&globalOffset, &loadedModel.modelGlobalOffset, sizeof(DirectX::XMFLOAT4X4));
	currentAnimation = modelAnimation->DefaultAnimation();
	SetAnimation(0);
	CalculateBoneMatrix();
	return S_OK;
}

Bone* Model::GetBone(const std::string& key)
{
	auto iterator = bones.find(key);
	if(iterator == bones.end())
		return nullptr;
	return iterator->second;
}

ModelNode* Model::GetNode(std::string name)
{
	std::list<ModelNode*> nodes;
	ModelNode* currentNode = rootNode;
	while (rootNode->ParentNode() != nullptr)
	{
		currentNode = currentNode->ParentNode();
	}
	nodes.push_back(currentNode);

	while (!nodes.empty())
	{
		currentNode = nodes.front();
		nodes.pop_front();

		if (currentNode->Name() == name)
			return currentNode;


		for (auto& childNode : currentNode->ChildNode())
		{
			nodes.push_back(&childNode);
		}

	}
	return nullptr;
}

void Model::PlayAnimation(FLOAT keyframe)
{
	if (currentAnimation == nullptr)
		currentAnimation = modelAnimation->DefaultAnimation();

	modelAnimation->SetAnimation(keyframe, *currentAnimation, bones);
	//modelAnimation->CreateAnimationMatrix(keyframe);
}

void Model::SetAnimation(UINT keyframe)
{
	if (currentAnimation == nullptr)
		currentAnimation = modelAnimation->DefaultAnimation();

	modelAnimation->SetAnimation(keyframe, *currentAnimation, bones);
	//modelAnimation->CreateAnimationMatrix((INT)keyframe);
}

void Model::SetCurrentAnimation(const std::string key)
{
	currentAnimation = modelAnimation->DefaultAnimation();
}

void Engine::Model::SetDefaultAnimation(const std::string key)
{
	modelAnimation->SetDefaultAnimation(key);
}

bool Model::RayToMeshCollision(const Vector3& rayPosition, const Vector3& rayDirection, Vector3& outHitPoint)
{
	if (!vertexSaved) return false;

	for (auto& mesh : submesh)
	{
		if (mesh->RayToMeshCollision(rayPosition, rayDirection, outHitPoint))
			return true;
	}
	return false;
}

#ifdef useAssimp
Model* Model::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, const std::string filePath)
{
	Model* newInstance = new Model(dxDevice, dxDeviceContext);
	if (FAILED(newInstance->LoadModel(filePath)))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}

	return newInstance;
}

HRESULT Model::LoadModel(const std::string& filePath)
{
	Assimp::Importer importer = {};
	vertexSaved = false;

	UINT flags = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;
	//importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
	//UINT flags = aiProcess_ConvertToLeftHanded;

	const aiScene* scene = importer.ReadFile(filePath, flags);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::string ErrMessage = "Assimp Error: %s\n" + std::string(importer.GetErrorString());
		MessageBoxA(NULL, ErrMessage.c_str(), "ERROR", MB_OK);
		return E_FAIL;
	}

	if (scene->HasAnimations())
	{
		UINT totalCount = 1;
		UINT boneIdx = 0;
		aiMesh* mesh = nullptr;
		aiBone* aiBone = nullptr;
		Bone* newBone = nullptr;
		std::string boneName = "";
		{
			bool nodeExsist = false;
			aiNode* root = scene->mRootNode;
			std::string rootBoneName = root->mName.C_Str();

			for (UINT i = 0; i < scene->mNumMeshes; i++)
			{
				mesh = scene->mMeshes[i];
				for (boneIdx = 0; boneIdx < mesh->mNumBones; boneIdx++)
				{
					aiBone = mesh->mBones[boneIdx];
					boneName = aiBone->mName.C_Str();
					if (rootBoneName != boneName)
						continue;

					newBone = Bone::Create(aiBone);
					nodeExsist = true;
					newBone->Index(0);
					bones[boneName] = newBone;
					break;
				}
				if (nodeExsist)
					break;
			}
			if (!nodeExsist)
			{
				newBone = Bone::Create(rootBoneName, 0, DirectX::XMMatrixIdentity());
				bones[rootBoneName] = newBone;
			}

		}
		mesh = nullptr;
		aiBone = nullptr;
		newBone = nullptr;

		for (UINT i = 0; i < scene->mNumMeshes; i++)
		{
			mesh = scene->mMeshes[i];
			// 본 데이터 가져오기
			for (boneIdx = 0; boneIdx < mesh->mNumBones; boneIdx++)
			{
				aiBone = mesh->mBones[boneIdx];
				std::string boneName = aiBone->mName.C_Str();
				if (bones.find(boneName) != bones.end())
					continue;

				newBone = Bone::Create(aiBone);
				newBone->Index(static_cast<UINT>(bones.size()));
				//newBone->Index(totalCount);
				//totalCount++;
				bones[boneName] = newBone;
			}
			submesh.push_back(Mesh::Create(dxDevice, dxDeviceContext, mesh, bones));
		}
		boneByIndex.resize(bones.size());
		for (auto bone : bones)
		{
			boneByIndex[bone.second->Index()] = bone.second;
		}
		modelAnimation = ModelAnimation::Create(scene, this);
	}
	else
	{
		for (UINT i = 0; i < scene->mNumMeshes; i++)
		{
			submesh.push_back(Mesh::Create(dxDevice, dxDeviceContext, scene->mMeshes[i], bones));
		}
		modelAnimation = nullptr;
	}

	for (UINT i = 0; i < scene->mNumMaterials; ++i)
	{
		//materials.push_back(Material::Create(dxDevice, dxDeviceContext, scene->mMaterials[i], To_WString(filePath)));
		materials.push_back(Material::Create(scene->mMaterials[i], To_WString(filePath)));
	}

	//LoadAnimation(scene, meshContainer.animations);
	rootNode = ModelNode::Create(scene->mRootNode);
	rootNode->pairingBones(bones);
	if(modelAnimation != nullptr)
		currentAnimation = modelAnimation->DefaultAnimation();

	if (scene)
		importer.FreeScene();

	return S_OK;
}
#endif