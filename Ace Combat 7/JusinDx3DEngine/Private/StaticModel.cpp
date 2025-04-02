#include "Foundation.h"
#include "StaticModel.h"
#include "GameInstance.h"

using namespace Engine;

StaticModel::StaticModel(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Component(dxDevice, dxDeviceContext), origin(true)
{
}

StaticModel::StaticModel(const StaticModel& other) : Component(other), origin(false)
{
	for (auto& material : other.materials)
		materials.push_back(material);
	for (auto& mesh : other.submesh)
		submesh.push_back(mesh);
}

void StaticModel::Free(void)
{
	if (!origin)
		return;
	for (auto& material : materials)
		Base::Destroy(material);
	materials.clear();

	for (auto& mesh : submesh)
		Base::Destroy(mesh);
	submesh.clear();
}

StaticModel* StaticModel::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, const std::string filePath)
{
	StaticModel* newInstance = new StaticModel(dxDevice, dxDeviceContext);
	if (FAILED(newInstance->LoadModel(filePath)))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
	return newInstance;
}

void StaticModel::Render(void)
{
	SetShader();
	for (auto& mesh : submesh)
	{
		if (mesh == nullptr)
			continue;
		if (materials[mesh->MaterialIndex()] != nullptr)
			materials[mesh->MaterialIndex()]->SetMaterial(dxDeviceContext);

		mesh->Render();
	}
}

HRESULT StaticModel::LoadModel(const std::string& filePath)
{
	Assimp::Importer importer;
	aiMesh* mesh = nullptr;

	UINT flags = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast | aiProcess_PreTransformVertices;
	importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, true);
	const aiScene* scene = importer.ReadFile(filePath, flags);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::string ErrMessage = "Assimp Error: %s\n" + std::string(importer.GetErrorString());
		MessageBoxA(NULL, ErrMessage.c_str(), "ERROR", MB_OK);
		return E_FAIL;
	}

	for (UINT i = 0; i < scene->mNumMeshes; i++)
	{
		submesh.push_back(Mesh::Create(dxDevice, dxDeviceContext, scene->mMeshes[i]));
	}

	for (UINT i = 0; i < scene->mNumMaterials; ++i)
	{
		materials.push_back(Material::Create(scene->mMaterials[i], To_WString(filePath)));
	}

	if (scene)
		importer.FreeScene();

	return S_OK;
}



