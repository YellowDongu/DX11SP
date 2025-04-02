
struct VS_INPUT
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD;
    uint4 boneIDs : BONES; // 4개의 본 인덱스
    float4 weights : WEIGHTS; // 각 본의 가중치
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD;
};

cbuffer WorldMatrixBuffer : register(b0)
{
    float4x4 worldMatrix;
};

cbuffer CameraMatrixBuffer : register(b1)
{
    float4x4 viewMatrix;
    float4x4 projectionMatrix;
};

cbuffer MaterialBuffer : register(b3)
{
    float4 diffuseColor;
    float4 specularColor;
    float shininess;
};

cbuffer BoneBuffer : register(b4)
{
    float4x4 boneTransform[256];
};

VS_OUTPUT VS_Main(VS_INPUT input)
{
    float4x4 skinMatrix = boneTransform[input.boneIDs[0]] * input.weights[0] +
                          boneTransform[input.boneIDs[1]] * input.weights[1] +
                          boneTransform[input.boneIDs[2]] * input.weights[2] +
                          boneTransform[input.boneIDs[3]] * input.weights[3];
    VS_OUTPUT output;
    output.pos = mul(float4(input.pos, 1.0f), skinMatrix);
    //output.normal = mul((float3x3) skinMatrix, input.normal);
    output.pos = float4(input.pos, 1.0f);
    output.tex = input.tex;
    
    //output.pos = mul(output.pos, worldMatrix);
    output.pos = mul(output.pos, viewMatrix);
    output.pos = mul(output.pos, projectionMatrix);
    
    return output;
}

/*
VS_OUTPUT VS_Main(VS_INPUT input)
{
    //float4x4 skinMatrix = boneTransform[input.boneIDs[0]] * input.weights[0] +
    //                      boneTransform[input.boneIDs[1]] * input.weights[1] +
    //                      boneTransform[input.boneIDs[2]] * input.weights[2] +
    //                      boneTransform[input.boneIDs[3]] * input.weights[3];
    VS_OUTPUT output;
    //output.pos = mul(float4(input.pos, 1.0f), skinMatrix);
    //output.normal = mul((float3x3) skinMatrix, input.normal);
    output.pos = float4(input.pos, 1.0f);
    output.tex = input.tex;
    
    //output.pos = mul(output.pos, worldMatrix);
    output.pos = mul(output.pos, viewMatrix);
    output.pos = mul(output.pos, projectionMatrix);
    
    return output;
}

*/