#pragma once


class MeshTest2
{
public:
    struct Bone
    {
        std::string name;
        int index;
        DirectX::XMFLOAT4X4 finalMatrix;
        //aiMatrix4x4 finalMatrix;
        DirectX::XMFLOAT4X4 combinedFinalMatrix;
        //aiMatrix4x4 combinedFinalMatrix;
        aiBone* bone;
    };

    struct Vertex
    {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT3 normal;  
        DirectX::XMFLOAT2 texCoord;
        UINT boneIDs[4]; 
        FLOAT weights[4];
    };

    struct SubMesh
    {
        aiMesh* mesh;
        ID3D11Buffer* vertex;
        ID3D11Buffer* index;
        UINT indexCount;

        void Free(void)
        {
            if(vertex)
                vertex->Release();
            if (index)
                index->Release();
        }
    };

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

private:
    MeshTest2(void) = delete;
    MeshTest2(ID3D11Device* device, ID3D11DeviceContext* context);
    virtual void Free(void);



public:
    virtual ~MeshTest2(void) { Free(); }
    static MeshTest2* Create(ID3D11Device* device, ID3D11DeviceContext* context, const std::string& filePath);

    void Update(void);
    void Render(void);

private:
    HRESULT CreateMesh(const std::string& filePath);
    HRESULT LoadMesh(ID3D11Device* device, ID3D11DeviceContext* context, const std::string& filePath, Assimp::Importer& importer, const aiScene*& scene);
    HRESULT CreateMesh(ID3D11Device* device, const aiScene* scene, std::vector<SubMesh>& mesh, std::unordered_map<std::string, Bone>& bones);
    void ProcessMesh(aiMesh*& mesh, std::vector<Vertex>& vertices, std::vector<UINT>& indices, std::unordered_map<std::string, Bone>& bones);
    HRESULT CreateBuffers(ID3D11Device* device, std::vector<Vertex>& vertices, std::vector<UINT>& indices, SubMesh& mesh);
    HRESULT LoadTexture(ID3D11Device* device, const std::wstring& textureFilePath, ID3D11ShaderResourceView*& texture, bool dds = true);
    void LoadMaterialAndTextures(ID3D11Device*& device, ID3D11DeviceContext*& context, const aiScene*& scene, const std::string& currentPath, std::vector<Material>& materials);


    ID3D11Device* device = nullptr;
    ID3D11DeviceContext* context = nullptr;
    Assimp::Importer importer;

    const aiScene* scene;
    std::vector<SubMesh> mesh;
    std::vector<UINT> indexCounts;
    std::unordered_map<std::string, Bone> bones;
    std::vector<Material> materials;

    UINT stride = sizeof(Vertex);
    UINT offset = 0;


    void UpdateAnimation(const aiScene* scene, float deltaTime, std::unordered_map<std::string, Bone>& bones)
    {
        aiAnimation* animation = scene->mAnimations[0];
        aiNodeAnim* nodeAnim = nullptr;
        // 애니메이션의 모든 채널(본)에 대해 변환을 업데이트
        for (UINT j = 0; j < animation->mNumChannels; j++)
        {
            nodeAnim = animation->mChannels[j];
            auto bone = bones.find(nodeAnim->mNodeName.C_Str());
            if (bone == bones.end())
                continue;

            // 애니메이션의 현재 시간에 맞는 변환 계산
            // (보간 없이 키프레임에서 가장 가까운 키프레임을 선택)
            float animationTime = fmod(deltaTime * animation->mTicksPerSecond, animation->mDuration);

            aiVector3D position = GetKeyframePosition(nodeAnim, animationTime);
            aiQuaternion rotation = GetKeyframeRotation(nodeAnim, animationTime);
            aiVector3D scale = GetKeyframeScale(nodeAnim, animationTime);

            // 변환 행렬 계산
            DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
            DirectX::XMMATRIX rotationMat = DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(rotation.x, rotation.y, rotation.z, rotation.w));
            DirectX::XMMATRIX scaleMat = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

            // 최종 본 변환 행렬
            DirectX::XMMATRIX finalTransform = DirectX::XMMatrixMultiply(translation, DirectX::XMMatrixMultiply(rotationMat, scaleMat));

            DirectX::XMStoreFloat4x4(&(*bone).second.finalMatrix, finalTransform);
        }
    }
    void CalculateBoneTransforms(const aiScene* scene, aiNode* rootNode, std::unordered_map<std::string, Bone>& bones)
    {
        std::list<std::pair<aiNode*, DirectX::XMMATRIX>> nodeQueue;
        nodeQueue.push_back(std::pair<aiNode*, DirectX::XMMATRIX>(rootNode, DirectX::XMMatrixIdentity()));
        aiNode* node = nullptr;

        while (!nodeQueue.empty())
        {
            node = nodeQueue.front().first;
            DirectX::XMMATRIX parentTransform = nodeQueue.front().second;
            nodeQueue.pop_front();

            auto boneIterator = bones.find(node->mName.C_Str());

            // 현재 본의 변환을 부모 변환과 결합
            DirectX::XMMATRIX currentTransform = DirectX::XMLoadFloat4x4(&boneIterator->second.finalMatrix);
            currentTransform *= DirectX::XMMatrixMultiply(parentTransform, ConvertToDirectXMatrix(node->mTransformation));
            DirectX::XMStoreFloat4x4(&boneIterator->second.combinedFinalMatrix, currentTransform);

            // 자식 본들에 대해 재귀적으로 변환 계산
            for (unsigned int i = 0; i < node->mNumChildren; ++i)
            {
                nodeQueue.push_back({ node->mChildren[i], currentTransform });
            }
        }
    }


    aiVector3D GetKeyframePosition(aiNodeAnim* nodeAnim, float animationTime)
    {
        if (nodeAnim->mNumPositionKeys == 0)
            return aiVector3D(0.0f, 0.0f, 0.0f); // 기본값

        // 가장 가까운 키프레임을 찾아서 리턴
        for (UINT i = 0; i < nodeAnim->mNumPositionKeys - 1; i++)
        {
            aiVectorKey currentKey = nodeAnim->mPositionKeys[i];
            aiVectorKey nextKey = nodeAnim->mPositionKeys[i + 1];

            if (animationTime >= currentKey.mTime && animationTime < nextKey.mTime)
            {
                // 키프레임이 두 개 사이에 있으면, 현재 키프레임을 반환
                return currentKey.mValue;
            }
        }

        // 마지막 키프레임 반환
        return nodeAnim->mPositionKeys[nodeAnim->mNumPositionKeys - 1].mValue;
    }
    aiQuaternion GetKeyframeRotation(aiNodeAnim* nodeAnim, float animationTime)
    {
        if (nodeAnim->mNumRotationKeys == 0)
            return aiQuaternion(0.0f, 0.0f, 0.0f, 1.0f); // 기본값

        // 가장 가까운 회전 키프레임을 찾아서 리턴
        for (UINT i = 0; i < nodeAnim->mNumRotationKeys - 1; i++)
        {
            aiQuatKey currentKey = nodeAnim->mRotationKeys[i];
            aiQuatKey nextKey = nodeAnim->mRotationKeys[i + 1];

            if (animationTime >= currentKey.mTime && animationTime < nextKey.mTime)
            {
                // 키프레임이 두 개 사이에 있으면, 현재 키프레임을 반환
                return currentKey.mValue;
            }
        }

        // 마지막 키프레임 반환
        return nodeAnim->mRotationKeys[nodeAnim->mNumRotationKeys - 1].mValue;
    }
    aiVector3D GetKeyframeScale(aiNodeAnim* nodeAnim, float animationTime)
    {
        if (nodeAnim->mNumScalingKeys == 0)
            return aiVector3D(1.0f, 1.0f, 1.0f); // 기본값

        // 가장 가까운 크기 키프레임을 찾아서 리턴
        for (UINT i = 0; i < nodeAnim->mNumScalingKeys - 1; i++)
        {
            aiVectorKey currentKey = nodeAnim->mScalingKeys[i];
            aiVectorKey nextKey = nodeAnim->mScalingKeys[i + 1];

            if (animationTime >= currentKey.mTime && animationTime < nextKey.mTime)
            {
                // 키프레임이 두 개 사이에 있으면, 현재 키프레임을 반환
                return currentKey.mValue;
            }
        }

        // 마지막 키프레임 반환
        return nodeAnim->mScalingKeys[nodeAnim->mNumScalingKeys - 1].mValue;
    }
    DirectX::XMMATRIX ConvertToDirectXMatrix(const aiMatrix4x4& aiMatrix)
    {
        return DirectX::XMMatrixSet(
            aiMatrix.a1, aiMatrix.a2, aiMatrix.a3, aiMatrix.a4,
            aiMatrix.b1, aiMatrix.b2, aiMatrix.b3, aiMatrix.b4,
            aiMatrix.c1, aiMatrix.c2, aiMatrix.c3, aiMatrix.c4,
            aiMatrix.d1, aiMatrix.d2, aiMatrix.d3, aiMatrix.d4
        );
    }


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
    HRESULT CompileVertexShader(const std::wstring& filePath, ID3DBlob*& vertexShaderBlob, ID3DBlob*& errorBlob);
    HRESULT InputLayoutSetting(ID3D11Device*& device, ID3DBlob*& vertexShaderBlob, ID3D11InputLayout*& inputLayout);
    std::wstring getAbsolutePathFromFile(const std::wstring& baseFilePath, const std::wstring& relativePath);

    HRESULT Load(const std::string& filePath);
    void SetMaterialInShader(ID3D11DeviceContext*& context, ID3D11Buffer*& materialBuffer, const Material& material);
    void Render(ID3D11DeviceContext*& context, SubMesh& subMesh);

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


/*
inline void LoadBones(aiMesh*& mesh, std::unordered_map<std::string, MeshTest2::Bone>& bones)
{
    for (UINT i = 0; i < mesh->mNumBones; i++)
    {
        aiBone* bone = mesh->mBones[i];
        std::string boneName = bone->mName.C_Str();

        if (bones.find(boneName) == bones.end())
        {
            MeshTest2::Bone newBone;
            newBone.name = boneName;
            newBone.index = i;
            newBone.offsetMatrix = bone->mOffsetMatrix;
            bones[boneName] = newBone;
        }
    }
}

inline void ApplyAnimation(const aiScene*& scene, std::unordered_map<std::string, MeshTest2::Bone>& bones, float time)
{
    aiAnimation* animation = scene->mAnimations[0];

    for (UINT i = 0; i < animation->mNumChannels; i++)
    {
        aiNodeAnim* channel = animation->mChannels[i];

        std::string boneName = channel->mNodeName.C_Str();
        if (bones.find(boneName) == bones.end()) continue;

        MeshTest2::Bone& bone = bones[boneName];

        aiVector3D position = channel->mPositionKeys[0].mValue;

        for (UINT j = 0; j < channel->mNumPositionKeys - 1; j++)
        {
            if (time < channel->mPositionKeys[j + 1].mTime)
            {
                float t = (time - channel->mPositionKeys[j].mTime) /
                    (channel->mPositionKeys[j + 1].mTime - channel->mPositionKeys[j].mTime);
                position = channel->mPositionKeys[j].mValue * (1.0f - t) +
                    channel->mPositionKeys[j + 1].mValue * t;
                break;
            }
        }

        aiMatrix4x4 transform;
        aiMatrix4x4::Translation(position, transform);
        bone.offsetMatrix = transform;
    }
}*/




































#include "pch.h"
#include "MeshTest.h"
#include "InputManager.h"

using namespace DirectX;


HRESULT MeshTest2::Load(const std::string& filePath)
{
    if (!device || !context)
        return E_FAIL;

    if (FAILED(LoadVertexShader(device, L"BoneVertexShader.hlsl", vertexShader, inputLayout)))
    {
        MessageBox(NULL, L"ERROR : BoneVertexShader", L"ERROR", MB_OK);
    }
    if (FAILED(LoadPixelShader(device, L"PixelShader.hlsl", pixelShader, samplerState)))
    {
        MessageBox(NULL, L"ERROR : PixelShader", L"ERROR", MB_OK);
    }
    if (FAILED(CreateRasterizerState(device, g_pRasterizerState)))
    {
        MessageBox(NULL, L"ERROR : CreateRasterizerState", L"ERROR", MB_OK);
    }
    if (FAILED(CreateMaterialBuffer(device, materialBuffer)))
    {
        MessageBox(NULL, L"ERROR : CreateMaterialBuffer", L"ERROR", MB_OK);
    }
    if (FAILED(DisableNonBlend(device, blendState)))
    {
        MessageBox(NULL, L"ERROR : DisableNonBlend", L"ERROR", MB_OK);
    }
    if (FAILED(CreateBoneBuffer(device, boneBuffer)))
    {
        MessageBox(NULL, L"ERROR : CreateBoneBuffer", L"ERROR", MB_OK);
    }
    if (FAILED(CreateBoneBuffer(device, boneOriginBuffer)))
    {
        MessageBox(NULL, L"ERROR : CreateBoneOriginBuffer", L"ERROR", MB_OK);
    }
    return S_OK;
}

void MeshTest2::SetMaterialInShader(ID3D11DeviceContext*& context, ID3D11Buffer*& materialBuffer, const Material& material)
{
    // 디퓨즈 텍스처 바인딩
    context->PSSetShaderResources(0, 1, &material.diffuseTexture);
    // 스펙큘러 텍스처 바인딩
    //context->PSSetShaderResources(1, 1, &material.specularTexture);
    // 노멀맵 텍스쳐 바인딩
    //context->PSSetShaderResources(1, 1, &material.normalTexture);


    D3D11_MAPPED_SUBRESOURCE mappedResource;
    if (SUCCEEDED(context->Map(materialBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
    {
        MaterialBufferType* dataPtr = (MaterialBufferType*)mappedResource.pData;
        dataPtr->diffuseColor = material.diffuseColor;
        dataPtr->specularColor = material.specularColor;
        dataPtr->shininess = material.shininess;
        dataPtr->hasSpecularTexture = (material.specularTexture != nullptr) ? 1 : 0;
        context->Unmap(materialBuffer, 0);

        // 셰이더에 바인딩
        context->PSSetConstantBuffers(1, 1, &materialBuffer);
    }
}

void MeshTest2::Render(ID3D11DeviceContext*& context, SubMesh& subMesh)
{
    UINT stride = sizeof(AdvencedVertex);
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, &subMesh.vertex, &stride, &offset);
    context->IASetIndexBuffer(subMesh.index, DXGI_FORMAT_R32_UINT, 0);
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    context->DrawIndexed((UINT)subMesh.indexCount, 0, 0);
}



HRESULT MeshTest2::CreateBoneBuffer(ID3D11Device*& device, ID3D11Buffer*& pBoneCBuffer)
{
    D3D11_BUFFER_DESC cbDesc = {};
    cbDesc.Usage = D3D11_USAGE_DYNAMIC;            // 동적으로 업데이트 가능
    cbDesc.ByteWidth = sizeof(BoneBuffer);         // 버퍼 크기
    cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // 상수 버퍼로 사용
    cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // CPU가 쓰기 가능
    cbDesc.MiscFlags = 0;

    return device->CreateBuffer(&cbDesc, nullptr, &pBoneCBuffer);
}

HRESULT MeshTest2::CreateMaterialBuffer(ID3D11Device*& device, ID3D11Buffer*& materialBuffer)
{
    D3D11_BUFFER_DESC materialBufferDesc = {};
    materialBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    materialBufferDesc.ByteWidth = sizeof(MaterialBufferType);
    materialBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    materialBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    return device->CreateBuffer(&materialBufferDesc, NULL, &materialBuffer);

}

HRESULT MeshTest2::CompileVertexShader(const std::wstring& filePath, ID3DBlob*& vertexShaderBlob, ID3DBlob*& errorBlob)
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
HRESULT MeshTest2::CompileShader(const std::wstring& filePath, const std::string& entryMethod, const std::string& shaderModel, ID3DBlob*& ShaderBlob, ID3DBlob*& errorBlob)
{
    HRESULT hr = D3DCompileFromFile(
        filePath.c_str(),    // 셰이더 파일 경로
        nullptr, nullptr,   // 매크로 및 포함 파일 없음
        entryMethod.c_str(),          // 엔트리 포인트 함수 (Vertex Shader)
        shaderModel.c_str(),           // 셰이더 모델 (Vertex Shader 5.0)
        D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_SKIP_OPTIMIZATION,  // 컴파일 옵션
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

HRESULT MeshTest2::LoadVertexShader(ID3D11Device*& device, ID3DBlob*& vertexShaderBlob, ID3D11VertexShader*& vertexShader)
{
    return device->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), nullptr, &vertexShader);
}

HRESULT MeshTest2::InputLayoutSetting(ID3D11Device*& device, ID3DBlob*& vertexShaderBlob, ID3D11InputLayout*& inputLayout)
{
    D3D11_INPUT_ELEMENT_DESC layout[] = {
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
    UINT numElements = ARRAYSIZE(layout);

    return device->CreateInputLayout(layout, numElements, vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), &inputLayout);
}
HRESULT MeshTest2::BonedInputLayoutSetting(ID3D11Device*& device, ID3DBlob*& vertexShaderBlob, ID3D11InputLayout*& inputLayout)
{
    D3D11_INPUT_ELEMENT_DESC layout[] = {
       { "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
       { "NORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
       { "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT,      0, 24,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
       { "BONES",     0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 32,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
       { "WEIGHTS",   0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 48,  D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
    UINT numElements = ARRAYSIZE(layout);

    return device->CreateInputLayout(layout, numElements, vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), &inputLayout);
}


HRESULT MeshTest2::LoadPixelShader(ID3D11Device*& device, ID3DBlob*& pixelShaderBlob, ID3D11PixelShader*& pixelShader)
{
    return device->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), nullptr, &pixelShader);
}
HRESULT MeshTest2::CreateSampler(ID3D11Device*& pDevice, ID3D11SamplerState*& g_pSamplerState)
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

HRESULT MeshTest2::LoadVertexShader(ID3D11Device*& device, const std::wstring& filePath, ID3D11VertexShader*& vertexShader, ID3D11InputLayout*& inputLayout)
{
    ID3DBlob* vertexShaderBlob = nullptr;
    ID3DBlob* errorBlob = nullptr;
    if (FAILED(CompileShader(filePath, "VS_Main", "vs_5_0", vertexShaderBlob, errorBlob)))
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
    if (FAILED(BonedInputLayoutSetting(device, vertexShaderBlob, inputLayout)))
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
HRESULT MeshTest2::LoadPixelShader(ID3D11Device*& device, const std::wstring& filePath, ID3D11PixelShader*& pixelShader, ID3D11SamplerState*& samplerState)
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

HRESULT MeshTest2::CreateRasterizerState(ID3D11Device*& device, ID3D11RasterizerState*& rasterizerState)
{
    D3D11_RASTERIZER_DESC rasterDesc = {};
    rasterDesc.FillMode = D3D11_FILL_SOLID; // 기본 값 (와이어프레임 모드: D3D11_FILL_WIREFRAME)
    rasterDesc.CullMode = D3D11_CULL_NONE;  // 컬링 없음 (DirectX 9의 D3DCULL_NONE과 동일)
    rasterDesc.FrontCounterClockwise = FALSE; // 기본값 (반시계 방향이 앞면)

    return device->CreateRasterizerState(&rasterDesc, &rasterizerState);
}


std::wstring MeshTest2::getAbsolutePathFromFile(const std::wstring& baseFilePath, const std::wstring& relativePath)
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


HRESULT MeshTest2::DisableNonBlend(ID3D11Device*& device, ID3D11BlendState*& blendState)
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

