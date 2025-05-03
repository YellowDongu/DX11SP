#pragma once


inline ENGINEDLL std::string To_String(const std::wstring& targetString)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(targetString);
}

inline ENGINEDLL std::wstring To_WString(const std::string& targetString)
{
    int wstrLength = MultiByteToWideChar(CP_UTF8, 0, targetString.c_str(), -1, nullptr, 0);
    std::wstring wideString(wstrLength - 1, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, targetString.c_str(), -1, &wideString[0], wstrLength);
    return wideString;
}

#define Msg(head, contents) MessageBoxW(0, contents, head, MB_OK)
#define ErrMsg(contents) MessageBoxW(0, contents, L"Error", MB_OK)


inline void ContvertMatrix(Matrix& out, const aiMatrix4x4& in)
{
    memcpy(&out, &in, sizeof(Matrix)); return;
    out._11 = in.a1; out._12 = in.a2; out._13 = in.a3; out._14 = in.a4;
    out._21 = in.b1; out._22 = in.b2; out._23 = in.b3; out._24 = in.b4;
    out._31 = in.c1; out._32 = in.c2; out._33 = in.c3; out._34 = in.c4;
    out._41 = in.d1; out._42 = in.d2; out._43 = in.d3; out._44 = in.d4;
}

inline bool isAbsolutePath(const std::string& path)
{
    #if defined(_WINDOWS) || defined(_WIN32)
    // 드라이브 문자 확인 (예: C:\ 또는 D:\)
    if (path.size() > 2 && std::isalpha(path[0]) && path[1] == ':' && (path[2] == '\\' || path[2] == '/'))
    {
        return true;
    }
    // UNC 경로 확인 (예: \\Server\Share\Path)
    if (path.size() > 1 && path[0] == '\\' && path[1] == '\\')
    {
        return true;
    }
    return false;

    #else
    return !path.empty() && path[0] == '/';
    #endif
}
inline bool isAbsolutePath(const std::wstring& path)
{
    #if defined(_WINDOWS) || defined(_WIN32)
    // 드라이브 문자 확인 (예: C:\ 또는 D:\)
    if (path.size() > 2 && std::iswalpha(path[0]) && path[1] == L':' && (path[2] == L'\\' || path[2] == L'/'))
    {
        return true;
    }
    // UNC 경로 확인 (예: \\Server\Share\Path)
    if (path.size() > 1 && path[0] == L'\\' && path[1] == L'\\')
    {
        return true;
    }
    return false;

    #else
    return !path.empty() && path[0] == L'/';
    #endif
}


inline std::wstring getAbsolutePathFromFile(const std::wstring& baseFilePath, const std::wstring& relativePath)
{
    if (isAbsolutePath(relativePath))
        return relativePath;

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


inline Matrix MatrixIdentity(void)
{
    Matrix result;
    DirectX::XMStoreFloat4x4(&result, DirectX::XMMatrixIdentity());
    return result;
}

inline float clamp(float value, float minVal, float maxVal)
{
    return (value < minVal) ? minVal : (value > maxVal) ? maxVal : value;
}
inline Vector3 QuaternionToAngle(fxmVector quaternion)
{
    Matrix matrix;
    XMStoreFloat4x4(&matrix, DirectX::XMMatrixRotationQuaternion(quaternion));
    return Vector3(DirectX::XMConvertToDegrees(asinf(-clamp(-matrix._23, -1.0f, 1.0f))),DirectX::XMConvertToDegrees(atan2f(matrix._13, matrix._33)) ,DirectX::XMConvertToDegrees(atan2f(matrix._21, matrix._22)));
}


inline ENGINEDLL DirectX::XMFLOAT2 WorldToScreen(const DirectX::XMFLOAT3& worldPos, const DirectX::XMMATRIX& viewProjectionMatrix, const D3D11_VIEWPORT* deviceInfomation, bool& insideScreen, bool checkOverFarPlane = true)
{
    DirectX::XMVECTOR clipSpacePos = XMVector3TransformCoord(DirectX::XMLoadFloat3(&worldPos), viewProjectionMatrix);

    DirectX::XMFLOAT4 ndcPos;
    DirectX::XMStoreFloat4(&ndcPos, clipSpacePos);

    if (ndcPos.w != 0.0f)
    {
        ndcPos.x /= ndcPos.w;
        ndcPos.y /= ndcPos.w;
        ndcPos.z /= ndcPos.w;
    }
    if (checkOverFarPlane)
        insideScreen = std::abs(ndcPos.x) <= 1.0f && std::abs(ndcPos.y) <= 1.0f && ndcPos.z >= 0.0f && ndcPos.z <= 1.0f;
    else
        insideScreen = std::abs(ndcPos.x) <= 1.0f && std::abs(ndcPos.y) <= 1.0f && ndcPos.z >= 0.0f;

    return DirectX::XMFLOAT2(ndcPos.x, ndcPos.y);
}


// [min, max) 랜덤 float값 생성
inline ENGINEDLL FLOAT RandomFloat(FLOAT min, FLOAT max)
{
    if (min >= max) return min;
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}
