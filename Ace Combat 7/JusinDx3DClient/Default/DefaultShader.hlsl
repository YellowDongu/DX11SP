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


cbuffer CameraMatrixBuffer : register(b1)
{
    float4x4 viewMatrix;
    float4x4 projectionMatrix;
};

cbuffer BoneBuffer : register(b4)
{
    float4x4 offsetMatrix[256];
    float4x4 transformMatrix[256];
    float4x4 animationMatrix[256];
};

matrix worldMatrix;


VS_OUTPUT VS_Main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    vector viewed;
    vector worldVertex;
    // 월드, 뷰, 프로젝션 행렬 적용
    viewed = mul(float4(input.position, 1.0f), transpose(viewMatrix));
    output.position = mul(viewed, transpose(projectionMatrix));
    output.texturePosition = input.texturePosition;
    return output;
    
    worldVertex = mul(float4(input.position, 1.0f), worldMatrix);
    viewed = mul(worldVertex, transpose(viewMatrix));
    output.position = mul(viewed, transpose(projectionMatrix));
    output.normal = normalize(mul(float4(input.normal, 0.0f), worldMatrix));
    output.texturePosition = input.texturePosition;
    return output;
}
// Vertex Shader

// Pixel Shader
Texture2D diffuseTexture : register(t0);
Texture2D specularTexture : register(t1);
SamplerState samplerState : register(s0);

cbuffer WorldLight
{
    vector worldLightDiffuse = vector(1.0f, 1.0f, 1.0f, 1.0f);
    float3 worldLightDirection = float3(1.f, -1.f, 1.f);
    
    
    
    vector worldLightAmbient = vector(1.f, 1.f, 1.f, 1.f);
    
    
    float minimumPower = 0.1f;
};

cbuffer MaterialBufferInput : register(b11)
{
    float4 diffuseColor;
    float4 specularColor;
    float4 ambientColor;
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

/*
struct PS_INPUT
{
    float4 position : SV_POSITION;
    float3 normal : SV_NORMAL;
    float2 texture : TEXCOORD0;
    float3 worldPosition : TEXCOORD1;
};
*/

struct PS_OUTPUT
{
    float4 color : SV_Target0;
};

PS_OUTPUT PS_Main(PS_INPUT input)
{
    PS_OUTPUT output = (PS_OUTPUT) 0;
    output.color = diffuseTexture.Sample(samplerState, input.tex);
    return output;
    
    // 빛 연산
    vector sampled = diffuseTexture.Sample(samplerState, input.tex);
    float shaded = max(dot(normalize(float4(worldLightDirection, 0.0f)) * -1.0f, input.normal), 0.f); // 각도에 따른 셰이딩
    
    sampled = worldLightDiffuse * sampled * saturate(shaded + (worldLightAmbient * ambientColor)); // 램버트 공식
    output.color = sampled;
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


/*
struct InputVertex
{
    float3 position : POSITION;
    float2 texture : TEXCOORD0;
};

struct Output
{
    float4 position : POSITION;
    float2 texture : TEXCOORD0;
};

// constant table
// 전역변수와 비슷하긴 하지만 저 전역변수들은 상수화(값바꾸기 불가능)되어있음 => 초기화를 통해서 값을 바꿀 수 있음
// 셰이더 외부 데이터들을 받아오기 위해서 선언함
matrix worldMatrix;
matrix viewMatrix;
matrix projectionMatrix;
// constant table
Texture2D diffuseTexture;

sampler linearSampler = sampler_state
{
    Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    //AddressU = clamp; // 1 이상은 1로 인지 랩 모드, 텍스쳐 타일링을 위한 거
    //AddressU = wrap; // 랩 모드, 텍스쳐 타일링을 위한 거, 1 이상되면 0으로 돌아감 
    AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    ComparisonFunc = D3D11_COMPARISON_NEVER;
    MinLOD = 0;
    MaxLOD = D3D11_FLOAT32_MAX;
};


// Entry Point
Output main(InputVertex input)
{
    Output output = (Output) 0; // 모든 멤버 0으로 초기화
    output.position = float4(input.position, 1.0f);
    
    
    float4 worldPosition = output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    input.texture = output.texture;
    
    return output;
}


struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD;
};
struct PS_OUTPUT
{
    float4 color : SV_Target0;
};

// Entry Point
PS_OUTPUT PS_Main(PS_INPUT input)
{
    PS_OUTPUT output = (PS_OUTPUT) 0;
    output.color = diffuseTexture.Sample(linearSampler, input.tex);
    
    return output;
}
// 픽셀 셰이더 코드는 길어지지 않는게 좋음 => 애초에 많이 불려오는 함수임, 특히 이 안에서 루프를 돌도록 하면 성능에 치명적임


// effect framework shading
// 셰이더 파일 하나에서 픽셀, 버텍스 셰이더 등등 전부 모아둔 경우도 있고 이걸 전부 분리하는 경우도 있지만
// 개인차인듯
// 원래는 전부 분리하는걸로 설계되어서 이걸 모아두는 파일을 위해 technique를 만듬

// Technique
// Technique11 DefaultTechnique  원래 이런식으로 해야되는데 지금 인식이 안된다
// 이걸 지원안하는 고대의 유물 그래픽카드가 있는데 지금 그럴 일은 없음
// 한 파일 내의 다양한 기법의 셰이더를 구분해서 그떄그떄 바꿔 쓰기 위해 여러 pass들을 정의해둠
// 테그니컬 내 패스의 갯수를 알 수 있음
// 원 패스 렌더링이 정석이고 투 패스 이상은 그림자같은 특이사항이 아닌 경우 외에는 원 패스를 사용해야됨
// 패스 안에서 렌더 스테이트를 정의할 수 있음
Technique DefaultTechnique
{
    pass defaultPass
    {
        VertexShader = compile vs_5_0 main();
        PixelShader = compile ps_5_0 PS_Main();
    }
    pass otherPass
    {
        VertexShader = compile vs_5_0 vs_main();
        PixelShader = compile ps_5_0 ps_main();
    }
}*/