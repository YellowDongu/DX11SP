#pragma once
#include "Base.h"

namespace Engine
{
	struct Font
	{
		DirectX::SpriteBatch* batch{ nullptr };
		DirectX::SpriteFont* font{ nullptr };
	};


	class ENGINEDLL Text : public Base
	{
	private:
		Text(void) = delete;
		Text(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
		Text(const Text& other);
		virtual ~Text(void) = default;
		virtual void Free(void) override;
	public:
		static Text* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, Font* font, const D3D11_VIEWPORT* viewPortSetting);

		void ChangeFont(Font* font) { font = currentFont; }
		Vector2 ConvertPosition(float3 worldPos, float screenWidth, float screenHeight)
		{
			return Vector2{(worldPos.x + 1.0f) * 0.5f * screenWidth, (1.0f - worldPos.y) * 0.5f * screenHeight};
			Vector2 screenPos;
			screenPos.x = (worldPos.x + 1.0f) * 0.5f * screenWidth;
			screenPos.y = (1.0f - worldPos.y) * 0.5f * screenHeight;
			return screenPos;
		}
		Vector2 ConvertPosition(float2 worldPos, float screenWidth, float screenHeight)
		{
			float2 screenPos;
			screenPos.x = (worldPos.x + screenWidth / 2.0f);   // 중앙 기준으로 이동
			screenPos.y = (-worldPos.y + screenHeight / 2.0f); // Y축 뒤집기
			return screenPos;
		}

		HRESULT RenderText(const wchar_t* text, float2 position, FLOAT scale, float4 color, FLOAT rotation, bool centerAllign = false);
		HRESULT RenderText(const std::wstring& text, float2 position, FLOAT scale, float4 color, FLOAT rotation, bool centerAllign = false);
		HRESULT RenderText(const wchar_t* text, float2 position, float2 scale, float4 color, FLOAT rotation, bool centerAllign = false);
		HRESULT RenderText(const std::wstring& text, float2 position, float2 scale, float4 color, FLOAT rotation, bool centerAllign = false);
		HRESULT RenderLeftAlinedText(const wchar_t* text, float2 position, float2 scale, float4 color, FLOAT rotation);
		HRESULT RenderLeftAlinedText(const std::wstring& text, float2 position, float2 scale, float4 color, FLOAT rotation);
	private:
		ID3D11Device* dxDevice;
		ID3D11DeviceContext* dxDeviceContext;
		const D3D11_VIEWPORT* viewportSetting;
		Font* currentFont;
	};

	class TextMaster : public Base
	{
	private:
		TextMaster(void) = delete;
		TextMaster(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, const D3D11_VIEWPORT& viewportSetting);
		TextMaster(const TextMaster&) = delete;
		virtual ~TextMaster(void) = default;
		virtual void Free(void);
	public:
		static TextMaster* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, const D3D11_VIEWPORT& viewportSetting);
		Text* CreateText(Font* font);
		Text* CreateText(std::wstring fontName);

		HRESULT LoadFont(const std::wstring& filePath);
		HRESULT ChangeFont(const std::wstring& fontKey, Text& text);
	private:
		ID3D11Device* dxDevice;
		ID3D11DeviceContext* dxDeviceContext;
		std::map<std::wstring, Font> fonts;
		Font* currentFont;

		const D3D11_VIEWPORT* viewportSetting;
	};
}