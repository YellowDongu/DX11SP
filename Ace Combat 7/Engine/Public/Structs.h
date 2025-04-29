#pragma once


#ifndef useAssimp
class ModelConverter
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
	struct FullModelChannel
	{
		std::string boneName;
		std::vector<std::pair<FLOAT, DirectX::XMFLOAT3>> positions;
		std::vector<std::pair<FLOAT, DirectX::XMFLOAT4>> quaternions;
		std::vector<std::pair<FLOAT, DirectX::XMFLOAT3>> scales;
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

	ModelConverter(void) = default;
	~ModelConverter(void) = default;
};

#endif


namespace Engine
{
	typedef struct DeviceInfomation
	{
		HWND hWnd;
		HINSTANCE hInstance;
		bool isWindowed;
		UINT viewpoirtWidth;
		UINT viewpoirtLength;
		bool shaderOutput;
	} DeviceInfo;

}

struct LightData
{
    enum class LightType {DIRECTIONAL, POINT, SpotLight, emmisive, END};
    
    LightType lightType;
    float4 direction;
    float4 position;
    FLOAT range;
    float4 diffuse;
    float4 ambient;
    float4 specular;
};

struct StaticModelVertex
{
    float3 position;
    float3 normal;
    float2 texcoord;
};

struct ENGINEDLL ModelVertex
{
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 normal;
    DirectX::XMFLOAT2 texcoord;
    //DIrectX::XMFLOAT3 tangent;
    UINT boneIDs[4];
    FLOAT weights[4];

    static const std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayout;

};

// 본 기반 애니메이션용 정점 구조체
struct ENGINEDLL AdvencedVertex
{
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 normal;
    DirectX::XMFLOAT2 texcoord;

    UINT boneIDs[4];
    float weights[4];

    static const std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayout;
};

struct BoneBuffer
{
    DirectX::XMFLOAT4X4 boneMatrix[256];
};

struct MaterialBufferType
{
    DirectX::XMFLOAT4 diffuseColor;
    DirectX::XMFLOAT4 specularColor;
    float shininess;
    int hasSpecularTexture;  // 1이면 specularTexture 있음, 0이면 없음
    float padding[2]; // 16바이트 정렬
};

struct ENGINEDLL UIVertex
{
    DirectX::XMFLOAT2 position;
    DirectX::XMFLOAT2 texturePosition;

    static const std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayout;
};

struct ENGINEDLL CubeVertex
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT3 texturePosition;

	static const std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayout;
};