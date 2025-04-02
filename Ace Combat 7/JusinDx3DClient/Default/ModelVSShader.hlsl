// Vertex Shader
struct VS_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD;
    uint4 boneIDs : BONES; // 4개의 본 인덱스
    float4 weights : WEIGHTS; // 각 본의 가중치
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float4 normal : NORMAL;
    float2 tex : TEXCOORD;
};

cbuffer BoneBuffer : register(b4)
{
    matrix boneMatrix[256];
};

matrix viewProjectionMatrix;
matrix worldMatrix;
//matrix BoneMatrix[512];

/*DefaultTechnique -> DefaultPass*/
VS_OUTPUT VS_Main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    
    matrix totalMatrix =
      boneMatrix[input.boneIDs.x] * input.weights.x
    + boneMatrix[input.boneIDs.y] * input.weights.y
    + boneMatrix[input.boneIDs.z] * input.weights.z
    + boneMatrix[input.boneIDs.w] * (1.f - (input.weights.x + input.weights.y + input.weights.z));
    
    vector vertex = mul(float4(input.position, 1.0f), totalMatrix);
    vector normal = mul(float4(input.normal, 0.0f), totalMatrix);
    
    vertex = mul(vertex, worldMatrix);
    
    // 월드, 뷰, 프로젝션 행렬 적용
    output.position = mul(vertex, viewProjectionMatrix);
    output.normal = normalize(mul(normal, worldMatrix));
    output.tex = input.tex;
    return output;
}

/*DefaultTechnique -> StaticModelPass*/
VS_OUTPUT VS_Main_PassTwo(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    
    // 월드, 뷰, 프로젝션 행렬 적용
    vector world = mul(float4(input.position, 1.0f), worldMatrix);
    output.position = mul(world, viewProjectionMatrix);
    output.tex = input.tex;
    return output;
}
// Vertex Shader
/*-----------------------------------------------------------------------------------------------------------------------*/
// Pixel Shader
Texture2D diffuseTexture : register(t0);
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

cbuffer MaterialBufferInput : register(b11)
{
    float4 diffuseColor;
    float4 specularColor;
    float shininess;
    int hasSpecularTexture; // 1이면 specularTexture 있음, 0이면 없음
    float2 padding; // 16바이트 정렬
};

//struct Light
//{
//    vector lightDirection = vector(1.f, -1.f, 0.f, 0.f);
//    vector lightPosition;
//    float lightRange;
//    vector lightDiffuseColor;
//    vector lightAmbientColor;
//    vector lightSpecularColor;
//};
//Light globalLight;
//cbuffer GlobalLight : register(b12)
//{
//    Light light[256];
//};






struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float4 normal : NORMAL;
    float2 tex : TEXCOORD;
};
/* // for light
struct PS_INPUT
{
    float4 position : SV_POSITION;
    float3 normal : SV_NORMAL;
    float2 texture : TEXCOORD0;
    float3 worldPosition : TEXCOORD1;
    float3 camPosition : TEXCOORD2;
};
*/
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

/*DefaultTechnique -> DefaultPass*/
//PS_OUTPUT PS_MAIN(PS_INPUT In)
//{
//    PS_OUTPUT Out = (PS_OUTPUT) 0;
//    
//    vector vMtrlDiffuse = diffuseTexture.Sample(linearSampler, In.tex);
//    
//    if (vMtrlDiffuse.a < 0.3f)
//        discard;
//    
//    float fShade = max(dot(normalize(lightDirection) * -1.f, In.normal), 0.f);
//    
//    vector vReflect = reflect(normalize(lightDirection), normalize(In.normal));
//    vector vLook = normalize(In.worldPosition - camPosition);
//    float fSpecular = pow(max(dot(normalize(vReflect) * -1.f, vLook), 0.f), 50.f);
//    
//    Out.color = lightDiffuseColor * vMtrlDiffuse * saturate(fShade + (lightAmbientColor * ambientColor)) +
//        (lightSpecularColor * specularColor) * fSpecular;
//    
//    return Out;
//}

// Pixel Shader
/*-----------------------------------------------------------------------------------------------------------------------*/

technique11 DefaultTechnique
{
    pass DefaultPass
    {
        VertexShader = compile vs_5_0 VS_Main();
        PixelShader = compile ps_5_0 PS_Main();
    }
    pass StaticModelPass
    {
        VertexShader = compile vs_5_0 VS_Main_PassTwo();
        PixelShader = compile ps_5_0 PS_Main();
        //SetVertexShader(CompileShader(vs_5_0, VSMain()));
        //SetPixelShader(CompileShader(ps_5_0, VS_Main_PassTwo()));
    }
}