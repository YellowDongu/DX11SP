// Default Pass

// Vertex Shader
struct VS_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 texturePosition : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float4 normal : NORMAL;
    float2 texturePosition : TEXCOORD;
};


matrix viewProjectionMatrix;
matrix worldMatrix;


VS_OUTPUT VS_Main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    
    vector worldVertex = mul(vector(input.position, 1.0f), worldMatrix);
    output.normal = normalize(mul(float4(input.normal, 0.0f), worldMatrix));
    output.position = mul(worldVertex, viewProjectionMatrix);
    output.texturePosition = input.texturePosition;
    return output;
}
// Vertex Shader

// Pixel Shader
Texture2D diffuseTexture : register(t0);
Texture2D specularTexture : register(t1);
SamplerState samplerState : register(s0);
sampler linearSampler = sampler_state
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
    AddressW = WRAP;
    ComparisonFunc = NEVER;
};

cbuffer MaterialBufferInput : register(b11)
{
    float4 diffuseColor;
    float4 specularColor;
    float shininess;
    int hasSpecularTexture; // 1이면 specularTexture 있음, 0이면 없음
    float2 padding; // 16바이트 정렬
};


struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float4 normal : NORMAL;
    float2 tex : TEXCOORD;
};

struct PS_OUTPUT
{
    float4 color : SV_Target0;
};

PS_OUTPUT PS_Main(PS_INPUT input)
{
    PS_OUTPUT output = (PS_OUTPUT) 0;
    output.color = diffuseTexture.Sample(samplerState, input.tex);
    return output;
    
}
// Pixel Shader

// Default Pass

technique11 defaultTechnique
{
    pass defaultPass
    {
        VertexShader = compile vs_5_0 VS_Main();
        PixelShader = compile ps_5_0 PS_Main();
    }
}

