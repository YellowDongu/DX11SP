// 인클루드용 hlsl

RasterizerState DefaultRasterizer
{
    FillMode = solid;
};

RasterizerState WireFrametRasterizer
{
    FillMode = solid;
};
//d3d11_Rasterizer_desc 참조해서 값 채워서 쓰기
//앞에 있는 d3d11 뭐시기는 제거

DepthStencilState DefaultDepthStancil
{

};

BlendState DefaultBlendState
{

};