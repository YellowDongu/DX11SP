#pragma once
#include "Component.h"

namespace Engine
{
	class ENGINEDLL Texture : public Component
	{
	private:
		Texture(void) = delete;
		Texture(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
		Texture(const Texture& other);
		virtual ~Texture(void) = default;

		virtual void Free(void);
	public:
		static Texture* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
		virtual Component* Clone(void) override;

		HRESULT ChangeTexture(const std::wstring key);
		void Render(void);

	private:
		std::vector<ID3D11ShaderResourceView*> textures;
		
	};

}
