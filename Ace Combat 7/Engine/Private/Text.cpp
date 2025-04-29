#include "Foundation.h"
#include "Text.h"


using namespace Engine;


Text::Text(ID3D11Device* _dxDevice, ID3D11DeviceContext* _dxDeviceContext) :  dxDevice(_dxDevice), dxDeviceContext(_dxDeviceContext), currentFont(nullptr)
{
}


Text* Text::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, Font* font, const D3D11_VIEWPORT* viewPortSetting)
{
	if (font == nullptr)
		return nullptr;
	Text* newInstance = new Text(dxDevice, dxDeviceContext);
	newInstance->currentFont = font;
	newInstance->viewportSetting = viewPortSetting;
	//if (FAILED(newInstance->Start()))
	//{
	//	Base::Destroy(newInstance);
	//	return nullptr;
	//}
	return newInstance;
}


void Text::Free(void)
{
}


HRESULT Text::RenderText(const wchar_t* text, float2 position, FLOAT scale, float4 color, FLOAT rotation, bool centerAllign)
{
	if (currentFont == nullptr || currentFont->font == nullptr || currentFont->batch == nullptr)
		return E_FAIL;

	position = ConvertPosition(position, viewportSetting->Width, viewportSetting->Height);

	dxDeviceContext->GSSetShader(nullptr, nullptr, 0);

	currentFont->batch->Begin();

	Vector2 vOrigin(Vector2::zero());
	if (centerAllign)
	{
		xmVector vTextSize = currentFont->font->MeasureString(text);
		XMStoreFloat2(&vOrigin, DirectX::XMVectorDivide(vTextSize, DirectX::XMVectorReplicate(2.0f)));
	}
	currentFont->font->DrawString(currentFont->batch, text, position, DirectX::XMLoadFloat4(&color), rotation, vOrigin, scale);

	currentFont->batch->End();

	return S_OK;
}

HRESULT Text::RenderText(const std::wstring& text, float2 position, FLOAT scale, float4 color, FLOAT rotation, bool centerAllign)
{
	if (currentFont == nullptr || currentFont->font == nullptr || currentFont->batch == nullptr)
		return E_FAIL;

	position = ConvertPosition(position, viewportSetting->Width, viewportSetting->Height);

	dxDeviceContext->GSSetShader(nullptr, nullptr, 0);

	currentFont->batch->Begin();

	Vector2 vOrigin(Vector2::zero());
	if (centerAllign)
	{
		xmVector vTextSize = currentFont->font->MeasureString(text.c_str());
		XMStoreFloat2(&vOrigin, DirectX::XMVectorDivide(vTextSize, DirectX::XMVectorReplicate(2.0f)));
	}
	currentFont->font->DrawString(currentFont->batch, text.c_str(), position, DirectX::XMLoadFloat4(&color), rotation, vOrigin, scale);

	currentFont->batch->End();

	return S_OK;
}

HRESULT Text::RenderText(const wchar_t* text, float2 position, float2 scale, float4 color, FLOAT rotation, bool centerAllign)
{
	if (currentFont == nullptr || currentFont->font == nullptr || currentFont->batch == nullptr)
		return E_FAIL;

	position = ConvertPosition(position, viewportSetting->Width, viewportSetting->Height);

	dxDeviceContext->GSSetShader(nullptr, nullptr, 0);

	currentFont->batch->Begin();

	Vector2 vOrigin(Vector2::zero());
	if (centerAllign)
	{
		xmVector vTextSize = currentFont->font->MeasureString(text);
		XMStoreFloat2(&vOrigin, DirectX::XMVectorDivide(vTextSize, DirectX::XMVectorReplicate(2.0f)));
	}
	currentFont->font->DrawString(currentFont->batch, text, position, DirectX::XMLoadFloat4(&color), rotation, vOrigin, scale);

	currentFont->batch->End();

	return S_OK;
}

HRESULT Text::RenderText(const std::wstring& text, float2 position, float2 scale, float4 color, FLOAT rotation, bool centerAllign)
{
	if (currentFont == nullptr || currentFont->font == nullptr || currentFont->batch == nullptr)
		return E_FAIL;

	position = ConvertPosition(position, viewportSetting->Width, viewportSetting->Height);

	dxDeviceContext->GSSetShader(nullptr, nullptr, 0);

	currentFont->batch->Begin();

	Vector2 vOrigin(Vector2::zero());
	if (centerAllign)
	{
		xmVector vTextSize = currentFont->font->MeasureString(text.c_str());
		XMStoreFloat2(&vOrigin, DirectX::XMVectorDivide(vTextSize, DirectX::XMVectorReplicate(2.0f)));
	}
	currentFont->font->DrawString(currentFont->batch, text.c_str(), position, DirectX::XMLoadFloat4(&color), rotation, vOrigin, scale);

	currentFont->batch->End();

	return S_OK;
}

HRESULT Text::RenderLeftAlinedText(const wchar_t* text, float2 position, float2 scale, float4 color, FLOAT rotation)
{
	if (currentFont == nullptr || currentFont->font == nullptr || currentFont->batch == nullptr)
		return E_FAIL;

	position = ConvertPosition(position, viewportSetting->Width, viewportSetting->Height);

	dxDeviceContext->GSSetShader(nullptr, nullptr, 0);

	currentFont->batch->Begin();

	Vector2 origin(Vector2::zero());

	XMStoreFloat2(&origin, currentFont->font->MeasureString(text));
	origin.y = 0.0f;

	currentFont->font->DrawString(currentFont->batch, text, position, DirectX::XMLoadFloat4(&color), rotation, origin, scale);

	currentFont->batch->End();

	return S_OK;
}

HRESULT Text::RenderLeftAlinedText(const std::wstring& text, float2 position, float2 scale, float4 color, FLOAT rotation)
{
	if (currentFont == nullptr || currentFont->font == nullptr || currentFont->batch == nullptr)
		return E_FAIL;

	position = ConvertPosition(position, viewportSetting->Width, viewportSetting->Height);

	dxDeviceContext->GSSetShader(nullptr, nullptr, 0);

	currentFont->batch->Begin();

	Vector2 origin(Vector2::zero());

	XMStoreFloat2(&origin, currentFont->font->MeasureString(text.c_str()));
	origin.y = 0.0f;

	currentFont->font->DrawString(currentFont->batch, text.c_str(), position, DirectX::XMLoadFloat4(&color), rotation, origin, scale);

	currentFont->batch->End();

	return S_OK;
}

TextMaster::TextMaster(ID3D11Device* _dxDevice, ID3D11DeviceContext* _dxDeviceContext, const D3D11_VIEWPORT& _viewportSetting) : dxDevice(_dxDevice), dxDeviceContext(_dxDeviceContext), currentFont(nullptr), viewportSetting(&_viewportSetting)
{
}

void TextMaster::Free(void)
{
	for (auto& font : fonts)
	{
		if (font.second.batch != nullptr)
			delete font.second.batch;
		if (font.second.font != nullptr)
			delete font.second.font;
	}
	fonts.clear();
}

TextMaster* TextMaster::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, const D3D11_VIEWPORT& viewportSetting)
{
	TextMaster* newInstance = new TextMaster(dxDevice, dxDeviceContext, viewportSetting);
	return newInstance;
}

Text* TextMaster::CreateText(Font* font)
{
	Text* newInstance = Text::Create(dxDevice,  dxDeviceContext, font, viewportSetting);
	return newInstance;
}

Text* TextMaster::CreateText(std::wstring fontName)
{
	auto font = fonts.find(fontName);
	if (font == fonts.end())
		return nullptr;

	Text* newInstance = Text::Create(dxDevice, dxDeviceContext, &font->second, viewportSetting);
	return newInstance;
}

HRESULT TextMaster::LoadFont(const std::wstring& filePath)
{
	auto font = fonts.find(filePath);
	if (font != fonts.end())
		return S_OK;

	Font newFont;
	newFont.batch = new DirectX::SpriteBatch(dxDeviceContext);
	newFont.font = new DirectX::SpriteFont(dxDevice, filePath.c_str());

	fonts[filePath] = newFont;
	return S_OK;
}

HRESULT TextMaster::ChangeFont(const std::wstring& fontKey, Text& text)
{
	auto font = fonts.find(fontKey);
	if (font == fonts.end())
		return E_FAIL;

	text.ChangeFont(&font->second);
	return S_OK;
}
