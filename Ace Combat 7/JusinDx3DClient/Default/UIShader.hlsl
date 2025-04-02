// Vertex Shader
struct VS_INPUT
{
    float2 pos : POSITION;
    float2 tex : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD;
};

matrix viewProjectionMatrix;
matrix worldMatrix;

/*DefaultTechnique -> DefaultPass*/
VS_OUTPUT VS_Main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    
    vector worldVertex = mul(float4(input.pos, 0.1f, 1.0f), worldMatrix);
    worldVertex.z = 0.1f;
    worldVertex.w = 1.0f;
    output.pos = mul(worldVertex, viewProjectionMatrix);
    output.tex = input.tex;
    return output;
}

// Vertex Shader
/*-----------------------------------------------------------------------------------------------------------------------*/
// Pixel Shader
Texture2D diffuseTexture : register(t0);
sampler linearSampler = sampler_state
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
    AddressW = WRAP;
    ComparisonFunc = NEVER;
};

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD;
};

struct PS_OUTPUT
{
    float4 color : SV_Target0;
};

/*DefaultTechnique -> DefaultPass*/
PS_OUTPUT PS_Main(PS_INPUT input)
{
    PS_OUTPUT output = (PS_OUTPUT) 0;
    float4 outputColor = diffuseTexture.Sample(linearSampler, input.tex);
    
    if (outputColor.a > 0.50f)
    {
        outputColor = float4(1.0, 0.0, 1.0, 1.0); // 완전 파란색
    }
    else
    {
        discard;
        //outputColor = float4(1.0, 0.0, 0.0, 1.0); // 완전 파란색
    }
    output.color = outputColor;
    return output;
}

/*DefaultTechnique -> */
PS_OUTPUT PS_Main_HUD(PS_INPUT input)
{
    PS_OUTPUT output = (PS_OUTPUT) 0;
    float4 outputColor = mul(diffuseTexture.Sample(linearSampler, input.tex), float4(0.0, 1.0, 0.0, 1.0));
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