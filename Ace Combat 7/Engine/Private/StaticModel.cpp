#include "Foundation.h"
#include "StaticModel.h"
#include "GameInstance.h"
#include "ModelLoader.h"
#include "GameObject.h"
#include "Transform.h"
using namespace Engine;

StaticModel::StaticModel(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Component(dxDevice, dxDeviceContext), origin(true)
{
	DirectX::XMStoreFloat4x4(&offsetMatrix, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&baseMatrix, DirectX::XMMatrixIdentity());
}

StaticModel::StaticModel(const StaticModel& other) : Component(other), origin(false), offsetMatrix(other.offsetMatrix), baseMatrix(other.baseMatrix), vertexSaved(other.vertexSaved)
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

StaticModel* StaticModel::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, const std::wstring filePath, ModelLoader modelLoader, bool saveVertex)
{
	StaticModel* newInstance = new StaticModel(dxDevice, dxDeviceContext);
	if (FAILED(newInstance->LoadModel(filePath, modelLoader, saveVertex)))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
	return newInstance;
}

void Engine::StaticModel::SetBoneMatrix(void)
{
	DirectX::XMFLOAT4X4 tempMatrix;

	DirectX::XMStoreFloat4x4(&tempMatrix, DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&offsetMatrix), DirectX::XMLoadFloat4x4(&baseMatrix)));

	ID3D11Buffer* buffer = GetCurrentShader()->ConstantBuffer(GetCurrentShader()->boneMatrixBuffer);
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	if (SUCCEEDED(dxDeviceContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
	{
		BoneBuffer* data = static_cast<BoneBuffer*>(mappedResource.pData);
		memcpy(data->boneMatrix, &tempMatrix, sizeof(DirectX::XMFLOAT4X4));
		dxDeviceContext->Unmap(buffer, 0);

		BindConstantBuffer(GetCurrentShader()->boneMatrixBufferA, buffer);
	}
}

void StaticModel::Render(void)
{
	SetShader();
	SetBoneMatrix();
	for (auto& mesh : submesh)
	{
		if (mesh == nullptr)
			continue;
		if (materials[mesh->MaterialIndex()] != nullptr)
			materials[mesh->MaterialIndex()]->SetMaterial(dxDeviceContext);
		mesh->Render();
	}
}


HRESULT StaticModel::LoadModel(const std::wstring& filePath, ModelLoader& modelLoader, bool saveVertex)
{
	vertexSaved = saveVertex;
	DirectX::XMStoreFloat4x4(&offsetMatrix, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&baseMatrix, DirectX::XMMatrixIdentity());
	HRESULT result = S_OK;

	ModelConverter::FullModel loadedModel;
	result = modelLoader.LoadFile(filePath, loadedModel);
	if (FAILED(result))
		return result;

	for (auto item : loadedModel.subMeshes)
	{
		submesh.push_back(Mesh::Create(dxDevice, dxDeviceContext, item, saveVertex, true));
	}
	for (auto item : loadedModel.materials)
	{
		materials.push_back(Material::Create(item, filePath));
	}


	memcpy(&offsetMatrix, &loadedModel.modelGlobalOffset, sizeof(DirectX::XMFLOAT4X4));
	return S_OK;
}

bool StaticModel::RayToMeshCollision(const Vector3& rayPosition, const Vector3& rayDirection, Vector3& outHitPoint)
{
	if (!vertexSaved) return false;
	Vector3 localOrigin, localDirection;
	xmMatrix inverseWorld, world;
	xmVector origin = DirectX::XMLoadFloat3(&rayPosition), direction = DirectX::XMLoadFloat3(&rayDirection), rayLocalOrigin, rayLocalDirection, rayEnd, rayLocalEnd;
	if (gameObject == nullptr || gameObject->transform() == nullptr)
		world = DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&baseMatrix), DirectX::XMLoadFloat4x4(&offsetMatrix));
	else
		world = DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&gameObject->transform()->WorldMatrix()), DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&baseMatrix), DirectX::XMLoadFloat4x4(&offsetMatrix)));
	inverseWorld = DirectX::XMMatrixInverse(nullptr, world);

	rayLocalOrigin = DirectX::XMVector3TransformCoord(origin, inverseWorld);
	rayEnd = DirectX::XMVectorAdd(origin, direction);
	rayLocalEnd = DirectX::XMVector3TransformCoord(rayEnd, inverseWorld);
	rayLocalDirection = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(rayLocalEnd, rayLocalOrigin));
	DirectX::XMStoreFloat3(&localOrigin, rayLocalOrigin);
	DirectX::XMStoreFloat3(&localDirection, rayLocalDirection);


	for (auto& mesh : submesh)
	{
		if (mesh->RayToMeshCollision(localOrigin, localDirection, outHitPoint))
		{
			DirectX::XMStoreFloat3(&outHitPoint, DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat3(&outHitPoint), world));
			return true;
		}
	}
	return false;
}


#ifdef useAssimp
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


HRESULT StaticModel::LoadModel(const std::string& filePath)
{
	DirectX::XMStoreFloat4x4(&offsetMatrix, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&baseMatrix, DirectX::XMMatrixIdentity());

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
#endif