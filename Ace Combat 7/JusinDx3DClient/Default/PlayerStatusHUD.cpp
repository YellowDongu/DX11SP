#include "pch.h"
#include "PlayerStatusHUD.h"

PlayerStatusHUD::PlayerStatusHUD(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::UIObject(dxDevice, dxDeviceContext), text(nullptr)
{
}

void PlayerStatusHUD::Free(void)
{
    Base::Destroy(text);
}

PlayerStatusHUD* PlayerStatusHUD::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, Engine::GameObject* player)
{
    PlayerStatusHUD* newInstance = new PlayerStatusHUD(dxDevice, dxDeviceContext);

    if (FAILED(newInstance->Start()))
    {
        Base::Destroy(newInstance);
        return nullptr;
    }

    return newInstance;
}

HRESULT PlayerStatusHUD::Start(void)
{
    //UIParts& part = compassLine;
    //
    //innerErrorCheck(compassLine.LoadUITexture(L"../Bin/Resources/UI/HUD/Compass_VerticalLine.png"), L"Load texture - Compass_VerticalLine.png");
    //CreateScale(compassLine.texture, scale);
    //innerErrorCheck(CreateVertex(compassLine.vertexBuffer, compassLine.indexBuffer, compassLine.indexCount, scale), L"Vertex/Index Create");

    text = CreateText(L"../Bin/ACES07.spritefont");
    if (text == nullptr)
        return E_FAIL;
    return S_OK;
}

HRESULT PlayerStatusHUD::Awake(void)
{
    return E_NOTIMPL;
}

void PlayerStatusHUD::FixedUpdate(void)
{
}

void PlayerStatusHUD::Update(void)
{
}

void PlayerStatusHUD::LateUpdate(void)
{
    AddRenderObject(RenderType::UI, this);
}

void PlayerStatusHUD::Render(void)
{
    Vector2 position = { static_cast<FLOAT>(windowSizeX), static_cast<FLOAT>(windowSizeY) };
    float winY = static_cast<FLOAT>(windowSizeY);
    position.x *= 0.20f;
    position.y *= -0.35f;
    text->RenderText(L"GUN", position, float2{ 0.8f, 1.25f }, { 0.0f, 1.0f, 0.0f, 1.0f }, 0.0f);

    position.y += winY  * -0.05f;
    text->RenderText(L"MSL", position, float2{ 0.8f, 1.25f }, { 0.0f, 1.0f, 0.0f, 1.0f }, 0.0f);

    position.y += winY * -0.035f;
    text->RenderText(L"SPCL", position, float2{ 0.8f, 1.25f }, { 0.0f, 1.0f, 0.0f, 1.0f }, 0.0f);

    position.y += winY * -0.05f;
    text->RenderText(L"FLR", position, float2{ 0.8f, 1.25f }, { 0.0f, 1.0f, 0.0f, 1.0f }, 0.0f);

    position.y += winY * -0.05f;
    text->RenderText(L"DMG", position, float2{ 0.8f, 1.25f }, { 0.0f, 1.0f, 0.0f, 1.0f }, 0.0f);
}














































/*

#include "MeshTest.h"
#include "InputManager.h"

using namespace DirectX;


MeshTest::MeshTest(ID3D11Device* _device, ID3D11DeviceContext* _context) : device(_device), context(_context), mesh(), samplerState(nullptr), vertexShader(nullptr), inputLayout(nullptr), pixelShader(nullptr), blendState(nullptr), g_pRasterizerState(nullptr), materialBuffer(nullptr)
{
}


MeshTest* MeshTest::Create(ID3D11Device* device, ID3D11DeviceContext* context, const std::string& filePath)
{
    MeshTest* newInstance = new MeshTest(device, context);

    //LoadModelAndCreateBoneMatrices(filePath);
    if (FAILED(newInstance->LoadMesh(filePath)))
    {
        delete newInstance;
        return nullptr;
    }

    return newInstance;
}

HRESULT MeshTest::LoadMesh(const std::string& filePath)
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


    //std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayout = {
    //   { "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
    //   { "NORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
    //   { "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT,      0, 24,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
    //   { "BONES",     0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 32,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
    //   { "WEIGHTS",   0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 48,  D3D11_INPUT_PER_VERTEX_DATA, 0 }
    //};
    std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayout = AdvencedVertex::inputLayout;
    //D3D11_INPUT_ELEMENT_DESC* inputDesc = inputLayout.data();

    //shader = Engine::Shader::Create(device, context, L"ModelVSShader.hlsl", &inputDesc, (UINT)inputLayout.size());
    shader = LoadShader(L"ModelVSShader.hlsl", inputLayout);
    SetShader(L"ModelVSShader.hlsl");

    return LoadMesh(device, context, filePath, mesh);
}

void MeshTest::SetMaterialInShader(ID3D11DeviceContext*& context, ID3D11Buffer*& materialBuffer, const Material& material)
{
    shader->SetShader();
    shader->ApplyShader();

    shader->BindTexture("diffuseTexture", material.diffuseTexture);
    //context->PSSetShaderResources(0, 1, &material.diffuseTexture);
    //context->PSSetShaderResources(1, 1, &material.specularTexture);
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

        context->PSSetConstantBuffers(1, 1, &materialBuffer);
    }
}

void MeshTest::UpdateBones(ID3D11DeviceContext*& context, ID3D11Buffer*& pBoneCBuffer, std::map<int, Node*>& nodes)
{
    std::vector<DirectX::XMFLOAT4X4> animationMatrix;
    std::vector<DirectX::XMFLOAT4X4> offsetMatrix;
    std::vector<DirectX::XMFLOAT4X4> transformMatrix;
    //DirectX::XMFLOAT4X4 tempStorage;
    for (int i = 0; i < nodes.size(); i++)
    {
        Node* node = (*nodes.find(i)).second;
        animationMatrix.push_back(node->finalMatrix);
        //animationMatrix.push_back(node->currentMatrix);
        //animationMatrix.push_back(tempStorage);
        offsetMatrix.push_back(node->offsetMatrix);
        //transformMatrix.push_back(node->finalMatrix);
        transformMatrix.push_back(node->transformMatrix);
    }

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    if (SUCCEEDED(context->Map(pBoneCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
    {
        BoneBuffer* data = static_cast<BoneBuffer*>(mappedResource.pData);
        memcpy(data->boneMatrix, animationMatrix.data(), sizeof(DirectX::XMFLOAT4X4) * animationMatrix.size());
        context->Unmap(pBoneCBuffer, 0);

        shader->SetConstantBuffer("BoneBuffer", pBoneCBuffer);
        context->VSSetConstantBuffers(4, 1, &pBoneCBuffer);
    }
}

void MeshTest::UpdateBoneOrigin(ID3D11DeviceContext*& context, ID3D11Buffer*& pBoneCBuffer, std::map<int, Node*>& nodes)
{
    std::vector<DirectX::XMFLOAT4X4> animationMatrix;
    std::vector<DirectX::XMFLOAT4X4> offsetMatrix;
    std::vector<DirectX::XMFLOAT4X4> transformMatrix;
    DirectX::XMFLOAT4X4 tempStorage;
    for (int i = 0; i < nodes.size(); i++)
    {
        Node* node = (*nodes.find(i)).second;
        DirectX::XMStoreFloat4x4(&tempStorage, DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&node->currentMatrix)));
        animationMatrix.push_back(tempStorage);
        DirectX::XMStoreFloat4x4(&tempStorage, DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&node->offsetMatrix)));
        offsetMatrix.push_back(tempStorage);
        DirectX::XMStoreFloat4x4(&tempStorage, DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&node->transformMatrix)));
        transformMatrix.push_back(tempStorage);
    }

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    if (SUCCEEDED(context->Map(pBoneCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
    {
        BoneBuffer* data = static_cast<BoneBuffer*>(mappedResource.pData);
        memcpy(data->boneMatrix, animationMatrix.data(), sizeof(DirectX::XMFLOAT4X4) * animationMatrix.size());
        context->Unmap(pBoneCBuffer, 0);

        context->VSSetConstantBuffers(5, 1, &pBoneCBuffer);
    }
}

void MeshTest::Update(void)
{
}
int test = 0;
bool relative = true;
void MeshTest::Render(void)
{
    DirectX::XMFLOAT3 position, scale;
    DirectX::XMFLOAT4 quaternion;

    if (InputManager::GetInstance()->getButton(KeyType::Y))
        test++;
    if (InputManager::GetInstance()->getButton(KeyType::U))
        test = 0;
    for (auto& matrix : mesh.boneMapping)
    {
        DirectX::XMStoreFloat4x4(&matrix.second->currentMatrix, DirectX::XMMatrixIdentity());
    }
    if (test)
    {
        for (auto& item : mesh.animations)
        {
            for (auto& node : item.second.nodes)
            {
                auto boneIterator = mesh.boneMapping.find(node.boneName);
                if (boneIterator == mesh.boneMapping.end())
                    continue;

                if (test >= node.positions.size() && test >= node.scales.size() && test >= node.quaternionRotations.size())
                    test = 0;

                GetAnimationStates(node, test, position, quaternion, scale);

                if (relative)
                {
                    GetRelativeState(node, test, boneIterator->second->transformMatrix, boneIterator->second->currentMatrix);

                }
                else
                {
                    DirectX::XMMATRIX worldMatrix =  DirectX::XMMatrixAffineTransformation(DirectX::XMLoadFloat3(&scale), XMVectorSet(0.f, 0.f, 0.f, 1.f), DirectX::XMLoadFloat4(&quaternion), DirectX::XMLoadFloat3(&position));
                    //worldMatrix = worldMatrix * DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&boneIterator->second->transformMatrix));
                    DirectX::XMStoreFloat4x4(&boneIterator->second->currentMatrix, worldMatrix);
                }
            }

        }

    }

    if(relative)
        SetBoneMatrixInRelative(mesh.rootNode);
    else
        SetBoneMatrix(mesh.rootNode);

    std::stack<Node*> nodes;
    std::list<Node*> parents;
    Node* parentNode;
    nodes.push(&mesh.rootNode);

    DirectX::XMFLOAT3 viewPosition, viewScale;
    DirectX::XMFLOAT4 viewQuaternion;

    while (!nodes.empty())
    {
        Node* node = nodes.top();
        nodes.pop();

        DecomposeMatrix(node->finalMatrix, viewPosition, viewQuaternion, viewScale);

        parentNode = node->parent;
        for (auto& pNode : node->mChildren)
        {
            nodes.push(&pNode);
        }
    }




    //std::stack<Node*> nodes;
    //std::list<Node*> parents;
    //Node* parentNode;
    //nodes.push(&mesh.rootNode);
    //DirectX::XMMATRIX matrix;
    //while (!nodes.empty())
    //{
    //    Node* node = nodes.top();
    //    nodes.pop();
    //    parentNode = node->parent;
    //    for (auto& pNode : node->mChildren)
    //    {
    //        nodes.push(&pNode);
    //    }
    //
    //    parents.clear();
    //    while (parentNode)
    //    {
    //        parents.push_front(parentNode);
    //        parentNode = parentNode->parent;
    //    }
    //    matrix = DirectX::XMMatrixIdentity();
    //
    //    if (relative)
    //    {
    //        for (auto& pNode : parents)
    //        {
    //            matrix *= DirectX::XMLoadFloat4x4(&pNode->transformMatrix);
    //            matrix *= DirectX::XMLoadFloat4x4(&pNode->currentMatrix);
    //        }
    //        //matrix = DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&node->currentMatrix), matrix);
    //
    //        matrix = DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&node->offsetMatrix), DirectX::XMMatrixMultiply(DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&node->transformMatrix), DirectX::XMLoadFloat4x4(&node->currentMatrix)), matrix));
    //        //matrix = DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&node->currentMatrix), matrix);
    //        DirectX::XMStoreFloat4x4(&node->finalMatrix, matrix);
    //    }
    //    else
    //    {
    //        for (auto& pNode : parents)
    //        {
    //            matrix = DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&pNode->currentMatrix), matrix);
    //            //matrix *= DirectX::XMLoadFloat4x4(&pNode->currentMatrix);
    //        }
    //        matrix = DirectX::XMLoadFloat4x4(&node->currentMatrix) * matrix;
    //        DirectX::XMStoreFloat4x4(&node->finalMatrix, XMMatrixMultiply(DirectX::XMLoadFloat4x4(&node->offsetMatrix), matrix));
    //    }
    //}

    //BoneMatrix(mesh, test);

    context->IASetInputLayout(inputLayout);
    context->VSSetShader(vertexShader, nullptr, 0);
    context->PSSetShader(pixelShader, nullptr, 0);
    context->PSSetSamplers(0, 1, &samplerState);
    context->RSSetState(g_pRasterizerState);
    context->OMSetBlendState(blendState, NULL, 0xffffffff);



    UpdateBones(context, boneBuffer, mesh.boneIndex);

    for (auto& item : mesh.subMesh)
    {
        SetMaterialInShader(context, materialBuffer, mesh.materials[item.materialIndex]);
        Render(context, item);
        shader->Render(item.indexBuffer, item.vertexBuffer, sizeof(AdvencedVertex), 0, DXGI_FORMAT_R32_UINT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        context->DrawIndexed((UINT)item.indices.size(), 0, 0);
    }
}

void MeshTest::Render(ID3D11DeviceContext*& context, SubMesh& subMesh)
{
    shader->Render(subMesh.indexBuffer, subMesh.vertexBuffer, sizeof(AdvencedVertex), 0, DXGI_FORMAT_R32_UINT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    context->DrawIndexed((UINT)subMesh.indices.size(), 0, 0);
}


HRESULT MeshTest::CreateBuffers(ID3D11Device*& device, ID3D11Buffer*& vertexBuffer, ID3D11Buffer*& indexBuffer, std::vector<AdvencedVertex>& vertices, std::vector<UINT>& indices)
{
    // ���ؽ� ���� ����
    D3D11_BUFFER_DESC vertexBufferDesc = {};
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = (UINT)(sizeof(AdvencedVertex) * vertices.size());
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA vertexData = {};
    ZeroMemory(&vertexData, sizeof(vertexData));
    vertexData.pSysMem = vertices.data();

    if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer)))
        return E_FAIL;

    // �ε��� ���� ����
    D3D11_BUFFER_DESC indexBufferDesc = {};
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = (UINT)(sizeof(UINT) * indices.size());
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA indexData = {};
    ZeroMemory(&indexData, sizeof(indexData));
    indexData.pSysMem = indices.data();

    if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer)))
        return E_FAIL;

    return S_OK;
}

void MeshTest::LoadMaterialAndTextures(ID3D11Device*& device, ID3D11DeviceContext*& context, const aiScene*& scene, const std::string& currentPath, std::vector<Material>& materials)
{
    aiMaterial* aiMaterial = nullptr;
    Material material;
    aiString texturePath;
    for (UINT i = 0; i < scene->mNumMaterials; ++i)
    {
        aiMaterial = scene->mMaterials[i];
        ZeroMemory(&material, sizeof(Material));

        // Texture Test
        //for (int j = 0; j < aiTextureType_TRANSMISSION; j++)
        //{
        //    if (aiMaterial->GetTexture((aiTextureType)j, 0, &texturePath) == AI_SUCCESS)
        //    {
        //        int alarm = 0;
        //    }
        //
        //}

        // ��ǻ�� �ؽ�ó �ε�
        if (aiMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == AI_SUCCESS)
        {
            std::wstring texPath = std::wstring(texturePath.C_Str(), texturePath.C_Str() + strlen(texturePath.C_Str()));
            std::wstring fullPath = getAbsolutePathFromFile(To_WString(currentPath), texPath);
            if (FAILED(DirectX::CreateDDSTextureFromFile(device, context, fullPath.c_str(), nullptr, &material.diffuseTexture)))
            {
                //MessageBoxW(0, );
                int alarm = 0;
            }

            //std::wstring texPath = std::wstring(texturePath.C_Str(), texturePath.C_Str() + strlen(texturePath.C_Str()));
            //if (FAILED(DirectX::CreateWICTextureFromFile(device, context, texPath.c_str(), nullptr, &material.diffuseTexture)))
            //{
            //    //MessageBoxW(0, );
            //    int alarm = 0;
            //}
        }

        // ��� �ؽ�ó �ε�
        // AC7
        //if (aiMaterial->GetTexture(aiTextureType_NORMALS, 0, &texturePath) == AI_SUCCESS)
        //{
        //    std::wstring texPath = std::wstring(texturePath.C_Str(), texturePath.C_Str() + strlen(texturePath.C_Str()));
        //    std::wstring fullPath = getAbsolutePathFromFile(To_WString(currentPath), texPath);
        //    if (FAILED(DirectX::CreateDDSTextureFromFile(device, context, fullPath.c_str(), nullptr, &material.specularTexture)))
        //    {
        //        //MessageBoxW(0, );
        //        int alarm = 0;
        //    }
        //}

        // �߰� �Ӽ� (��ǻ�� ����, ����ŧ�� ���� ��) �ε�
        aiColor4D diffuseColor;
        aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
        material.diffuseColor = DirectX::XMFLOAT4(diffuseColor.r, diffuseColor.g, diffuseColor.b, diffuseColor.a);

        aiColor4D specColor;
        aiMaterial->Get(AI_MATKEY_COLOR_SPECULAR, specColor);
        material.specularColor = DirectX::XMFLOAT4(specColor.r, specColor.g, specColor.b, specColor.a);

        ai_real shininess;
        aiMaterial->Get(AI_MATKEY_SHININESS, shininess);
        material.shininess = shininess;

        materials.push_back(material);
    }
}
bool MeshTest::LoadBones(const aiScene*& scene, Node& rootNode, std::unordered_map<std::string, Node*>& boneMapping, std::vector<DirectX::XMFLOAT4X4>& boneMatrix, std::map<int, Node*>& boneIndex)
{
    aiAnimation* animation = nullptr;
    aiNodeAnim* nodeAnim = nullptr;
    DirectX::XMFLOAT4X4 identityMatrix;
    DirectX::XMStoreFloat4x4(&identityMatrix, DirectX::XMMatrixIdentity());
    GetNode(scene->mRootNode, rootNode, boneMapping);
    if (scene->HasAnimations())
    {
        std::string boneName;
        std::unordered_map<std::string, Node*>::iterator iterator;
        UINT totalCount = 0;
        UINT boneIdx = 0;
        aiMesh* mesh = nullptr;
        aiBone* aiBone = nullptr;
        for (UINT i = 0; i < scene->mNumMeshes; i++)
        {
            mesh = scene->mMeshes[i];
            // �� ������ ��������
            for (boneIdx = 0; boneIdx < mesh->mNumBones; boneIdx++)
            {
                aiBone = mesh->mBones[boneIdx];
                std::string boneName = aiBone->mName.C_Str();
                iterator = boneMapping.find(boneName);
                if (iterator == boneMapping.end())
                    continue;

                boneIndex[totalCount] = (*iterator).second;
                (*iterator).second->offsetMatrix = DirectX::XMFLOAT4X4(
                    aiBone->mOffsetMatrix.a1, aiBone->mOffsetMatrix.a2, aiBone->mOffsetMatrix.a3, aiBone->mOffsetMatrix.a4,
                    aiBone->mOffsetMatrix.b1, aiBone->mOffsetMatrix.b2, aiBone->mOffsetMatrix.b3, aiBone->mOffsetMatrix.b4,
                    aiBone->mOffsetMatrix.c1, aiBone->mOffsetMatrix.c2, aiBone->mOffsetMatrix.c3, aiBone->mOffsetMatrix.c4,
                    aiBone->mOffsetMatrix.d1, aiBone->mOffsetMatrix.d2, aiBone->mOffsetMatrix.d3, aiBone->mOffsetMatrix.d4);
                DirectX::XMStoreFloat4x4(&(*iterator).second->offsetMatrix, DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&(*iterator).second->offsetMatrix)));

                totalCount++;
            }

        }

        return true;
    }
    return false;
}

void MeshTest::LoadAnimation(const aiScene* scene, std::map<std::string, AnimationInfo>& animations)
{
    AnimationInfo animationInfo;
    AnimationNode animationNode;
    aiAnimation* animation = nullptr;
    aiNodeAnim* channel = nullptr;

    for (UINT animIndex = 0; animIndex < scene->mNumAnimations; ++animIndex)
    {
        animationInfo.Clear();
        animation = scene->mAnimations[animIndex];
        animationInfo.name = animation->mName.C_Str();
        animationInfo.duration = (float)animation->mDuration;
        animationInfo.ticksPerSecond = (float)animation->mTicksPerSecond;


        for (UINT channelIndex = 0; channelIndex < animation->mNumChannels; ++channelIndex)
        {
            animationNode.Clear();
            channel = animation->mChannels[channelIndex];
            animationNode.boneName = channel->mNodeName.C_Str();

            // ��ġ(Position) Ű������
            for (UINT key = 0; key < channel->mNumPositionKeys; ++key)
            {
                animationNode.positions.push_back({ (float)channel->mPositionKeys[key].mTime, { (float)channel->mPositionKeys[key].mValue.x, (float)channel->mPositionKeys[key].mValue.y, (float)channel->mPositionKeys[key].mValue.z } });
            }

            // ȸ��(Rotation) Ű������
            for (UINT key = 0; key < channel->mNumRotationKeys; ++key)
            {
                animationNode.quaternionRotations.push_back({ (float)channel->mRotationKeys[key].mTime , { (float)channel->mRotationKeys[key].mValue.x, (float)channel->mRotationKeys[key].mValue.y, (float)channel->mRotationKeys[key].mValue.z, (float)channel->mRotationKeys[key].mValue.w } });
            }

            // ������(Scaling) Ű������
            for (UINT key = 0; key < channel->mNumScalingKeys; ++key)
            {
                animationNode.scales.push_back({ (float)channel->mScalingKeys[key].mTime , { (float)channel->mScalingKeys[key].mValue.x, (float)channel->mScalingKeys[key].mValue.y, (float)channel->mScalingKeys[key].mValue.z } });
            }
            animationInfo.nodes.push_back(animationNode);
        }
        animations[animationInfo.name] = animationInfo;
    }
}



void MeshTest::Free(void)
{
    materialBuffer->Release();

    samplerState->Release();
    vertexShader->Release();
    inputLayout->Release();
    pixelShader->Release();
    blendState->Release();
    g_pRasterizerState->Release();
}

void MeshTest::GetMeshData(aiMesh*& mesh, std::vector<AdvencedVertex>& vertices, std::vector<UINT>& indices)
{
    int boneID;
    aiBone* bone = nullptr;
    AdvencedVertex vertex;
    aiMatrix4x4 offsetMatrix;
    UINT i = 0;
    size_t j = 0;
    UINT boneIdx = 0;
    UINT weightIdx = 0;

    vertices.clear();
    indices.clear();

    std::vector<std::vector<std::pair<int, float>>> vertexBoneData(mesh->mNumVertices);

    // �� ������ ��������
    for (boneIdx = 0; boneIdx < mesh->mNumBones; boneIdx++)
    {
        bone = mesh->mBones[boneIdx];
        boneID = boneIdx;  // �� ID �Ҵ� (���� �� �̸��� ������� ID�� ������)

        // ���� ������ ��� ���� (�ִϸ��̼� ���� �� ���)
        offsetMatrix = bone->mOffsetMatrix;

        // ���� ������ �ִ� ���ؽ� ����
        for (weightIdx = 0; weightIdx < bone->mNumWeights; weightIdx++)
        {
            vertexBoneData[bone->mWeights[weightIdx].mVertexId].push_back({ boneID,  bone->mWeights[weightIdx].mWeight });
        }
    }

    for (i = 0; i < mesh->mNumVertices; i++)
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

    // ������ �� ������ ���� (�ִ� 4�������� ����)
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

    // �ε��� ������ ����
    aiFace face;
    for (i = 0; i < mesh->mNumFaces; i++)
    {
        face = mesh->mFaces[i];
        for (j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

}


HRESULT MeshTest::LoadSubMesh(ID3D11Device*& device, ID3D11DeviceContext*& context, aiMesh*& mesh, MeshData& meshContainer)
{
    if (!mesh)
        return E_FAIL;
    SubMesh temp;
    meshContainer.subMesh.push_back(temp);
    SubMesh& newSubMesh = meshContainer.subMesh.back();

    newSubMesh.materialIndex = mesh->mMaterialIndex;
    GetMeshData(mesh, newSubMesh.vertices, newSubMesh.indices);
    if (FAILED(CreateBuffers(device, newSubMesh.vertexBuffer, newSubMesh.indexBuffer, newSubMesh.vertices, newSubMesh.indices)))
    {
        meshContainer.subMesh.pop_back();
        return E_FAIL;
    }


    return S_OK;
}

HRESULT MeshTest::LoadMesh(ID3D11Device*& device, ID3D11DeviceContext*& context, const std::string& filePath, MeshData& meshContainer)
{
    Assimp::Importer importer;
    aiMesh* mesh = nullptr;

    //UINT flags = aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace |
    //    aiProcess_LimitBoneWeights | aiProcess_GenSmoothNormals | aiProcess_ValidateDataStructure | aiProcess_PreTransformVertices;
    //UINT flags = aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_GenNormals | aiProcess_FlipUVs;
    //UINT flags = aiProcess_ConvertToLeftHanded | aiProcess_Triangulate | aiProcess_GenNormals;
    UINT flags = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast | aiProcess_LimitBoneWeights;
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
        mesh = scene->mMeshes[i];
        LoadSubMesh(device, context, mesh, meshContainer);
    }

    LoadMaterialAndTextures(device, context, scene, filePath, meshContainer.materials);
    LoadBones(scene, meshContainer.rootNode, meshContainer.boneMapping, meshContainer.boneMatrix, meshContainer.boneIndex);
    LoadAnimation(scene, meshContainer.animations);
    if (scene)
        importer.FreeScene();

    return S_OK;
}




HRESULT MeshTest::CreateBoneBuffer(ID3D11Device*& device, ID3D11Buffer*& pBoneCBuffer)
{
    D3D11_BUFFER_DESC cbDesc = {};
    cbDesc.Usage = D3D11_USAGE_DYNAMIC;            // �������� ������Ʈ ����
    cbDesc.ByteWidth = sizeof(BoneBuffer);         // ���� ũ��
    cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // ��� ���۷� ���
    cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // CPU�� ���� ����
    cbDesc.MiscFlags = 0;

    return device->CreateBuffer(&cbDesc, nullptr, &pBoneCBuffer);
}

HRESULT MeshTest::CreateMaterialBuffer(ID3D11Device*& device, ID3D11Buffer*& materialBuffer)
{
    D3D11_BUFFER_DESC materialBufferDesc = {};
    materialBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    materialBufferDesc.ByteWidth = sizeof(MaterialBufferType);
    materialBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    materialBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    return device->CreateBuffer(&materialBufferDesc, NULL, &materialBuffer);

}

HRESULT MeshTest::CompileVertexShader(const std::wstring& filePath, ID3DBlob*& vertexShaderBlob, ID3DBlob*& errorBlob)
{
    HRESULT hr = D3DCompileFromFile(
        filePath.c_str(),    // ���̴� ���� ���
        nullptr, nullptr,   // ��ũ�� �� ���� ���� ����
        "VS_Main",          // ��Ʈ�� ����Ʈ �Լ� (Vertex Shader)
        "vs_5_0",           // ���̴� �� (Vertex Shader 5.0)
        D3DCOMPILE_ENABLE_STRICTNESS,  // ������ �ɼ�
        0,                  // ��Ÿ �ɼ� ����
        &vertexShaderBlob,  // ��� Blob (���̴� �ڵ�)
        &errorBlob          // ���� �޽���
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
HRESULT MeshTest::CompileShader(const std::wstring& filePath, const std::string& entryMethod, const std::string& shaderModel, ID3DBlob*& ShaderBlob, ID3DBlob*& errorBlob)
{
    HRESULT hr = D3DCompileFromFile(
        filePath.c_str(),    // ���̴� ���� ���
        nullptr, nullptr,   // ��ũ�� �� ���� ���� ����
        entryMethod.c_str(),          // ��Ʈ�� ����Ʈ �Լ� (Vertex Shader)
        shaderModel.c_str(),           // ���̴� �� (Vertex Shader 5.0)
        D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_SKIP_OPTIMIZATION,  // ������ �ɼ�
        0,                  // ��Ÿ �ɼ� ����
        &ShaderBlob,  // ��� Blob (���̴� �ڵ�)
        &errorBlob          // ���� �޽���
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

HRESULT MeshTest::LoadVertexShader(ID3D11Device*& device, ID3DBlob*& vertexShaderBlob, ID3D11VertexShader*& vertexShader)
{
    return device->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), nullptr, &vertexShader);
}

HRESULT MeshTest::InputLayoutSetting(ID3D11Device*& device, ID3DBlob*& vertexShaderBlob, ID3D11InputLayout*& inputLayout)
{
    D3D11_INPUT_ELEMENT_DESC layout[] = {
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
    UINT numElements = ARRAYSIZE(layout);

    return device->CreateInputLayout(layout, numElements, vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), &inputLayout);
}
HRESULT MeshTest::BonedInputLayoutSetting(ID3D11Device*& device, ID3DBlob*& vertexShaderBlob, ID3D11InputLayout*& inputLayout)
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


HRESULT MeshTest::LoadPixelShader(ID3D11Device*& device, ID3DBlob*& pixelShaderBlob, ID3D11PixelShader*& pixelShader)
{
    return device->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), nullptr, &pixelShader);
}
HRESULT MeshTest::CreateSampler(ID3D11Device*& pDevice, ID3D11SamplerState*& g_pSamplerState)
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

HRESULT MeshTest::LoadVertexShader(ID3D11Device*& device, const std::wstring& filePath, ID3D11VertexShader*& vertexShader, ID3D11InputLayout*& inputLayout)
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
HRESULT MeshTest::LoadPixelShader(ID3D11Device*& device, const std::wstring& filePath, ID3D11PixelShader*& pixelShader, ID3D11SamplerState*& samplerState)
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

HRESULT MeshTest::CreateRasterizerState(ID3D11Device*& device, ID3D11RasterizerState*& rasterizerState)
{
    D3D11_RASTERIZER_DESC rasterDesc = {};
    rasterDesc.FillMode = D3D11_FILL_SOLID; // �⺻ �� (���̾������� ���: D3D11_FILL_WIREFRAME)
    rasterDesc.CullMode = D3D11_CULL_NONE;  // �ø� ���� (DirectX 9�� D3DCULL_NONE�� ����)
    rasterDesc.FrontCounterClockwise = FALSE; // �⺻�� (�ݽð� ������ �ո�)

    return device->CreateRasterizerState(&rasterDesc, &rasterizerState);
}


std::wstring MeshTest::getAbsolutePathFromFile(const std::wstring& baseFilePath, const std::wstring& relativePath)
{
    wchar_t full_path[MAX_PATH];

    std::wstring baseDir = baseFilePath;
    size_t pos = baseDir.find_last_of(L"\\/");
    if (pos != std::wstring::npos)
    {
        baseDir = baseDir.substr(0, pos);
    }

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


HRESULT MeshTest::DisableNonBlend(ID3D11Device*& device, ID3D11BlendState*& blendState)
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


float MeshTest::GetFactor(double lastTime, double nextTime, double currentTime)
{
    return (float)((currentTime - lastTime) / (nextTime - lastTime));
}
DirectX::XMFLOAT3 MeshTest::Lerp(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, float factor)
{
    DirectX::XMFLOAT3 result;
    DirectX::XMStoreFloat3(&result, DirectX::XMVectorLerp(DirectX::XMLoadFloat3(&start), DirectX::XMLoadFloat3(&end), factor));
    return result;
}
DirectX::XMFLOAT4 MeshTest::SLerp(const DirectX::XMFLOAT4& start, const DirectX::XMFLOAT4& end, float factor)
{
    DirectX::XMFLOAT4 result;
    DirectX::XMStoreFloat4(&result, DirectX::XMQuaternionSlerp(DirectX::XMLoadFloat4(&start), DirectX::XMLoadFloat4(&end), factor));
    return result;
}
DirectX::XMFLOAT3 MeshTest::Interpolate(std::vector<std::pair<float, DirectX::XMFLOAT3>>& list, float currentTime)
{
    if (list.size() == 1)
    {
        return list[0].second;
    }

    float factor;
    for (size_t i = 0; i < list.size() - 1; i++)
    {
        if (currentTime < list[i + 1].first)
        {
            if (currentTime == list[i].first)
                return list[i].second;

            factor = GetFactor(list[i].first, list[i + 1].first, currentTime);
            return Lerp(list[i].second, list[i + 1].second, factor);
        }
    }

    return list.back().second;
}
DirectX::XMFLOAT4 MeshTest::Interpolate(std::vector<std::pair<float, DirectX::XMFLOAT4>>& list, float currentTime)
{
    if (list.size() == 1)
    {
        return list.front().second;
    }

    float factor;
    for (size_t i = 0; i < list.size() - 1; i++)
    {
        if (currentTime < list[i + 1].first)
        {
            if (currentTime == list[i].first)
                return list[i].second;

            factor = GetFactor(list[i].first, list[i + 1].first, currentTime);
            return SLerp(list[i].second, list[i + 1].second, factor);
        }
    }

    return list.back().second;
}



void ConvertMatrix(DirectX::XMFLOAT4X4& out, const aiMatrix4x4& in)
{
    out = DirectX::XMFLOAT4X4(in.a1, in.a2, in.a3, in.a4, in.b1, in.b2, in.b3, in.b4, in.c1, in.c2, in.c3, in.c4, in.d1, in.d2, in.d3, in.d4);
}

// aiNode�� Node�� ��ȯ�ϴ� �Լ� (������ �̿�)
void GetNode(const aiNode* rootAiNode, Node& rootNode, std::unordered_map<std::string, Node*>& boneMapping)
{
    DirectX::XMFLOAT4X4 identityMatrix;
    DirectX::XMStoreFloat4x4(&identityMatrix, DirectX::XMMatrixIdentity());

    std::stack<std::pair<const aiNode*, Node*>> nodeStack;
    nodeStack.push({ rootAiNode, &rootNode });

    const aiNode* currentAiNode = nullptr;
    Node* currentNode = nullptr;
    while (!nodeStack.empty())
    {
        std::pair<const aiNode*, Node*> currentPair = nodeStack.top();
        nodeStack.pop();

        currentAiNode = currentPair.first;
        currentNode = currentPair.second;
        currentNode->currentMatrix = identityMatrix;
        currentNode->finalMatrix = identityMatrix;
        currentNode->offsetMatrix = identityMatrix;
        currentNode->transformMatrix = identityMatrix;

        currentNode->BoneName = currentAiNode->mName.C_Str();
        boneMapping[currentNode->BoneName] = currentNode;

        ConvertMatrix(currentNode->transformMatrix, currentAiNode->mTransformation);
        DirectX::XMStoreFloat4x4(&currentNode->transformMatrix, DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&currentNode->transformMatrix)));
        for (UINT i = 0; i < currentAiNode->mNumChildren; ++i)
        {
            currentNode->mChildren.push_back(Node());
            currentNode->mChildren[i].parent = currentNode;
        }

        for (UINT i = 0; i < currentAiNode->mNumChildren; ++i)
        {
            nodeStack.push({ currentAiNode->mChildren[i], &currentNode->mChildren[i] });
        }
    }
}


void DecomposeMatrix(const DirectX::XMFLOAT4X4& matrix, DirectX::XMFLOAT3& position, DirectX::XMFLOAT4& rotation, DirectX::XMFLOAT3& scale, bool test)
{
    // 4x4 ����� XMMATRIX�� ��ȯ
    //XMMATRIX matrix = XMLoadFloat4x4(&mat);

    // ��ġ ���� (4��° ���� ��ġ)
    position = DirectX::XMFLOAT3(matrix._41, matrix._42, matrix._43);

    // ũ�� ����
    scale = DirectX::XMFLOAT3(
        DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSet(matrix._11, matrix._12, matrix._13, 0.0f))),
        DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSet(matrix._21, matrix._22, matrix._23, 0.0f))),
        DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSet(matrix._31, matrix._32, matrix._33, 0.0f)))
    );

    // ȸ�� ����
    // ��ġ�� ��Ŀ��� ��ġ �κ��� 0���� ���� ��, ȸ�� �κи� ����
    DirectX::XMFLOAT4X4 rotationMatrix = matrix;
    rotationMatrix._41 = rotationMatrix._42 = rotationMatrix._43 = 0.0f; // ��ġ �κ��� 0���� ����

    if (test)
    {
        // Scale�� ����ȭ
        rotationMatrix._11 /= scale.x;
        rotationMatrix._12 /= scale.x;
        rotationMatrix._13 /= scale.x;

        rotationMatrix._21 /= scale.y;
        rotationMatrix._22 /= scale.y;
        rotationMatrix._23 /= scale.y;

        rotationMatrix._31 /= scale.z;
        rotationMatrix._32 /= scale.z;
        rotationMatrix._33 /= scale.z;
    }

    // ȸ�� ��Ŀ��� ���ʹϾ� ����
    DirectX::XMStoreFloat4(&rotation, DirectX::XMQuaternionNormalize(DirectX::XMQuaternionRotationMatrix(DirectX::XMLoadFloat4x4(&rotationMatrix))));
}


void BoneMatrix(MeshData& mesh, Node& node, int frame)
{
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT4 rotation;
    DirectX::XMFLOAT3 scale;
    DirectX::XMMATRIX nodeTransform = DirectX::XMLoadFloat4x4(&node.transformMatrix);
    const DirectX::XMMATRIX& parentMatrix = (node.parent != nullptr) ? DirectX::XMLoadFloat4x4(&node.parent->currentMatrix) : DirectX::XMMatrixIdentity();
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
        DirectX::XMMATRIX translationMatrix = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
        DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationQuaternion(DirectX::XMQuaternionRotationRollPitchYaw(rotation.x, rotation.y, rotation.z));
        DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

        nodeTransform = translationMatrix * rotationMatrix * scaleMatrix;
        break;
    }

    // �ڽ� ����� ��ȯ�� �����մϴ�.
    DirectX::XMMATRIX nodeMatrix = nodeTransform * parentMatrix;
    DirectX::XMStoreFloat4x4(&node.currentMatrix, nodeMatrix);
    DirectX::XMStoreFloat4x4(&node.finalMatrix, DirectX::XMLoadFloat4x4(&node.offsetMatrix) * nodeMatrix);
}
void BoneMatrix(MeshData& mesh, int frame)
{
    std::list<Node*> nodes;
    nodes.push_back(&mesh.rootNode);
    Node* node = nullptr;
    while (!nodes.empty())
    {
        node = nodes.front();
        nodes.pop_front();
        BoneMatrix(mesh, *node, frame);
        for (auto& childNode : node->mChildren)
        {
            nodes.push_back(&childNode);
        }
    }
}




void matrixTest(Matrix& mat1, Matrix& mat2)
{
    DirectX::XMFLOAT3 position1, position2;
    DirectX::XMFLOAT4 rotation1, rotation2;
    DirectX::XMFLOAT3 scale1, scale2;
    DecomposeMatrix(mat1, position1, rotation1, scale1);
    DecomposeMatrix(mat2, position2, rotation2, scale2);

    int breakpoint = 0;
}


void CreateMatrix(Matrix& matrix, const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT4& quaternion, const DirectX::XMFLOAT3& scale)
{
    DirectX::XMStoreFloat4x4(&matrix, DirectX::XMMatrixAffineTransformation(XMLoadFloat3(&scale), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), DirectX::XMLoadFloat4(&quaternion), DirectX::XMLoadFloat3(&position)));
}
void CreateMatrix(Matrix& matrix, const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT4& quaternion, const DirectX::XMFLOAT3& scale, const DirectX::XMFLOAT3& pivotPosition)
{
    DirectX::XMStoreFloat4x4(&matrix, DirectX::XMMatrixAffineTransformation(XMLoadFloat3(&scale), XMLoadFloat3(&pivotPosition), DirectX::XMLoadFloat4(&quaternion), DirectX::XMLoadFloat3(&position)));
}


void GetAnimationStates(AnimationNode& node, const int keyframe, Matrix& matrix)
{
    DirectX::XMStoreFloat4x4(&matrix, DirectX::XMMatrixAffineTransformation(DirectX::XMLoadFloat3(&node.scales[keyframe].second), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), DirectX::XMLoadFloat4(&node.quaternionRotations[keyframe].second), XMLoadFloat3(&node.positions[keyframe].second)));
}

void GetAnimationStates(AnimationNode& node, const int keyframe, DirectX::XMFLOAT3& position, DirectX::XMFLOAT4& quaternion, DirectX::XMFLOAT3& scale)
{
    if (keyframe >= node.scales.size())
        scale = node.scales.back().second;
    else
        scale = node.scales[keyframe].second;

    if (keyframe >= node.quaternionRotations.size())
        quaternion = node.quaternionRotations.back().second;
    else
        quaternion = node.quaternionRotations[keyframe].second;

    if (keyframe >= node.positions.size())
        position = node.positions.back().second;
    else
        position = node.positions[keyframe].second;
}

void GetAnimationStates(AnimationNode& node, const int keyframe, Matrix& matrix, DirectX::XMFLOAT3& position, DirectX::XMFLOAT4& quaternion, DirectX::XMFLOAT3& scale)
{
    GetAnimationStates(node, keyframe, matrix);
    GetAnimationStates(node, keyframe, position, quaternion, scale);
}

void GetRelativeState(AnimationNode& node, const int keyframe, Matrix& transformationMatrix, Matrix& matrix)
{
    DirectX::XMFLOAT3 position, otherPosition;
    DirectX::XMFLOAT4 quaternion, otherQuaternion;
    DirectX::XMFLOAT3 scale, otherScale;
    GetAnimationStates(node, keyframe, position, quaternion, scale);
    DecomposeMatrix(transformationMatrix, otherPosition, otherQuaternion, otherScale);


    position.x -= otherPosition.x;
    position.y -= otherPosition.y;
    position.z -= otherPosition.z;

    scale.x -= (otherScale.x - scale.x);
    scale.y -= (otherScale.y - scale.y);
    scale.z -= (otherScale.z - scale.z);

    DirectX::XMVECTOR q1 = DirectX::XMLoadFloat4(&quaternion);
    DirectX::XMVECTOR q2 = DirectX::XMLoadFloat4(&otherQuaternion);

    DirectX::XMMATRIX mat1 = DirectX::XMMatrixRotationQuaternion(q1);
    DirectX::XMMATRIX mat2 = DirectX::XMMatrixInverse(nullptr, DirectX::XMMatrixRotationQuaternion(q2));
    mat1 = mat2 * mat1;

    DirectX::XMVECTOR q2_inv = DirectX::XMQuaternionInverse(q2);
    DirectX::XMVECTOR relativeQuat = DirectX::XMQuaternionMultiply(q2_inv, q1);
    DirectX::XMStoreFloat4(&quaternion, relativeQuat);
    DirectX::XMMATRIX mat3 = DirectX::XMMatrixRotationQuaternion(XMLoadFloat4(&quaternion));


    //DirectX::XMFLOAT4X4 inversed;
    //DirectX::XMStoreFloat4x4(&inversed, DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&transformationMatrix)));
    //DecomposeMatrix(inversed, otherPosition, otherQuaternion, otherScale);

    CreateMatrix(matrix, position, quaternion, scale, otherPosition);
}



void SetBoneMatrix(Node& rootNode)
{
    std::list<Node*> nodeQueue;
    std::list<Node*> nodes;
    DirectX::XMMATRIX totalMatrix;
    nodeQueue.push_back(&rootNode);
    nodes.push_back(&rootNode);
    Node* currentNode = nullptr;
    while (!nodeQueue.empty())
    {
        currentNode = nodeQueue.front();
        nodeQueue.pop_front();

        for (auto& childNode : currentNode->mChildren)
        {
            nodeQueue.push_back(&childNode);
            nodes.push_back(&childNode);
        }

        if (currentNode->parent != nullptr)
            totalMatrix = DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&currentNode->currentMatrix), DirectX::XMLoadFloat4x4(&currentNode->parent->finalMatrix));
        else
            totalMatrix = DirectX::XMLoadFloat4x4(&currentNode->currentMatrix);

        DirectX::XMStoreFloat4x4(&currentNode->finalMatrix, totalMatrix);
    }

    for (auto& childNode : nodes)
    {
        DirectX::XMStoreFloat4x4(&childNode->finalMatrix, DirectX::XMLoadFloat4x4(&childNode->offsetMatrix) * DirectX::XMLoadFloat4x4(&childNode->finalMatrix));
    }
}

void SetBoneMatrixInRelative(Node& rootNode)
{
    std::list<Node*> nodeQueue;
    std::list<Node*> nodes;
    DirectX::XMMATRIX totalMatrix;
    nodeQueue.push_back(&rootNode);
    nodes.push_back(&rootNode);
    Node* currentNode = nullptr;
    while (!nodeQueue.empty())
    {
        currentNode = nodeQueue.front();
        nodeQueue.pop_front();

        for (auto& childNode : currentNode->mChildren)
        {
            nodeQueue.push_back(&childNode);
            nodes.push_back(&childNode);
        }
        if (currentNode->parent != nullptr)
            totalMatrix = DirectX::XMMatrixMultiply(DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&currentNode->transformMatrix), DirectX::XMLoadFloat4x4(&currentNode->currentMatrix)), DirectX::XMLoadFloat4x4(&currentNode->parent->finalMatrix));
        else
            totalMatrix = DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&currentNode->transformMatrix), DirectX::XMLoadFloat4x4(&currentNode->currentMatrix));

        DirectX::XMStoreFloat4x4(&currentNode->finalMatrix, totalMatrix);
    }

    for (auto& childNode : nodes)
    {
        DirectX::XMStoreFloat4x4(&childNode->finalMatrix, DirectX::XMLoadFloat4x4(&childNode->offsetMatrix) * DirectX::XMLoadFloat4x4(&childNode->finalMatrix));
        //DirectX::XMStoreFloat4x4(&childNode->finalMatrix, DirectX::XMLoadFloat4x4(&childNode->finalMatrix)* DirectX::XMLoadFloat4x4(&childNode->offsetMatrix));
        //DirectX::XMStoreFloat4x4(&childNode->finalMatrix, DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&childNode->transformMatrix)) * DirectX::XMLoadFloat4x4(&childNode->offsetMatrix));
    }
}






*/
