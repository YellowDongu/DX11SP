#include "Foundation.h"
#include "LineDrawer.h"
#include "GameInstance.h"
using namespace Engine;

LineDrawer::LineDrawer(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Component(dxDevice, dxDeviceContext), positions{}, primitiveBatch(nullptr), basicEffect(nullptr), inputLayout(nullptr)
{
}

LineDrawer::LineDrawer(const LineDrawer& other) : Component(other), positions(other.positions), primitiveBatch(other.primitiveBatch), basicEffect(other.basicEffect), inputLayout(other.inputLayout)
{
}

void LineDrawer::Free(void)
{
    positions.clear();
    inputLayout.Reset();
    primitiveBatch = nullptr;
    basicEffect = nullptr; 
    inputLayout = nullptr;

}

LineDrawer* LineDrawer::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
    LineDrawer* newInstance = new LineDrawer(dxDevice, dxDeviceContext);
    if (FAILED(newInstance->Start()))
    {
        Base::Destroy(newInstance);
        return nullptr;
    }
    return newInstance;
}

Component* LineDrawer::Clone(void)
{
    return new LineDrawer(*this);
}

HRESULT LineDrawer::Start(void)
{
    primitiveBatch = std::make_shared<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>(dxDeviceContext);
    if (primitiveBatch == nullptr)
        return E_FAIL;

    basicEffect = std::make_shared<DirectX::BasicEffect>(dxDevice);
    if (basicEffect == nullptr)
        return E_FAIL;
    basicEffect->SetVertexColorEnabled(true);

    void const* shaderByteCode;
    size_t byteCodeLength;
    basicEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

    return dxDevice->CreateInputLayout(DirectX::VertexPositionColor::InputElements, DirectX::VertexPositionColor::InputElementCount, shaderByteCode, byteCodeLength, inputLayout.GetAddressOf());
}

void LineDrawer::Render(void)
{
    if (positions.size() <= 1)
        return;

    dxDeviceContext->IASetInputLayout(inputLayout.Get());

    basicEffect->SetProjection(DirectX::XMLoadFloat4x4(&EngineInstance()->ProjectionMatrix()));
    basicEffect->SetView(DirectX::XMLoadFloat4x4(&EngineInstance()->ViewMatrix()));
    basicEffect->SetWorld(DirectX::XMMatrixIdentity());

    basicEffect->Apply(dxDeviceContext);

    primitiveBatch->Begin();

    auto priviousIterator = positions.begin();
    for (auto iterator = ++(positions.begin()); iterator != positions.end(); )
    {
        primitiveBatch->DrawLine(
            DirectX::VertexPositionColor(DirectX::XMLoadFloat3(&*iterator), DirectX::XMLoadFloat4(&color)),
            DirectX::VertexPositionColor(DirectX::XMLoadFloat3(&*priviousIterator), DirectX::XMLoadFloat4(&color)));

        iterator++; priviousIterator++;
    }

    primitiveBatch->End();
}
