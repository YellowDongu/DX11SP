#pragma once
#include "Base.h"

struct Font
{
	DirectX::SpriteBatch* batch{ nullptr };
	DirectX::SpriteFont* font{ nullptr };


	//Font* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
	//{
	//	DirectX::
	//}
};


namespace Engine
{
	class TextureManager : public Base
	{
	public:
		TextureManager(void) = delete;
		TextureManager(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
		virtual ~TextureManager(void);
		virtual void Free(void);
	public:
		static TextureManager* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);

		HRESULT GetTexture(const std::wstring& key, ID3D11ShaderResourceView*& texture);
		HRESULT SetTexture(const std::wstring& key, const std::wstring& path);

	private:
		ID3D11Device* device;
		ID3D11DeviceContext* context;

		std::unordered_map<std::wstring, ID3D11ShaderResourceView*> textures;
	};
}