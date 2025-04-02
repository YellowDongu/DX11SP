Texture2D diffuseTexture : register(t0);
Texture2D specularTexture : register(t1);
SamplerState samplerState : register(s0);

cbuffer MaterialBuffer : register(b11)
{
    float4 diffuseColor;
    float4 specularColor;
    float shininess;
    int hasSpecularTexture;  // 1이면 specularTexture 있음, 0이면 없음
    float2 padding;          // 16바이트 정렬
};

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD;
};

/*
struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 worldPosition : TEXCOORD1;
};
*/

struct PS_OUTPUT
{
    float4 color : SV_Target0;
};

PS_OUTPUT PS_Main(PS_INPUT input)
{
    PS_OUTPUT output;
    output.color = diffuseTexture.Sample(samplerState, input.tex);
    
    return output;
    
}


/*Texture2D diffuseTexture : register(t0);
Texture2D specularTexture : register(t1);
SamplerState samplerState : register(s0);

cbuffer MaterialBuffer : register(b11)
{
    float4 diffuseColor;
    float4 specularColor;
    float shininess;
    int hasSpecularTexture; // 1이면 specularTexture 있음, 0이면 없음
    float3 padding; // 16바이트 정렬
};

cbuffer LightBuffer : register(b12)
{
    float3 lightDirection; // 광원 방향 (정규화된 벡터)
    float intensity; // 광원의 강도
};

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD;
    float3 normal : NORMAL; // 추가됨
};

float4 PS_Main(PS_INPUT input) : SV_Target
{
    float4 texDiffuse = diffuseTexture.Sample(samplerState, input.tex);
    float4 texSpecular = hasSpecularTexture ? specularTexture.Sample(samplerState, input.tex) : float4(1, 1, 1, 1);

    // 정규화된 법선 벡터 사용
    float3 normal = normalize(input.normal);
    
    // 디퓨즈 라이팅 계산 (람버트 반사 모델)
    float diffuseIntensity = max(dot(normal, -lightDirection), 0.0f);
    
    // 최종 색상 계산
    float4 finalColor = (diffuseColor * texDiffuse + specularColor * texSpecular) * diffuseIntensity * intensity;

    return finalColor;
}*/