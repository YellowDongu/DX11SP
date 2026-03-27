#include "pch.h"
#include "PlayerStatusHUD.h"
#include "AircraftMetaData.h"
#include "Player.h"
#include "FireControlSystem.h"
#include "RMWR.h"
#include "PlayerPilot.h"
PlayerStatusHeadUpDisplay::PlayerStatusHeadUpDisplay(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::UIObject(dxDevice, dxDeviceContext), text(nullptr)
{
}

void PlayerStatusHeadUpDisplay::Free(void)
{
    Base::Destroy(text);

    standardMissile.Free();
    standardMissileInside.Free();
    uniqueMissile.Free();
    uniqueMissileInside.Free();
    aircraft.Free();
    aircraftInside.Free();
    missionEndParts.Free();
}

PlayerStatusHeadUpDisplay* PlayerStatusHeadUpDisplay::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, Engine::GameObject* player)
{
    PlayerStatusHeadUpDisplay* newInstance = new PlayerStatusHeadUpDisplay(dxDevice, dxDeviceContext);
    newInstance->LinkData(static_cast<Player*>(player));
    if (FAILED(newInstance->Start()))
    {
        Base::Destroy(newInstance);
        return nullptr;
    }

    return newInstance;
}

#define innerErrorCheck(result, errmsg)					   \
if (FAILED(result))										   \
{														   \
	ErrMsg((std::wstring(L"PSHUD::Failed : ") + errmsg).c_str()); \
	return E_FAIL;										   \
}														   \

HRESULT PlayerStatusHeadUpDisplay::Start(void)
{
    innerErrorCheck(standardMissile.LoadUITexture(L"../Bin/Resources/UI/HUD/PrograssBar/Weapons/w_msl_a0.dds"), L"Load texture - w_msl_a0.dds");
    CreateScale(standardMissile.texture, scale);
    innerErrorCheck(CreateVertex(standardMissile.vertexBuffer, standardMissile.indexBuffer, standardMissile.indexCount, scale), L"Vertex/Index Create");

    innerErrorCheck(standardMissileInside.LoadUITexture(L"../Bin/Resources/UI/HUD/PrograssBar/Weapons/w_msl_a0_inside.dds"), L"Load texture - w_msl_a0_inside.dds");
    CreateScale(standardMissileInside.texture, scale);
    innerErrorCheck(CreateVertex(standardMissileInside.vertexBuffer, standardMissileInside.indexBuffer, standardMissileInside.indexCount, scale), L"Vertex/Index Create");

    innerErrorCheck(uniqueMissile.LoadUITexture(L"../Bin/Resources/UI/HUD/PrograssBar/Weapons/w_laam_a0.dds"), L"Load texture - w_laam_a0.dds");
    CreateScale(uniqueMissile.texture, scale);
    innerErrorCheck(CreateVertex(uniqueMissile.vertexBuffer, uniqueMissile.indexBuffer, uniqueMissile.indexCount, scale), L"Vertex/Index Create");

    innerErrorCheck(uniqueMissileInside.LoadUITexture(L"../Bin/Resources/UI/HUD/PrograssBar/Weapons/w_laam_a0_inside.dds"), L"Load texture - w_laam_a0_inside.dds");
    CreateScale(uniqueMissileInside.texture, scale);
    innerErrorCheck(CreateVertex(uniqueMissileInside.vertexBuffer, uniqueMissileInside.indexBuffer, uniqueMissileInside.indexCount, scale), L"Vertex/Index Create");

    if (metaData->AircraftModelName == L"F15E")
    {
        innerErrorCheck(aircraft.LoadUITexture(L"../Bin/Resources/UI/HUD/PrograssBar/Aircraft/hud_acIcon_f15e.png"), L"Load texture - hud_acIcon_f15e.png");
        innerErrorCheck(aircraftInside.LoadUITexture(L"../Bin/Resources/UI/HUD/PrograssBar/Aircraft/hud_acIcon_f15e_inside.png"), L"Load texture - hud_acIcon_f15e_inside.png");
    }
    else if (metaData->AircraftModelName == L"F16C")
    {
        innerErrorCheck(aircraft.LoadUITexture(L"../Bin/Resources/UI/HUD/PrograssBar/Aircraft/hud_acIcon_f16c.png"), L"Load texture - hud_acIcon_f16c.png");
        innerErrorCheck(aircraftInside.LoadUITexture(L"../Bin/Resources/UI/HUD/PrograssBar/Aircraft/hud_acIcon_f16c_inside.png"), L"Load texture - hud_acIcon_f16c_inside.png");
    }
    else if (metaData->AircraftModelName == L"F14D")
    {
        innerErrorCheck(aircraft.LoadUITexture(L"../Bin/Resources/UI/HUD/PrograssBar/Aircraft/hud_acIcon_f14d.png"), L"Load texture - hud_acIcon_f14d.png");
        innerErrorCheck(aircraftInside.LoadUITexture(L"../Bin/Resources/UI/HUD/PrograssBar/Aircraft/hud_acIcon_f14d_inside.png"), L"Load texture - hud_acIcon_f14d_inside.png");
    }
    else /*if (metaData->AircraftModelName == L"SU33") -> For Memo */
    {
        innerErrorCheck(aircraft.LoadUITexture(L"../Bin/Resources/UI/HUD/PrograssBar/Aircraft/hud_acIcon_su33.png"), L"Load texture - hud_acIcon_su33.png");
        innerErrorCheck(aircraftInside.LoadUITexture(L"../Bin/Resources/UI/HUD/PrograssBar/Aircraft/hud_acIcon_su33_inside.png"), L"Load texture - hud_acIcon_su33_inside.png");
    }

    CreateScale(aircraft.texture, scale);
    innerErrorCheck(CreateVertex(aircraft.vertexBuffer, aircraft.indexBuffer, aircraft.indexCount, scale), L"Vertex/Index Create");

    CreateScale(aircraftInside.texture, scale);
    innerErrorCheck(CreateVertex(aircraftInside.vertexBuffer, aircraftInside.indexBuffer, aircraftInside.indexCount, scale), L"Vertex/Index Create");
    

    innerErrorCheck(missionEndParts.LoadUITexture(L"../Bin/Resources/UI/Menu/CommonWidget/Assets/LensFlare/anamaphoric_ring_3.png"), L"Load texture - anamaphoric_ring_3.png");
    CreateScale(missionEndParts.texture, missionEndParts.scale);
    innerErrorCheck(CreateVertex(missionEndParts.vertexBuffer, missionEndParts.indexBuffer, missionEndParts.indexCount, missionEndParts.scale), L"Vertex/Index Create");

    text = ::CreateText(L"../Bin/ACES07.spritefont"); 
    if (text == nullptr)
        return E_FAIL;

    textSize = Vector2::one();
    return S_OK;
}

HRESULT PlayerStatusHeadUpDisplay::Awake(void)
{
    return S_OK;
}

void PlayerStatusHeadUpDisplay::FixedUpdate(void)
{
}

void PlayerStatusHeadUpDisplay::Update(void)
{
}

void PlayerStatusHeadUpDisplay::LateUpdate(void)
{
    AddRenderObject(RenderType::UI, this);
}

void PlayerStatusHeadUpDisplay::Render(void)
{
    Matrix worldMatrix;
    Vector2 position = { static_cast<FLOAT>(windowSizeX) * 0.35f, static_cast<FLOAT>(windowSizeY) * -0.16f };
    Engine::Shader* shader = ::GetCurrentShader();

    float winY = 16.0f * 1.7f;
    float xStore = position.x, xSecondStore = position.x + 16.0f * 10.0f;
    text->RenderText(L"GUN", position, textSize, textColor, 0.0f);
    position.x = xSecondStore;
    text->RenderLeftAlinedText(std::to_wstring(fcs->GunCount()), position, textSize, textColor, 0.0f);
    position.x = xStore;


    position.y -= winY * 1.2f;
    text->RenderText(L"MSL", position, textSize, textColor, 0.0f);

    position.x = xSecondStore;
    text->RenderLeftAlinedText(std::to_wstring(fcs->StandardMissileCount()), position, textSize, textColor, 0.0f);
    position.x = xStore;


    position.y -= winY;
    text->RenderText(L"SPCL", position, textSize, textColor, 0.0f);

    position.x = xSecondStore;
    text->RenderLeftAlinedText(std::to_wstring(fcs->UniqueMissileCount()), position, textSize, textColor, 0.0f);
    position.x = xStore;


    position.y -= winY * 1.2f;
    text->RenderText(L"FLR", position, textSize, textColor, 0.0f);

    position.x = xSecondStore;
    text->RenderLeftAlinedText(std::to_wstring(6), position, textSize, textColor, 0.0f);
    position.x = xStore;


    position.y -= winY * 1.2f;
    text->RenderText(L"DMG", position, textSize, textColor, 0.0f);

    float prograss = rmwr->Health() / rmwr->MAXHealth();
    position.x = xSecondStore;
    text->RenderLeftAlinedText(std::to_wstring(static_cast<UINT>(prograss * 100.0f)) + L"%", position, textSize, textColor, 0.0f);
    position.x = xStore;


    position.y -= winY * 1.2f + scale.y * 0.5f;
    position.x += scale.x * 0.25f;



    DirectX::XMStoreFloat4x4(&worldMatrix, CreateMatrix(position, Vector2::one(), 0.0f));
    ::SetWorldMatrix(worldMatrix);
    ::SetTexture(diffuseTexture, aircraft.texture);
    shader->Render(aircraft.indexBuffer, aircraft.vertexBuffer, stride);
    dxDeviceContext->DrawIndexed(aircraft.indexCount, 0, 0);

    shader->PassNumber(1);

    shader->BindVariable("prograss", &prograss, sizeof(FLOAT));
    ::SetTexture(diffuseTexture, aircraftInside.texture);
    shader->Render(aircraftInside.indexBuffer, aircraftInside.vertexBuffer, stride);
    dxDeviceContext->DrawIndexed(aircraftInside.indexCount, 0, 0);
    //GetCurrentShader()->BindVariable("prograss", &fcs->CoolDownRStandardMissile(), sizeof(FLOAT));
    shader->PassNumber(0);

    if (fcs->WeaponSelectedStatus() == 0)
    {
        position.x -= scale.x * 0.75f;
        RenderMissileStatus(standardMissile, standardMissileInside, fcs->CoolDownLStandardMissile(), position);

        position.x += scale.x * 0.75f * 2.0f;
        RenderMissileStatus(standardMissile, standardMissileInside, fcs->CoolDownRStandardMissile(), position);
    }
    else
    {
        //Vector2 center = position;
        //FLOAT sign = 1.0f;
        //for (size_t i = 0; i < fcs->UniqueMissileCoolTime().size(); i++)
        //{
        //    sign = (1.0f - 3.0f * static_cast<FLOAT>(i % 2));
        //    position.x -= (scale.x * 0.75f * sign + scale.x * 0.1f * static_cast<FLOAT>(i / 2) * std::abs(sign) / sign);
        //
        //}


        position.x -= scale.x * 0.75f;
        RenderMissileStatus(uniqueMissile, uniqueMissileInside, fcs->UniqueMissileCoolTime()[0].first, position);

        position.x += scale.x * 0.75f * 2.0f;
        RenderMissileStatus(uniqueMissile, uniqueMissileInside, fcs->UniqueMissileCoolTime()[1].first, position);


    }



    if (missionEnd)
    {
        shader->PassNumber(5);
        static std::string uiColorName = "UIcolor";
        static float4 green = float4{0.0f, 1.0f, 0.0f, 1.0f};
        GetCurrentShader()->BindVariable(uiColorName, &green, sizeof(DirectX::XMFLOAT4));
        DirectX::XMStoreFloat4x4(&worldMatrix, CreateMatrix(Vector2::zero(), Vector2{ 5.0f,3.0f }, 0.0f));
        ::SetMatrix(world, worldMatrix);

        shader->BindTexture(diffuseTexture, missionEndParts.texture);
        shader->Render(missionEndParts.indexBuffer, missionEndParts.vertexBuffer, stride);
        dxDeviceContext->DrawIndexed(missionEndParts.indexCount, 0, 0);

        text->RenderText(L"MISSION SUCCESS", Vector2::zero(), textSize, textColor, 0.0f, true);
        shader->PassNumber(0);
    }

}

void PlayerStatusHeadUpDisplay::LinkData(Player* player)
{
    PlayerPilot* pilot = static_cast<PlayerPilot*>(player->GetComponent(L"PlayerPilot"));
    if (pilot == nullptr)
        return;
    metaData = &pilot->LinkObjectInfomation().aircraftInfomation;
    fcs = static_cast<FCS*>(player->GetComponent(L"FCS"));
    rmwr = static_cast<RMWR*>(player->GetComponent(L"RMWR"));
}

void PlayerStatusHeadUpDisplay::RenderMissileStatus(UIParts& parts, UIParts& innerParts, FLOAT coolTime, Vector2& position)
{
    Matrix worldMatrix;
    DirectX::XMStoreFloat4x4(&worldMatrix, CreateMatrix(position, Vector2::one(), 0.0f));
    SetWorldMatrix(worldMatrix);
    SetTexture(diffuseTexture, parts.texture);
    GetCurrentShader()->Render(parts.indexBuffer, parts.vertexBuffer, stride);
    dxDeviceContext->DrawIndexed(parts.indexCount, 0, 0);

    GetCurrentShader()->PassNumber(1);

    float prograss = (coolTime < 0) ? 1.0f : 1.0f - coolTime;
    GetCurrentShader()->BindVariable("prograss", &prograss, sizeof(FLOAT));
    SetTexture(diffuseTexture, innerParts.texture);
    GetCurrentShader()->Render(innerParts.indexBuffer, innerParts.vertexBuffer, stride);
    dxDeviceContext->DrawIndexed(innerParts.indexCount, 0, 0);
    GetCurrentShader()->PassNumber(0);
}
