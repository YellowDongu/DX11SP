#include "Foundation.h"
#include "Mesh.h"
#include "GameInstance.h"
#include "Bone.h"
using namespace Engine;

Mesh::Mesh(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : BaseObject(dxDevice, dxDeviceContext), vertexBuffer(nullptr), indexBuffer(nullptr), model(nullptr), materialIndex(0), stride(0), offset(0), indexCount(0)
{
}

void Mesh::Free(void)
{
    if (vertexBuffer)
        vertexBuffer->Release();
    if (indexBuffer)
        indexBuffer->Release();
    vertexBuffer = nullptr;
    indexBuffer = nullptr;
}

Mesh* Mesh::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, ModelConverter::FullModelMesh& subMesh, bool saveVertex, bool staticModel)
{
    Mesh* newInstance = new Mesh(dxDevice, dxDeviceContext);
    if (FAILED(newInstance->LoadMesh(subMesh, saveVertex, staticModel)))
    {
        delete newInstance;
        return nullptr;
    }
    return newInstance;
}


bool Mesh::RayToMeshCollision(const Vector3& rayPosition, const Vector3& rayDirection, Vector3& outHitPoint)
{
    bool hit = false;
    float nearestDist = FLT_MAX;
    DirectX::XMVECTOR vertex0, vertex1, vertex2;
    DirectX::XMVECTOR rayOrigin = DirectX::XMLoadFloat3(&rayPosition);
    DirectX::XMVECTOR rayxmDirection = DirectX::XMLoadFloat3(&rayDirection);

    float distance;
    for (size_t i = 0; i < savedIndices.size(); i += 3)
    {
        vertex0 = DirectX::XMLoadFloat3(&savedVertices[savedIndices[i]].position);
        vertex1 = DirectX::XMLoadFloat3(&savedVertices[savedIndices[i + 1]].position);
        vertex2 = DirectX::XMLoadFloat3(&savedVertices[savedIndices[i + 2]].position);


        if (DirectX::TriangleTests::Intersects(rayOrigin, rayxmDirection, vertex0, vertex1, vertex2, distance))
        {
            if (distance < nearestDist)
            {
                nearestDist = distance;
                outHitPoint = rayPosition + rayDirection * distance;
                hit = true;
            }
        }
    }
    return hit;
}

HRESULT Mesh::LoadMesh(ModelConverter::FullModelMesh& subMesh, bool saveVertex, bool staticModel)
{
    std::vector<ModelVertex> vertices;
    size_t indicesSize = subMesh.index.size();
    std::vector<UINT> indices(indicesSize, 0);
    vertices.reserve(subMesh.vertex.size());

    ConvertVertexData(vertices, subMesh.vertex, staticModel);
    memcpy(indices.data(), subMesh.index.data(), sizeof(UINT) * indicesSize);
    memcpy(&materialIndex, &subMesh.materialIndex, sizeof(UINT));

    if (saveVertex)
    {
        savedVertices.resize(vertices.size());
        savedIndices.resize(indices.size());

        memcpy(savedVertices.data(), vertices.data(), sizeof(ModelVertex) * vertices.size());
        memcpy(savedIndices.data(), indices.data(), sizeof(UINT) * indices.size());
    }

    offset = 0;
    stride = sizeof(ModelVertex);
    indexCount = static_cast<UINT>(indicesSize);
    return CreateBuffer(dxDevice, vertexBuffer, indexBuffer, vertices, indices);
}

HRESULT Mesh::ConvertVertexData(std::vector<ModelVertex>& vertices, std::vector<ModelConverter::FullModelVertex>& otherVertices, bool staticModel)
{
    ModelVertex vertex;

    if (staticModel)
    {
        for (auto& otherVertice : otherVertices)
        {
            memcpy(&vertex.position, &otherVertice.position, sizeof(DirectX::XMFLOAT3));
            memcpy(&vertex.normal, &otherVertice.normal, sizeof(DirectX::XMFLOAT3));
            memcpy(&vertex.texcoord, &otherVertice.texcoord, sizeof(DirectX::XMFLOAT2));
            //memcpy(&vertex.texcoord, &otherVertice.texcoord, sizeof(DirectX::XMFLOAT2) * 8);
            //memcpy(&vertex.tangent, &otherVertice.tangent, sizeof(DirectX::XMFLOAT3));
            //memcpy(&vertex.bitTangent, &otherVertice.bitTangent, sizeof(DirectX::XMFLOAT3));
            ZeroMemory(&vertex.boneIDs, sizeof(UINT) * 4);
            ZeroMemory(&vertex.weights, sizeof(FLOAT) * 4);
            vertex.weights[0] = 1.0f;

            vertices.push_back(vertex);
        }
    }
    else
    {
        for (auto& otherVertice : otherVertices)
        {
            memcpy(&vertex.position, &otherVertice.position, sizeof(DirectX::XMFLOAT3));
            memcpy(&vertex.normal, &otherVertice.normal, sizeof(DirectX::XMFLOAT3));
            memcpy(&vertex.texcoord, &otherVertice.texcoord, sizeof(DirectX::XMFLOAT2));
            //memcpy(&vertex.texcoord, &otherVertice.texcoord, sizeof(DirectX::XMFLOAT2) * 8);
            //memcpy(&vertex.tangent, &otherVertice.tangent, sizeof(DirectX::XMFLOAT3));
            //memcpy(&vertex.bitTangent, &otherVertice.bitTangent, sizeof(DirectX::XMFLOAT3));
            memcpy(&vertex.boneIDs, &otherVertice.boneIDs, sizeof(UINT) * 4);
            memcpy(&vertex.weights, &otherVertice.weights, sizeof(FLOAT) * 4);

            vertices.push_back(vertex);
        }
    }


    return S_OK;
}

HRESULT Mesh::CreateBuffer(ID3D11Device* dxDevice, ID3D11Buffer*& vertexBuffer, ID3D11Buffer*& indexBuffer, std::vector<ModelVertex>& vertices, std::vector<UINT>& indices)
{
    // 버텍스 버퍼 생성
    D3D11_BUFFER_DESC vertexBufferDesc = {};
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = (UINT)(sizeof(ModelVertex) * vertices.size());
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA vertexData = {};
    ZeroMemory(&vertexData, sizeof(vertexData));
    vertexData.pSysMem = vertices.data();

    if (FAILED(dxDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer)))
        return E_FAIL;

    // 인덱스 버퍼 생성
    D3D11_BUFFER_DESC indexBufferDesc = {};
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = (UINT)(sizeof(UINT) * indices.size());
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA indexData = {};
    ZeroMemory(&indexData, sizeof(indexData));
    indexData.pSysMem = indices.data();

    if (FAILED(dxDevice->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer)))
        return E_FAIL;

    return S_OK;
}

void Mesh::Render(void)
{
    ::Render(indexBuffer, vertexBuffer, stride, offset, DXGI_FORMAT_R32_UINT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    dxDeviceContext->DrawIndexed(indexCount, 0, 0);
}


#ifdef useAssimp
Mesh* Mesh::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, aiMesh* mesh)
{
    std::map<std::string, Bone*> dummy;
    Mesh* newInstance = new Mesh(dxDevice, dxDeviceContext);
    if (FAILED(newInstance->LoadMesh(mesh, dummy)))
    {
        delete newInstance;
        return nullptr;
    }
    return newInstance;
}

Mesh* Mesh::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, aiMesh* mesh, std::map<std::string, Bone*>& bones)
{
    Mesh* newInstance = new Mesh(dxDevice, dxDeviceContext);
    if (FAILED(newInstance->LoadMesh(mesh, bones)))
    {
        delete newInstance;
        return nullptr;
    }
    return newInstance;
}

HRESULT Mesh::LoadMesh(aiMesh* mesh, std::map<std::string, Bone*>& bones)
{
    if (mesh == nullptr)
        return E_FAIL;

    std::vector<ModelVertex> vertices;
    std::vector<UINT> indices;
    LoadMeshData(mesh, vertices, indices, bones);
    offset = 0;
    stride = sizeof(ModelVertex);
    materialIndex = mesh->mMaterialIndex;
    indexCount = static_cast<UINT>(indices.size());
    return CreateBuffer(dxDevice, vertexBuffer, indexBuffer, vertices, indices);
}

HRESULT Mesh::LoadMeshData(aiMesh* mesh, std::vector<ModelVertex>& vertices, std::vector<UINT>& indices, std::map<std::string, Bone*>& bones)
{
    int boneID = 0;
    aiBone* bone = nullptr;
    ModelVertex vertex;
    aiMatrix4x4 offsetMatrix;
    aiFace face;
    aiVertexWeight weight;
    UINT i = 0, boneIdx = 0, weightIdx = 0;
    size_t j = 0;
    std::map<std::string, Bone*>::iterator boneIterator;
    vertices.clear();
    indices.clear();
    bool safety = false;
    float weightValue = 0.0f;

    std::vector<std::vector<std::pair<int, float>>> vertexBoneData(mesh->mNumVertices);

    // 본 데이터 가져오기
    for (boneIdx = 0; boneIdx < mesh->mNumBones; boneIdx++)
    {
        bone = mesh->mBones[boneIdx];

        boneIterator = bones.find(bone->mName.C_Str());
        boneID = boneIterator->second->Index();  // 본 ID 할당

        // 본이 영향을 주는 버텍스 정보
        for (weightIdx = 0; weightIdx < bone->mNumWeights; weightIdx++)
        {
            weight = bone->mWeights[weightIdx];
            for (auto& boneInfo : vertexBoneData[bone->mWeights[weightIdx].mVertexId])
            {
                if (boneInfo.first == boneID)
                {
                    safety = true;
                    break;
                }
                weightValue += static_cast<float>(boneInfo.second);
            }

            if(weightValue == 1.0f)
                safety = true;

            weightValue = 0.0f;

            if (!safety)
                vertexBoneData[bone->mWeights[weightIdx].mVertexId].push_back({ boneID,  bone->mWeights[weightIdx].mWeight });
            else
                safety = false;
        }
    }

    for (i = 0; i < mesh->mNumVertices; i++)
    {
        vertex.weights[0] = 1.0f;
        vertex.position.x = mesh->mVertices[i].x;
        vertex.position.y = mesh->mVertices[i].y;
        vertex.position.z = mesh->mVertices[i].z;

        if (mesh->HasNormals())
        {
            vertex.normal.x = mesh->mNormals[i].x;
            vertex.normal.y = mesh->mNormals[i].y;
            vertex.normal.z = mesh->mNormals[i].z;
        }

        //if (mesh->HasTangentsAndBitangents())
        //{
        //    memcpy(&vertex.tangent, &mesh->mTangents[i], static_cast<size_t>(sizeof(FLOAT)) * 3);
        //    memcpy(&vertex.bitTangent, &mesh->mBitangents[i], static_cast<size_t>(sizeof(FLOAT)) * 3);
        //}
        if (mesh->HasTextureCoords(0))
        {
            vertex.texcoord.x = mesh->mTextureCoords[0][i].x;
            vertex.texcoord.y = mesh->mTextureCoords[0][i].y;
        }
        //for (size_t i = 0; i < 8; i++)
        //{
        //    if (mesh->HasTextureCoords(i))
        //    {
        //        memcpy(&vertex.texcoord[i], &mesh->mTextureCoords[i][i], static_cast<size_t>(sizeof(FLOAT)) * 2);
        //    }
        //}


        vertices.push_back(vertex);
    }

    // 정점에 본 데이터 적용 (최대 4개까지만 저장)
    for (i = 0; i < mesh->mNumVertices; i++)
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
    for (i = 0; i < mesh->mNumFaces; i++)
    {
        face = mesh->mFaces[i];
        for (j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }
    return S_OK;
}
#endif