// Vertex Shader
struct VS_INPUT
{
    float3 position : POSITION;
    float3 tex : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float3 tex : TEXCOORD;
};

matrix viewProjectionMatrix;
matrix worldMatrix;

/*DefaultTechnique -> DefaultPass*/
VS_OUTPUT VS_Main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    
    vector vertex = mul(float4(input.position, 1.0f), worldMatrix);
    
    output.position = mul(vertex, viewProjectionMatrix);
    output.tex = input.tex;
    return output;
}

// Vertex Shader
/*-----------------------------------------------------------------------------------------------------------------------*/
// Pixel Shader
TextureCube diffuseTexture : register(t0);
Texture2D specularTexture : register(t1);
SamplerState samplerState : register(s0);
sampler linearSampler = sampler_state
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = WRAP; // 랩 모드, 텍스쳐 타일링을 위한 거, 1 이상되면 0으로 돌아감 
    //AddressU = CLAMP; // 1 이상은 1로 인지 랩 모드, 텍스쳐 타일링을 위한 거
    AddressV = WRAP;
    AddressW = WRAP;
    ComparisonFunc = NEVER;
    //MinLOD = 0;
    //MaxLOD = FLT_MAX;
};

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float3 tex : TEXCOORD;
};
struct PS_OUTPUT
{
    float4 color : SV_Target0;
};

/*DefaultTechnique -> DefaultPass*/
PS_OUTPUT PS_Main(PS_INPUT input)
{
    PS_OUTPUT output = (PS_OUTPUT) 0;
    output.color = diffuseTexture.Sample(linearSampler, input.tex);
    
    return output;
    
}
// Pixel Shader
/*-----------------------------------------------------------------------------------------------------------------------*/

technique11 DefaultTechnique
{
    pass DefaultPass
    {
        VertexShader = compile vs_5_0 VS_Main();
        PixelShader = compile ps_5_0 PS_Main();
    }
}