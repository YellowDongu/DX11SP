#pragma once
#ifdef useAssimp


class ENGINEDLL ModelConverter
{
public:
	struct FullModelVertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 texcoord[8];
		DirectX::XMFLOAT3 tangent;
		DirectX::XMFLOAT3 bitTangent;
		UINT boneIDs[4];
		FLOAT weights[4];
	};
	struct FullModelMesh
	{
		std::vector<ModelConverter::FullModelVertex> vertex;
		std::vector<UINT> index;
		UINT materialIndex;
	};
	struct FullModelMaterial
	{
		DirectX::XMFLOAT4 diffuseColor;
		DirectX::XMFLOAT4 ambientColor;
		DirectX::XMFLOAT4 specularColor;
		DirectX::XMFLOAT4 emissiveColor;
		DirectX::XMFLOAT4 transparentColor;
		DirectX::XMFLOAT4 reflectiveColor;
		FLOAT shininess;

		std::string texturePath[AI_TEXTURE_TYPE_MAX];
	};
	struct FullModelBone
	{
		std::string name;
		DirectX::XMFLOAT4X4 offsetMatrix;
		UINT index;
	};
	struct FullModelNode
	{
		std::string name;
		DirectX::XMFLOAT4X4 defaultTransformationMatrix;
		UINT index;
		UINT boneIndex;
		UINT parentIndex;
		std::vector<UINT> childIndex;
	};

	struct KeyFramePair
	{
		FLOAT keyFrame;
		DirectX::XMFLOAT3 data;
	};
	struct QuaternionKeyFramePair
	{
		FLOAT keyFrame;
		DirectX::XMFLOAT4 data;
	};
	struct FullModelChannel
	{
		std::string boneName;
		std::vector<KeyFramePair> positions;
		std::vector<QuaternionKeyFramePair> quaternions;
		std::vector<KeyFramePair> scales;
	};
	struct FullModelAnimation
	{
		std::string name;
		FLOAT ticksPerSecond;
		FLOAT duration;
		std::vector<ModelConverter::FullModelChannel> channels;
	};
	struct FullModel
	{
		std::string name;
		std::vector<ModelConverter::FullModelMesh> subMeshes;
		std::vector<ModelConverter::FullModelMaterial> materials;
		std::vector<ModelConverter::FullModelBone> bones;
		std::vector<ModelConverter::FullModelNode> nodes;
		std::vector<ModelConverter::FullModelAnimation> animations;
		DirectX::XMFLOAT4X4 modelGlobalOffset;
	};

public:
	ModelConverter(void) = default;
	~ModelConverter(void) = default;

	HRESULT ConvertFBX(const std::string filePath, const std::wstring savePath, const DirectX::XMFLOAT4X4& modelGlobalOffset, bool staticModel = false);

	HRESULT LoadModel(const aiScene* scene, ModelConverter::FullModel& model);
	HRESULT LoadMesh(const aiMesh* aiMesh, ModelConverter::FullModelMesh& mesh, std::vector<FullModelBone>& bones, std::map<std::string, UINT>& boneMap);
	HRESULT LoadMaterial(const aiMaterial* aimaterial, ModelConverter::FullModelMaterial& material);
	HRESULT LoadBone(const aiBone* aibone, ModelConverter::FullModelBone& bone);
	HRESULT LoadNode(aiNode* rootNode, std::vector<ModelConverter::FullModelNode>& nodes, std::vector<ModelConverter::FullModelBone>& bones);
	HRESULT LoadAnimation(const aiAnimation* aianimation, ModelConverter::FullModelAnimation& material);
	HRESULT LoadChannel(const aiNodeAnim* aiChannel, ModelConverter::FullModelChannel& channel);

	void ConvertMatrix(Matrix& out, const aiMatrix4x4& in);
	Matrix ConvertMatrix(const aiMatrix4x4& in);
	void StringToBinary(const std::string& text, std::ofstream& fileStream);

	HRESULT SaveModel(ModelConverter::FullModel& model, std::ofstream& fileStream);
	HRESULT SaveMesh(ModelConverter::FullModelMesh& mesh, std::ofstream& fileStream);
	HRESULT SaveMaterial(ModelConverter::FullModelMaterial& material, std::ofstream& fileStream);
	HRESULT SaveBone(ModelConverter::FullModelBone& bone, std::ofstream& fileStream);
	HRESULT SaveNode(ModelConverter::FullModelNode& node, std::ofstream& fileStream);
	HRESULT SaveAnimation(ModelConverter::FullModelAnimation& material, std::ofstream& fileStream);
	HRESULT SaveChannel(ModelConverter::FullModelChannel& channel, std::ofstream& fileStream);

private:

};

#endif