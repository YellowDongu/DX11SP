#include "Foundation.h"
#include "Material.h"
#include "GameInstance.h"

using namespace Engine;



Material::Material(void) : diffuseColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)), specularColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)), shininess(32.0f), diffuseTexture(nullptr), specularTexture(nullptr), hasSpecularTexture(false), normalTexture(nullptr) ,buffer(){}


Material* Material::Create(ModelConverter::FullModelMaterial& material, const std::wstring& modelPath)
{
    Material* newInstance = new Material();
    if (FAILED(newInstance->LoadMaterial(material, modelPath)))
    {
        Base::Destroy(newInstance);
        return nullptr;
    }
    return newInstance;
}

void Material::Free(void)
{
}

HRESULT Material::GetTexture(std::wstring key, std::wstring path, ID3D11ShaderResourceView*& texture)
{
    GameInstance* instance = EngineInstance();
    if (!instance)
        return E_FAIL;

    HRESULT result = instance->GetTexture(key, texture);
    if (SUCCEEDED(result))
        return result;

    result = instance->SetTexture(key, path);
    if (FAILED(result))
        return result;

    return instance->GetTexture(key, texture);
}

HRESULT Material::LoadMaterial(ModelConverter::FullModelMaterial& material, const std::wstring& modelPath)
{
    std::wstring texPath;
    HRESULT result = S_OK;


    /*enum aiTextureType  --- texture type index
    {
        aiTextureType_NONE = 0,
        aiTextureType_DIFFUSE = 1,
        aiTextureType_SPECULAR = 2,
        aiTextureType_AMBIENT = 3,
        aiTextureType_EMISSIVE = 4,
        aiTextureType_HEIGHT = 5,
        aiTextureType_NORMALS = 6,
        aiTextureType_SHININESS = 7,
        aiTextureType_OPACITY = 8,
        aiTextureType_DISPLACEMENT = 9,
        aiTextureType_LIGHTMAP = 10,
        aiTextureType_REFLECTION = 11,
        aiTextureType_BASE_COLOR = 12,
        aiTextureType_NORMAL_CAMERA = 13,
        aiTextureType_EMISSION_COLOR = 14,
        aiTextureType_METALNESS = 15,
        aiTextureType_DIFFUSE_ROUGHNESS = 16,
        aiTextureType_AMBIENT_OCCLUSION = 17,
        aiTextureType_UNKNOWN = 18,
        aiTextureType_SHEEN = 19,
        aiTextureType_CLEARCOAT = 20,
        aiTextureType_TRANSMISSION = 21, AI_TEXTURE_TYPE_MAX

        #ifndef SWIG
        _aiTextureType_Force32Bit = INT_MAX
        #endif
    };*/

    std::wstring fullPath;

    // Diffuse Texture
    texPath = To_WString(material.texturePath[1]);
    if (texPath.length() != 0)
    {
        fullPath = getAbsolutePathFromFile(modelPath, texPath);
        if (FAILED(GetTexture(fullPath, fullPath, diffuseTexture)))
        {
            std::wstring errMsg = L"TextureLoadError : " + fullPath;
            ErrMsg(errMsg.c_str());
            result = E_FAIL;
        }
    }

    // Normal Texture
    texPath = To_WString(material.texturePath[6]);
    if (texPath.length() != 0)
    {
        fullPath = getAbsolutePathFromFile(modelPath, texPath);
        normalTexture = nullptr;
        //if (FAILED(GetTexture(fullPath, fullPath, diffuseTexture)))
        //{
        //    std::wstring errMsg = L"TextureLoadError : " + fullPath;
        //    ErrMsg(errMsg.c_str());
        //    result = E_FAIL;
        //}
    }

    // Specular Texture
    texPath = To_WString(material.texturePath[2]);
    if (texPath.length() != 0)
    {
        fullPath = getAbsolutePathFromFile(modelPath, texPath);
        normalTexture = nullptr;
        //if (FAILED(GetTexture(fullPath, fullPath, diffuseTexture)))
        //{
        //    std::wstring errMsg = L"TextureLoadError : " + fullPath;
        //    ErrMsg(errMsg.c_str());
        //    result = E_FAIL;
        //}
    }
    // allTexture
    //for (size_t i = 0; i < 21; i++)
    //{
    //    texPath = To_WString(material.texturePath[i]);
    //    if (texPath.length() != 0)
    //    {
    //        fullPath = getAbsolutePathFromFile(modelPath, texPath);
    //        if (FAILED(GetTexture(fullPath, fullPath, Texture[i])))
    //        {
    //            std::wstring errMsg = L"TextureLoadError : " + fullPath;
    //            ErrMsg(errMsg.c_str());
    //            result = E_FAIL;
    //        }
    //    }
    //}

    memcpy(&diffuseColor, &material.diffuseColor, sizeof(DirectX::XMFLOAT4));
    memcpy(&specularColor, &material.specularColor, sizeof(DirectX::XMFLOAT4));
    memcpy(&shininess, &material.shininess, sizeof(FLOAT));
    hasSpecularTexture = (specularTexture != nullptr) ? 1 : 0;

    ZeroMemory(&buffer, sizeof(MaterialBufferType));
    memcpy(&buffer.diffuseColor, &diffuseColor, sizeof(DirectX::XMFLOAT4));
    memcpy(&buffer.specularColor, &specularColor, sizeof(DirectX::XMFLOAT4));
    memcpy(&buffer.shininess, &shininess, sizeof(FLOAT));
    memcpy(&buffer.hasSpecularTexture, &hasSpecularTexture, sizeof(bool));

    return S_OK;
    return result;
}

HRESULT Material::SetMaterial(ID3D11DeviceContext*& context)
{
    if (GetCurrentShader() == nullptr)
        return E_FAIL;

    ID3D11Buffer* materialBuffer = GetCurrentShader()->ConstantBuffer(GetCurrentShader()->MaterialConstantBuffer);

    if (materialBuffer == nullptr)
        return E_FAIL;

    // 디퓨즈 텍스처 바인딩
    ::SetTexture(GetCurrentShader()->diffuseTextureA, diffuseTexture);
    // 스펙큘러 텍스처 바인딩
    //::SetTexture(SpecularTextureBufferA, specularTexture);
    // 노멀맵 텍스쳐 바인딩
    //::SetTexture(NormalTextureBufferA, normalTexture);

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    if (SUCCEEDED(context->Map(materialBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
    {
        MaterialBufferType* dataPtr = (MaterialBufferType*)mappedResource.pData;
        memcpy(dataPtr, &buffer, sizeof(MaterialBufferType));
        context->Unmap(materialBuffer, 0);

        context->PSSetConstantBuffers(1, 1, &materialBuffer);
        return S_OK;
    }
    return E_FAIL;
}

#ifdef useAssimp
Material* Material::Create(aiMaterial* material, const std::wstring& modelPath)
{
    Material* newInstance = new Material();
    if (FAILED(newInstance->LoadMaterial(material, modelPath)))
    {
        Base::Destroy(newInstance);
        return nullptr;
    }
    return newInstance;
}

HRESULT Material::LoadMaterial(aiMaterial* aiMaterial, const std::wstring& modelPath)
{
    aiString texturePath;
    std::wstring texPath;
    HRESULT result = S_OK;
    aiColor4D loadedColor;

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

    // 디퓨즈 텍스처 로드
    if (aiMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == AI_SUCCESS)
    {
        texPath = std::wstring(texturePath.C_Str(), texturePath.C_Str() + strlen(texturePath.C_Str()));
        std::wstring fullPath = getAbsolutePathFromFile(modelPath, texPath);
        if (FAILED(GetTexture(fullPath, fullPath, diffuseTexture)))
        {
            std::wstring errMsg = L"TextureLoadError : " + fullPath;
            //ErrMsg(errMsg.c_str());
            //result = E_FAIL;
        }
    }

    // 노멀 텍스처 로드
    if (aiMaterial->GetTexture(aiTextureType_NORMALS, 0, &texturePath) == AI_SUCCESS)
    {
        texPath = std::wstring(texturePath.C_Str(), texturePath.C_Str() + strlen(texturePath.C_Str()));
        std::wstring fullPath = getAbsolutePathFromFile(modelPath, texPath);
        //if (FAILED(GetTexture(fullPath, fullPath, normalTexture)))
        // {
        // 
        //    std::wstring errMsg = L"TextureLoadError : " + fullPath;
        //    ErrMsg(errMsg.c_str());
        //    result = E_FAIL;
        // }
    }

    // 스펙큘러 텍스처 로드
    if (aiMaterial->GetTexture(aiTextureType_SPECULAR, 0, &texturePath) == AI_SUCCESS)
    {
        texPath = std::wstring(texturePath.C_Str(), texturePath.C_Str() + strlen(texturePath.C_Str()));
        std::wstring fullPath = getAbsolutePathFromFile(modelPath, texPath);
        //if (FAILED(GetTexture(fullPath, fullPath, specularTexture)))
        // {
        // 
        //    std::wstring errMsg = L"TextureLoadError : " + fullPath;
        //    ErrMsg(errMsg.c_str());
        //    result = E_FAIL;
        // }
    }

    // 추가 속성 (디퓨즈 색상, 스펙큘러 색상 등) 로드
    if (aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, loadedColor) == AI_SUCCESS)
        diffuseColor = DirectX::XMFLOAT4(loadedColor.r, loadedColor.g, loadedColor.b, loadedColor.a);
    if (aiMaterial->Get(AI_MATKEY_COLOR_SPECULAR, loadedColor) == AI_SUCCESS)
        specularColor = DirectX::XMFLOAT4(loadedColor.r, loadedColor.g, loadedColor.b, loadedColor.a);

    ai_real aiShininess;
    if (aiMaterial->Get(AI_MATKEY_SHININESS, aiShininess) == AI_SUCCESS)
        shininess = aiShininess;

    //if (aiMaterial->Get(AI_MATKEY_COLOR_AMBIENT, loadedColor) == AI_SUCCESS)
    //    ambientColor = DirectX::XMFLOAT4(loadedColor.r, loadedColor.g, loadedColor.b, loadedColor.a);
    //if (aiMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, loadedColor) == AI_SUCCESS)
    //    emmisiveColor = DirectX::XMFLOAT4(loadedColor.r, loadedColor.g, loadedColor.b, loadedColor.a);



    hasSpecularTexture = (specularTexture != nullptr) ? 1 : 0;

    ZeroMemory(&buffer, sizeof(MaterialBufferType));
    buffer.diffuseColor = diffuseColor;
    buffer.specularColor = specularColor;
    buffer.shininess = shininess;
    buffer.hasSpecularTexture = hasSpecularTexture;

    return S_OK;
    return result;
}


#endif // useAssimp
