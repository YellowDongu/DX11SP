#include "pch.h"
#include "PlayerStatusHUD.h"
#include "AircraftMetaData.h"
#include "Player.h"
#include "FireControlSystem.h"
#include "RMWR.h"

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

    innerErrorCheck(aircraft.LoadUITexture(L"../Bin/Resources/UI/HUD/PrograssBar/Aircraft/hud_acIcon_su33.png"), L"Load texture - hud_acIcon_su33.png");
    CreateScale(aircraft.texture, scale);
    innerErrorCheck(CreateVertex(aircraft.vertexBuffer, aircraft.indexBuffer, aircraft.indexCount, scale), L"Vertex/Index Create");

    innerErrorCheck(aircraftInside.LoadUITexture(L"../Bin/Resources/UI/HUD/PrograssBar/Aircraft/hud_acIcon_su33_inside.png"), L"Load texture - hud_acIcon_su33_inside.png");
    CreateScale(aircraftInside.texture, scale);
    innerErrorCheck(CreateVertex(aircraftInside.vertexBuffer, aircraftInside.indexBuffer, aircraftInside.indexCount, scale), L"Vertex/Index Create");
    

    text = CreateText(L"../Bin/ACES07.spritefont");
    if (text == nullptr)
        return E_FAIL;

    textSize = Vector2::one();
    return S_OK;
}

HRESULT PlayerStatusHeadUpDisplay::Awake(void)
{
    return E_NOTIMPL;
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



    DirectX::XMStoreFloat4x4(&worldMatrix, CreateMatrix(position, Vector2::one(), 180.0f));
    SetWorldMatrix(worldMatrix);
    SetTexture(diffuseTexture, aircraft.texture);
    GetCurrentShader()->Render(aircraft.indexBuffer, aircraft.vertexBuffer, stride);
    dxDeviceContext->DrawIndexed(aircraft.indexCount, 0, 0);

    GetCurrentShader()->PassNumber(1);

    GetCurrentShader()->BindVariable("prograss", &prograss, sizeof(FLOAT));
    SetTexture(diffuseTexture, aircraftInside.texture);
    GetCurrentShader()->Render(aircraftInside.indexBuffer, aircraftInside.vertexBuffer, stride);
    dxDeviceContext->DrawIndexed(aircraftInside.indexCount, 0, 0);
    //GetCurrentShader()->BindVariable("prograss", &fcs->CoolDownRStandardMissile(), sizeof(FLOAT));
    GetCurrentShader()->PassNumber(0);

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
        RenderMissileStatus(standardMissile, standardMissileInside, fcs->UniqueMissileCoolTime()[0].first, position);

        position.x += scale.x * 0.75f * 2.0f;
        RenderMissileStatus(standardMissile, standardMissileInside, fcs->UniqueMissileCoolTime()[1].first, position);


    }
}

void PlayerStatusHeadUpDisplay::LinkData(Player* player)
{
    metaData = &player->AircraftData();
    fcs = static_cast<FCS*>(player->GetComponent(L"FCS"));
    rmwr = static_cast<RMWR*>(player->GetComponent(L"RMWR"));
}

void PlayerStatusHeadUpDisplay::RenderMissileStatus(UIParts& parts, UIParts& innerParts, FLOAT coolTime, Vector2& position)
{
    Matrix worldMatrix;
    DirectX::XMStoreFloat4x4(&worldMatrix, CreateMatrix(position, Vector2::one(), 180.0f));
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
