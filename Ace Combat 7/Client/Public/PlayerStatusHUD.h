#pragma once
#include "UIObject.h"

class PlayerStatusHeadUpDisplay : public Engine::UIObject
{
private:
    PlayerStatusHeadUpDisplay(void) = delete;
    PlayerStatusHeadUpDisplay(const PlayerStatusHeadUpDisplay&) = delete;
    PlayerStatusHeadUpDisplay(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
    virtual ~PlayerStatusHeadUpDisplay(void) = default;
    virtual void Free(void);
public:
    static PlayerStatusHeadUpDisplay* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, Engine::GameObject* player);
    virtual Engine::GameObject* Clone(void) override { return nullptr; }

    virtual HRESULT Start(void) override;
    virtual HRESULT Awake(void) override;
    virtual void FixedUpdate(void) override;
    virtual void Update(void) override;
    virtual void LateUpdate(void) override;
    virtual void Render(void) override;

    void LinkData(class Player* player);
    void RenderMissileStatus(UIParts& parts, UIParts& innerParts, FLOAT coolTime, Vector2& Position);

private:
    Engine::Text* text;
    float2 textSize;
    float4 textColor{ 0.0f, 1.0f, 0.0f, 1.0f };
    const struct AircraftMetaData* metaData;
    class FireControlSystem* fcs;
    class RadarMissileWarningReceiver* rmwr;

    UIParts standardMissile, standardMissileInside;
    UIParts uniqueMissile, uniqueMissileInside;
    UIParts aircraft, aircraftInside;

    const UINT stride = sizeof(UIVertex);
    std::string diffuseTexture = "diffuseTexture";
    std::string world = "worldMatrix";
}typedef StatusHUD, PlayerStatudHUD;