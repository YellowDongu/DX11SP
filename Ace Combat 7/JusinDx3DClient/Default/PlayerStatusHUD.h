#pragma once
#include "UIObject.h"

class PlayerStatusHUD : public Engine::UIObject
{
private:
    PlayerStatusHUD(void) = delete;
    PlayerStatusHUD(const PlayerStatusHUD&) = delete;
    PlayerStatusHUD(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
    virtual ~PlayerStatusHUD(void) = default;
    virtual void Free(void);
public:
    static PlayerStatusHUD* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, Engine::GameObject* player);
    virtual Engine::GameObject* Clone(void) override { return nullptr; }

    virtual HRESULT Start(void) override;
    virtual HRESULT Awake(void) override;
    virtual void FixedUpdate(void) override;
    virtual void Update(void) override;
    virtual void LateUpdate(void) override;
    virtual void Render(void) override;

private:
    Engine::Text* text;

};


/*


// 텍스쳐 정보도 담고있는 메테리얼
struct Material
{
    DirectX::XMFLOAT4 diffuseColor;    // 디퓨즈 색상
    DirectX::XMFLOAT4 specularColor;   // 스펙큘러 색상
    float shininess;                   // 반사도(Shininess)

    ID3D11ShaderResourceView* diffuseTexture;  // 디퓨즈 텍스처
    ID3D11ShaderResourceView* specularTexture; // 스펙큘러 텍스처
    ID3D11ShaderResourceView* normalTexture; // 스펙큘러 텍스처
    int hasSpecularTexture;  // 1이면 specularTexture 있음, 0이면 없음 (16바이트 정렬)

    void Free(void)
    {
        if (diffuseTexture)
            diffuseTexture->Release();
        if (specularTexture)
            specularTexture->Release();
    }

    Material(void) : diffuseColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)), specularColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)), shininess(32.0f), diffuseTexture(nullptr), specularTexture(nullptr), hasSpecularTexture(false), normalTexture(nullptr)
    {
    }
};

//// 본 애니메이션의 매트릭스를 담고있음
//struct Bone
//{
//    std::string name;
//    int parentIndex;
//    int index;
//    DirectX::XMFLOAT4X4 offsetMatrix;
//    Bone(void) : name(""), parentIndex(0), index(0), offsetMatrix{} {}
//    void Clear(void) { name.clear(); parentIndex = 0; index = 0; }
//};

struct Node
{
    std::string BoneName;
    DirectX::XMFLOAT4X4 offsetMatrix;
    DirectX::XMFLOAT4X4 transformMatrix;
    DirectX::XMFLOAT4X4 currentMatrix;
    DirectX::XMFLOAT4X4 finalMatrix;
    std::vector<Node> mChildren;
    Node* parent = nullptr;
};

struct AnimationNode
{
    std::string boneName;

    std::vector<std::pair<float, DirectX::XMFLOAT3>> positions;
    std::vector<std::pair<float, DirectX::XMFLOAT4>> quaternionRotations;
    std::vector<std::pair<float, DirectX::XMFLOAT3>> scales;

    void Clear(void)
    {
        boneName.clear();
        positions.clear();
        quaternionRotations.clear();
        scales.clear();
    }
};


struct AnimationInfo
{
    std::string name;
    float duration;
    float ticksPerSecond;

    std::vector<AnimationNode> nodes;

    void Clear(void)
    {
        name.clear();
        duration = 0.0f;
        ticksPerSecond = 0.0f;
        nodes.clear();
    }

};



// For test
//struct SimpleMeshData
//{
//    ID3D11Buffer* vertexBuffer = nullptr;
//    ID3D11Buffer* indexBuffer = nullptr;
//
//    std::vector<Vertex> vertices;
//    std::vector<UINT> indices;
//};
// For test

// 서브매쉬
struct SubMesh
{
    ID3D11Buffer* vertexBuffer = nullptr;
    ID3D11Buffer* indexBuffer = nullptr;

    std::vector<AdvencedVertex> vertices;
    std::vector<UINT> indices;

    INT materialIndex = 0;

    void Free(void)
    {
        if (vertexBuffer)
            vertexBuffer->Release();
        if (indexBuffer)
            indexBuffer->Release();
        vertices.clear();
        indices.clear();
    }
};

// 이름은 메쉬인데 사실상 모델
struct MeshData
{
    std::vector<SubMesh> subMesh;

    std::vector<Material> materials;

    //std::unordered_map<std::string, Bone> boneMapping;
    std::vector<DirectX::XMFLOAT4X4> boneMatrix;
    std::map<std::string, AnimationInfo> animations;
    std::unordered_map<std::string, Node*> boneMapping;
    std::map<int, Node*> boneIndex;
    Node rootNode;

    MeshData(void) : subMesh{}, materials{}, boneMatrix{}, boneMapping{}, animations{}
    {

    }
    ~MeshData(void)
    {
        for (auto& item : subMesh)
        {
            item.Free();
        }
        subMesh.clear();

        for (auto& item : materials)
        {
            item.Free();
        }
        materials.clear();
    }
};

class MeshTest
{
public:

private:
    MeshTest(void) = delete;
    MeshTest(ID3D11Device* device, ID3D11DeviceContext* context);
    virtual void Free(void);
public:
    virtual ~MeshTest(void) { Free(); }
    static MeshTest* Create(ID3D11Device* device, ID3D11DeviceContext* context, const std::string& filePath);

    void Update(void);
    void Render(void);

private:
    // 동작
    void Render(ID3D11DeviceContext*& context, SubMesh& subMesh);
    void SetMaterialInShader(ID3D11DeviceContext*& context, ID3D11Buffer*& materialBuffer, const Material& material);
    void UpdateBones(ID3D11DeviceContext*& context, ID3D11Buffer*& pBoneCBuffer, std::map<int, Node*>& nodes);
    void UpdateBoneOrigin(ID3D11DeviceContext*& context, ID3D11Buffer*& pBoneCBuffer, std::map<int, Node*>& nodes);
    // 동작

    // 테스트
    //HRESULT CreateBuffers(ID3D11Device*& device, ID3D11Buffer*& vertexBuffer, ID3D11Buffer*& indexBuffer, std::vector<Vertex>& vertices, std::vector<UINT>& indices);
    HRESULT InputLayoutSetting(ID3D11Device*& device, ID3DBlob*& vertexShaderBlob, ID3D11InputLayout*& inputLayout);
    HRESULT CompileVertexShader(const std::wstring& filePath, ID3DBlob*& vertexShaderBlob, ID3DBlob*& errorBlob);
    // 테스트

    // 로드
    HRESULT LoadMesh(const std::string& filePath);
    HRESULT CreateBuffers(ID3D11Device*& device, ID3D11Buffer*& vertexBuffer, ID3D11Buffer*& indexBuffer, std::vector<AdvencedVertex>& vertices, std::vector<UINT>& indices);
    void LoadMaterialAndTextures(ID3D11Device*& device, ID3D11DeviceContext*& context, const aiScene*& scene, const std::string& currentPath, std::vector<Material>& materials);
    bool LoadBones(const aiScene*& scene, Node& node, std::unordered_map<std::string, Node*>& boneMapping, std::vector<DirectX::XMFLOAT4X4>& boneMatrix, std::map<int, Node*>& boneIndex);
    void LoadAnimation(const aiScene* scene, std::map<std::string, AnimationInfo>& animations);
    void GetMeshData(aiMesh*& mesh, std::vector<AdvencedVertex>& meshVertices, std::vector<UINT>& meshIndices);
    HRESULT LoadSubMesh(ID3D11Device*& device, ID3D11DeviceContext*& context, aiMesh*& mesh, MeshData& meshContainer);
    HRESULT LoadMesh(ID3D11Device*& device, ID3D11DeviceContext*& context, const std::string& filePath, MeshData& mesh);
    // 로드

    // 렌더에 필요한 정보
    HRESULT CreateBoneBuffer(ID3D11Device*& device, ID3D11Buffer*& pBoneCBuffer);
    HRESULT CreateMaterialBuffer(ID3D11Device*& device, ID3D11Buffer*& materialBuffer);
    HRESULT CompileShader(const std::wstring& filePath, const std::string& entryMethod, const std::string& shaderModel, ID3DBlob*& ShaderBlob, ID3DBlob*& errorBlob);
    HRESULT LoadVertexShader(ID3D11Device*& device, ID3DBlob*& vertexShaderBlob, ID3D11VertexShader*& vertexShader);
    HRESULT BonedInputLayoutSetting(ID3D11Device*& device, ID3DBlob*& vertexShaderBlob, ID3D11InputLayout*& inputLayout);
    HRESULT LoadPixelShader(ID3D11Device*& device, ID3DBlob*& pixelShaderBlob, ID3D11PixelShader*& pixelShader);
    HRESULT CreateSampler(ID3D11Device*& pDevice, ID3D11SamplerState*& g_pSamplerState);
    HRESULT LoadVertexShader(ID3D11Device*& device, const std::wstring& filePath, ID3D11VertexShader*& vertexShader, ID3D11InputLayout*& inputLayout);
    HRESULT LoadPixelShader(ID3D11Device*& device, const std::wstring& filePath, ID3D11PixelShader*& pixelShader, ID3D11SamplerState*& samplerState);
    HRESULT CreateRasterizerState(ID3D11Device*& device, ID3D11RasterizerState*& rasterizerState);
    HRESULT DisableNonBlend(ID3D11Device*& device, ID3D11BlendState*& blendState);
    // 렌더에 필요한 정보

    // 경로 편집
    std::wstring getAbsolutePathFromFile(const std::wstring& baseFilePath, const std::wstring& relativePath);
    // 경로 편집

    // Animation
    float GetFactor(double lastTime, double nextTime, double currentTime);
    DirectX::XMFLOAT3 Lerp(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, float factor);
    DirectX::XMFLOAT4 SLerp(const DirectX::XMFLOAT4& start, const DirectX::XMFLOAT4& end, float factor);
    DirectX::XMFLOAT3 Interpolate(std::vector<std::pair<float, DirectX::XMFLOAT3>>& list, float currentTime);
    DirectX::XMFLOAT4 Interpolate(std::vector<std::pair<float, DirectX::XMFLOAT4>>& list, float currentTime);
    // Animation

    ID3D11Device* device = nullptr;
    ID3D11DeviceContext* context = nullptr;

    MeshData mesh;
    Node node;

    ID3D11SamplerState* samplerState = nullptr;
    ID3D11VertexShader* vertexShader = nullptr;
    ID3D11InputLayout* inputLayout = nullptr;
    ID3D11PixelShader* pixelShader = nullptr;
    ID3D11BlendState* blendState = nullptr;
    ID3D11RasterizerState* g_pRasterizerState = nullptr;
    ID3D11Buffer* materialBuffer = nullptr;
    ID3D11Buffer* boneBuffer = nullptr;
    ID3D11Buffer* boneOriginBuffer = nullptr;
};



void ConvertMatrix(DirectX::XMFLOAT4X4& out, const aiMatrix4x4& in);
void GetNode(const aiNode* rootAiNode, Node& rootNode, std::unordered_map<std::string, Node*>& boneMapping);
void DecomposeMatrix(const DirectX::XMFLOAT4X4& matrix, DirectX::XMFLOAT3& position, DirectX::XMFLOAT4& rotation, DirectX::XMFLOAT3& scale, bool test = false);
void BoneMatrix(MeshData& mesh, Node& node, int frame);
void BoneMatrix(MeshData& mesh, int frame);
void matrixTest(Matrix& mat1, Matrix& mat2);
void CreateMatrix(Matrix& matrix, const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT4& quaternion, const DirectX::XMFLOAT3& scale);
void CreateMatrix(Matrix& matrix, const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT4& quaternion, const DirectX::XMFLOAT3& scale, const DirectX::XMFLOAT3& pivotPosition);
void GetAnimationStates(AnimationNode& node, const int keyframe, Matrix& matrix);
void GetAnimationStates(AnimationNode& node, const int keyframe, DirectX::XMFLOAT3& position, DirectX::XMFLOAT4& quaternion, DirectX::XMFLOAT3& scale);
void GetAnimationStates(AnimationNode& node, const int keyframe, Matrix& matrix, DirectX::XMFLOAT3& position, DirectX::XMFLOAT4& quaternion, DirectX::XMFLOAT3& scale);
void GetRelativeState(AnimationNode& node, const int keyframe, Matrix& transformationMatrix, Matrix& matrix);
void SetBoneMatrix(Node& rootNode);
void SetBoneMatrixInRelative(Node& rootNode);








/*

inline void BoneMatrix(MeshData& mesh, Node& node, int frame, const DirectX::XMMATRIX& parentMatrix)
{
    bool animated = false;
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT4 rotation;
    DirectX::XMFLOAT3 scale;
    DirectX::XMMATRIX nodeTransform = DirectX::XMMatrixIdentity();
    AnimationInfo& info = (*mesh.animations.begin()).second;
    for (size_t i = 0; i < info.nodes.size(); i++)
    {
        if (info.nodes[i].boneName != node.BoneName)
            continue;

        AnimationNode& aniNode = info.nodes[i];
        if (frame >= aniNode.positions.size())
            position = aniNode.positions.end()->second;
        else
            position = aniNode.positions[frame].second;

        if (frame >= aniNode.quaternionRotations.size())
            rotation = aniNode.quaternionRotations.end()->second;
        else
            rotation = aniNode.quaternionRotations[frame].second;

        if (frame >= aniNode.scales.size())
            scale = aniNode.scales.end()->second;
        else
            scale = aniNode.scales[frame].second;
        animated = true;
        break;
    }
    if (animated)
    {
        DirectX::XMMATRIX translationMatrix = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
        DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationQuaternion(DirectX::XMQuaternionRotationRollPitchYaw(rotation.x, rotation.y, rotation.z));
        DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

        nodeTransform = DirectX::XMLoadFloat4x4(&node.transformMatrix) * (translationMatrix * rotationMatrix * scaleMatrix);
    }


    // 자식 노드의 변환을 누적합니다.
    DirectX::XMMATRIX nodeMatrix = parentMatrix * nodeTransform;
    DirectX::XMStoreFloat4x4(&node.finalMatrix, DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&node.transformMatrix)) * nodeMatrix * DirectX::XMLoadFloat4x4(&node.offsetMatrix));

    for (auto& childNode : node.mChildren)
    {
        BoneMatrix(mesh, node, frame, nodeMatrix);
    }
}
inline void LoadBoneMesh(const aiScene*& scene, std::vector<AdvencedVertex>& meshVertices, std::vector<UINT>& meshIndices)
{
    aiMesh* mesh = nullptr;
    int boneID;
    aiBone* bone = nullptr;
    AdvencedVertex vertex;
    aiMatrix4x4 offsetMatrix;

    std::vector<AdvencedVertex> vertices;
    std::vector<UINT> indices;

    aiNode* rootNode = scene->mRootNode;
    for (UINT i = 0; i < rootNode->mNumChildren; ++i)
    {
        aiNode* node = rootNode->mChildren[i];
        if (node->mNumMeshes > 0)
        {
            for (UINT meshIdx = 0; meshIdx < node->mNumMeshes; ++meshIdx)
            {
                aiMesh* mesh = scene->mMeshes[node->mMeshes[meshIdx]];
                for (UINT boneIdx = 0; boneIdx < mesh->mNumBones; ++boneIdx)
                {
                    aiBone* bone = mesh->mBones[boneIdx];
                }
            }
        }
    }


    for (UINT meshIdx = 0; meshIdx < scene->mNumMeshes; meshIdx++)
    {
        vertices.clear();
        indices.clear();

        mesh = scene->mMeshes[meshIdx];

        std::vector<std::vector<std::pair<int, float>>> vertexBoneData(mesh->mNumVertices);

        // 본 데이터 가져오기
        for (UINT boneIdx = 0; boneIdx < mesh->mNumBones; boneIdx++)
        {
            bone = mesh->mBones[boneIdx];
            boneID = boneIdx;  // 본 ID 할당 (보통 본 이름을 기반으로 ID를 매핑함)

            // 본의 오프셋 행렬 저장 (애니메이션 적용 시 사용)
            offsetMatrix = bone->mOffsetMatrix;

            // 본이 영향을 주는 버텍스 정보
            for (UINT weightIdx = 0; weightIdx < bone->mNumWeights; weightIdx++)
            {
                aiVertexWeight weight = bone->mWeights[weightIdx];
                int vertexID = weight.mVertexId;
                float weightValue = weight.mWeight;

                vertexBoneData[vertexID].push_back({ boneID, weightValue });
            }
        }

        for (UINT i = 0; i < mesh->mNumVertices; i++)
        {
            ZeroMemory(&vertex, sizeof(AdvencedVertex));
            vertex.position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };

            if (mesh->HasNormals())
            {
                vertex.normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
            }

            if (mesh->HasTextureCoords(0))
            {
                vertex.texcoord = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
            }

            vertices.push_back(vertex);
        }

        size_t j = 0;
        // 정점에 본 데이터 적용 (최대 4개까지만 저장)
        for (UINT i = 0; i < mesh->mNumVertices; i++)
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

        // 인덱스 데이터 저장
        aiFace face;
        for (UINT i = 0; i < mesh->mNumFaces; i++)
        {
            face = mesh->mFaces[i];
            for (j = 0; j < face.mNumIndices; j++)
            {
                indices.push_back(face.mIndices[j]);
            }
        }


        //for (auto& vertex : vertices)
        //{
        //    meshVertices.push_back(vertex);
        //}
        //for (auto& index : indices)
        //{
        //    meshIndices.push_back(index);
        //}
        meshVertices.insert(meshVertices.end(), vertices.begin(), vertices.end());
        meshIndices.insert(meshIndices.end(), indices.begin(), indices.end());
    }
}





inline void GetMeshDataWithNoSubMesh(const aiScene*& scene, std::vector<AdvencedVertex>& meshVertices, std::vector<UINT>& meshIndices)
{

    aiMesh* mesh = nullptr;
    int boneID;
    aiBone* bone = nullptr;
    AdvencedVertex vertex;
    aiMatrix4x4 offsetMatrix;

    std::vector<AdvencedVertex> vertices;
    std::vector<UINT> indices;


    for (UINT meshIdx = 0; meshIdx < scene->mNumMeshes; meshIdx++)
    {
        vertices.clear();
        indices.clear();

        mesh = scene->mMeshes[meshIdx];

        std::vector<std::vector<std::pair<int, float>>> vertexBoneData(mesh->mNumVertices);

        // 본 데이터 가져오기
        for (UINT boneIdx = 0; boneIdx < mesh->mNumBones; boneIdx++)
        {
            bone = mesh->mBones[boneIdx];
            boneID = boneIdx;  // 본 ID 할당 (보통 본 이름을 기반으로 ID를 매핑함)

            // 본의 오프셋 행렬 저장 (애니메이션 적용 시 사용)
            offsetMatrix = bone->mOffsetMatrix;

            // 본이 영향을 주는 버텍스 정보
            for (UINT weightIdx = 0; weightIdx < bone->mNumWeights; weightIdx++)
            {
                aiVertexWeight weight = bone->mWeights[weightIdx];
                int vertexID = weight.mVertexId;
                float weightValue = weight.mWeight;

                vertexBoneData[vertexID].push_back({ boneID, weightValue });
            }
        }

        for (UINT i = 0; i < mesh->mNumVertices; i++)
        {
            ZeroMemory(&vertex, sizeof(AdvencedVertex));
            vertex.position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };

            if (mesh->HasNormals())
            {
                vertex.normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
            }

            if (mesh->HasTextureCoords(0))
            {
                vertex.texcoord = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
            }

            vertices.push_back(vertex);
        }

        size_t j = 0;
        // 정점에 본 데이터 적용 (최대 4개까지만 저장)
        for (UINT i = 0; i < mesh->mNumVertices; i++)
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

        // 인덱스 데이터 저장
        aiFace face;
        for (UINT i = 0; i < mesh->mNumFaces; i++)
        {
            face = mesh->mFaces[i];
            for (j = 0; j < face.mNumIndices; j++)
            {
                indices.push_back(face.mIndices[j]);
            }
        }

        meshVertices.insert(meshVertices.end(), vertices.begin(), vertices.end());
        meshIndices.insert(meshIndices.end(), indices.begin(), indices.end());
    }
}





inline HRESULT CompileVertexShader(const std::wstring& filePath, ID3DBlob*& vertexShaderBlob, ID3DBlob*& errorBlob)
{
    HRESULT hr = D3DCompileFromFile(
        filePath.c_str(),    // 셰이더 파일 경로
        nullptr, nullptr,   // 매크로 및 포함 파일 없음
        "VS_Main",          // 엔트리 포인트 함수 (Vertex Shader)
        "vs_5_0",           // 셰이더 모델 (Vertex Shader 5.0)
        D3DCOMPILE_ENABLE_STRICTNESS,  // 컴파일 옵션
        0,                  // 기타 옵션 없음
        &vertexShaderBlob,  // 출력 Blob (셰이더 코드)
        &errorBlob          // 오류 메시지
    );

    if (FAILED(hr))
    {
        if (errorBlob)
        {
            OutputDebugStringA((char*)errorBlob->GetBufferPointer());
            errorBlob->Release();
        }
        return hr;
    }
    return S_OK;
}
inline HRESULT CompileShader(const std::wstring& filePath, const std::string& entryMethod, const std::string& shaderModel, ID3DBlob*& ShaderBlob, ID3DBlob*& errorBlob)
{
    HRESULT hr = D3DCompileFromFile(
        filePath.c_str(),    // 셰이더 파일 경로
        nullptr, nullptr,   // 매크로 및 포함 파일 없음
        entryMethod.c_str(),          // 엔트리 포인트 함수 (Vertex Shader)
        shaderModel.c_str(),           // 셰이더 모델 (Vertex Shader 5.0)
        D3DCOMPILE_ENABLE_STRICTNESS,  // 컴파일 옵션
        0,                  // 기타 옵션 없음
        &ShaderBlob,  // 출력 Blob (셰이더 코드)
        &errorBlob          // 오류 메시지
    );

    if (FAILED(hr))
    {
        if (errorBlob)
        {
            OutputDebugStringA((char*)errorBlob->GetBufferPointer());
            errorBlob->Release();
        }
        return hr;
    }
    return S_OK;
}

inline HRESULT LoadVertexShader(ID3D11Device*& device, ID3DBlob*& vertexShaderBlob, ID3D11VertexShader*& vertexShader)
{
    return device->CreateVertexShader(
        vertexShaderBlob->GetBufferPointer(),
        vertexShaderBlob->GetBufferSize(),
        nullptr,
        &vertexShader
    );
}

inline HRESULT InputLayoutSetting(ID3D11Device*& device, ID3DBlob*& vertexShaderBlob, ID3D11InputLayout*& inputLayout)
{
    D3D11_INPUT_ELEMENT_DESC layout[] = {
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
    UINT numElements = ARRAYSIZE(layout);

    return device->CreateInputLayout(layout, numElements, vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), &inputLayout);
}
inline HRESULT BonedInputLayoutSetting(ID3D11Device*& device, ID3DBlob*& vertexShaderBlob, ID3D11InputLayout*& inputLayout)
{
    D3D11_INPUT_ELEMENT_DESC layout[] = {
        { "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT,      0, 24,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "boneIDs",   0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 32,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "weights",   0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
    UINT numElements = ARRAYSIZE(layout);

    return device->CreateInputLayout(layout, numElements, vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), &inputLayout);
}


inline HRESULT LoadPixelShader(ID3D11Device*& device, ID3DBlob*& pixelShaderBlob, ID3D11PixelShader*& pixelShader)
{
    return device->CreatePixelShader(
        pixelShaderBlob->GetBufferPointer(),
        pixelShaderBlob->GetBufferSize(),
        nullptr,
        &pixelShader
    );
}
inline HRESULT CreateSampler(ID3D11Device*& pDevice, ID3D11SamplerState*& g_pSamplerState)
{
    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    return pDevice->CreateSamplerState(&samplerDesc, &g_pSamplerState);
}

inline HRESULT LoadVertexShader(ID3D11Device*& device, const std::wstring& filePath, ID3D11VertexShader*& vertexShader, ID3D11InputLayout*& inputLayout)
{
    ID3DBlob* vertexShaderBlob = nullptr;
    ID3DBlob* errorBlob = nullptr;
    if (FAILED(CompileShader(L"VertexShader.hlsl", "VS_Main", "vs_5_0", vertexShaderBlob, errorBlob)))
    {
        if (vertexShaderBlob)
            vertexShaderBlob->Release();
        if (errorBlob)
            errorBlob->Release();

        return E_FAIL;
    }
    if (FAILED(LoadVertexShader(device, vertexShaderBlob, vertexShader)))
    {
        if (vertexShaderBlob)
            vertexShaderBlob->Release();
        if (errorBlob)
            errorBlob->Release();
        if (vertexShader)
            vertexShader->Release();
        vertexShader = nullptr;
        return E_FAIL;
    }
    if(FAILED(BonedInputLayoutSetting(device, vertexShaderBlob, inputLayout)))
    {
        if (vertexShaderBlob)
            vertexShaderBlob->Release();
        if (errorBlob)
            errorBlob->Release();
        if (vertexShader)
            vertexShader->Release();
        vertexShader = nullptr;
        if (inputLayout)
            inputLayout->Release();
        inputLayout = nullptr;
        return E_FAIL;
    }

    if (vertexShaderBlob)
        vertexShaderBlob->Release();
    if (errorBlob)
        errorBlob->Release();
    return S_OK;
}
inline HRESULT LoadPixelShader(ID3D11Device*& device, const std::wstring& filePath, ID3D11PixelShader*& pixelShader, ID3D11SamplerState*& samplerState)
{
    ID3DBlob* vertexShaderBlob = nullptr;
    ID3DBlob* errorBlob = nullptr;
    if (FAILED(CompileShader(L"PixelShader.hlsl", "PS_Main", "ps_5_0", vertexShaderBlob, errorBlob)))
    {
        if (vertexShaderBlob)
            vertexShaderBlob->Release();
        if (errorBlob)
            errorBlob->Release();

        return E_FAIL;
    }
    if (FAILED(LoadPixelShader(device, vertexShaderBlob, pixelShader)))
    {
        if (vertexShaderBlob)
            vertexShaderBlob->Release();
        if (errorBlob)
            errorBlob->Release();
        if (pixelShader)
            pixelShader->Release();
        pixelShader = nullptr;
        return E_FAIL;
    }
    if (FAILED(CreateSampler(device, samplerState)))
    {
        if (vertexShaderBlob)
            vertexShaderBlob->Release();
        if (errorBlob)
            errorBlob->Release();
        if (pixelShader)
            pixelShader->Release();
        pixelShader = nullptr;
        if (samplerState)
            samplerState->Release();
        samplerState = nullptr;

    }


    return S_OK;
}

inline void CreateRasterizerState(ID3D11Device*& device, ID3D11RasterizerState*& rasterizerState)
{
    D3D11_RASTERIZER_DESC rasterDesc = {};
    rasterDesc.FillMode = D3D11_FILL_SOLID; // 기본 값 (와이어프레임 모드: D3D11_FILL_WIREFRAME)
    rasterDesc.CullMode = D3D11_CULL_NONE;  // 컬링 없음 (DirectX 9의 D3DCULL_NONE과 동일)
    rasterDesc.FrontCounterClockwise = FALSE; // 기본값 (반시계 방향이 앞면)

    HRESULT hr = device->CreateRasterizerState(&rasterDesc, &rasterizerState);
    if (FAILED(hr))
    {
        MessageBoxA(0, "Failed to create rasterizer state!", "Error", MB_OK);
    }
}

inline std::wstring getAbsolutePathFromFile(const std::wstring& baseFilePath, const std::wstring& relativePath)
{
    wchar_t full_path[MAX_PATH];

    // baseFilePath에서 디렉터리 경로 추출
    std::wstring baseDir = baseFilePath;
    size_t pos = baseDir.find_last_of(L"\\/");
    if (pos != std::wstring::npos)
    {
        baseDir = baseDir.substr(0, pos);  // 파일 경로에서 디렉터리만 남긴다
    }

    // baseDir을 기준으로 상대 경로를 절대 경로로 변환
    std::wstring fullRelativePath = baseDir + L"\\" + relativePath;

    if (GetFullPathNameW(fullRelativePath.c_str(), MAX_PATH, full_path, nullptr))
    {
        return std::wstring(full_path);
    }
    else
    {
        return L"";
    }
}


inline HRESULT DisableNonBlend(ID3D11Device*& device, ID3D11BlendState*& blendState)
{
    D3D11_BLEND_DESC blendDesc;
    ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));
    blendDesc.RenderTarget[0].BlendEnable = FALSE;
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    return device->CreateBlendState(&blendDesc, &blendState);
}

*/





/*

enum MatrixType
{
    DXWorld,
    DXProjection,
    End
};

namespace Engine
{

    class MatrixManager final : public Base
    {
    private:
        MatrixManager(ID3D11Device*& device, ID3D11DeviceContext*& context);
        ~MatrixManager(void) = default;

        virtual void Free(void) override;
    public:
        static MatrixManager* Create(ID3D11Device*& device, ID3D11DeviceContext*& context);

        HRESULT Initialize(void);

        HRESULT CreateMatrixBuffer(ID3D11Device*& device, ID3D11Buffer*& matrixBuffer);
        HRESULT SetTransform(MatrixType type, const XMMATRIX& matrix);

    private:
        std::vector<ID3D11Buffer*> matrixBuffer;
        ID3D11Device* device;
        ID3D11DeviceContext* context;

    };
}

*/


/*
struct BoneMatrixX
{
    std::string name;
    aiMatrix4x4 offsetMatrix;
    aiMatrix4x4 finalTransformation;
};
struct Vertex5
{
    aiVector3D position;
    aiVector3D normal;
    aiVector2D texCoord;
    std::vector<int> boneIDs;
    std::vector<float> boneWeights;
};

inline void ApplyBoneMatricesToVertices(std::vector<Vertex5>& vertices, const std::vector<BoneMatrixX>& boneMatrices)
{
    for (auto& vertex : vertices)
    {
        aiMatrix4x4 transformation;
        aiMatrix4x4 finalTransformation = aiMatrix4x4();
        for (size_t i = 0; i < vertex.boneIDs.size(); ++i)
        {
            int boneID = vertex.boneIDs[i];
            float weight = vertex.boneWeights[i];
            transformation = (boneMatrices[boneID].finalTransformation * weight);
            finalTransformation = finalTransformation + transformation;
        }
        // 정점에 최종 변환 행렬 적용
        vertex.position = finalTransformation * vertex.position;
        vertex.normal = finalTransformation * vertex.normal;
    }
}

// Assimp를 사용하여 모델 로드 및 본 행렬 생성 함수
inline void LoadModelAndCreateBoneMatrices(const std::string& modelPath)
{
    // Assimp 임포터 생성
    Assimp::Importer importer;

    // 모델 파일 로드
    const aiScene* scene = importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene)
    {
        //std::cerr << "모델 로드 실패: " << importer.GetErrorString() << std::endl;
        return;
    }

    // 본 행렬을 저장할 벡터
    std::vector<BoneMatrixX> boneMatrices;

    // 본 정보를 추출하는 재귀 함수
    std::function<void(const aiNode*, const aiMatrix4x4&)> processNode;
    processNode = [&](const aiNode* node, const aiMatrix4x4& parentTransform)
        {
            aiMatrix4x4 nodeTransform = node->mTransformation;
            aiMatrix4x4 globalTransform = parentTransform * nodeTransform;

            // 본이 있는지 확인
            if (scene->mMeshes)
            {
                for (unsigned int i = 0; i < node->mNumMeshes; ++i)
                {
                    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
                    if (mesh->HasBones())
                    {
                        for (unsigned int j = 0; j < mesh->mNumBones; ++j)
                        {
                            aiBone* bone = mesh->mBones[j];
                            BoneMatrixX boneMatrix;
                            boneMatrix.name = bone->mName.C_Str();
                            boneMatrix.offsetMatrix = bone->mOffsetMatrix;
                            boneMatrix.finalTransformation = globalTransform * bone->mOffsetMatrix;
                            boneMatrices.push_back(boneMatrix);
                        }
                    }
                }
            }

            // 자식 노드 처리
            for (unsigned int i = 0; i < node->mNumChildren; ++i)
            {
                processNode(node->mChildren[i], globalTransform);
            }
        };

    // 루트 노드부터 시작하여 본 정보 추출
    processNode(scene->mRootNode, aiMatrix4x4());
    int alarm = 0;
    // 추출된 본 행렬 출력 (디버깅용)
    //for (const auto& boneMatrix : boneMatrices)
    //{
    //    std::cout << "본 이름: " << boneMatrix.name << std::endl;
    //    std::cout << "오프셋 행렬: " << boneMatrix.offsetMatrix << std::endl;
    //    std::cout << "최종 변환 행렬: " << boneMatrix.finalTransformation << std::endl;
    //}

    std::vector<Vertex5> vertices;
    Vertex5 vertex = { { 1.0f,1.0f ,1.0f }, { 1.0f ,1.0f ,1.0f }, { 1.0f ,1.0f } };
    vertex.boneIDs.push_back(18);
    vertex.boneWeights.push_back(1.0f);
    vertices.push_back(vertex);

    ApplyBoneMatricesToVertices(vertices, boneMatrices);
    int alarm2 = 0;
}

*/
