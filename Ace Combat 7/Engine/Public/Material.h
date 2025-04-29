#pragma once
#include "Base.h"

namespace Engine
{
	class Material : public Base
	{
	private:
		Material(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
		Material(void);
		virtual ~Material(void) = default;
		virtual void Free(void) override;
	public:
		static Material* Create(ModelConverter::FullModelMaterial& material, const std::wstring& modelPath);
		
		HRESULT GetTexture(std::wstring key, std::wstring path, ID3D11ShaderResourceView*& texture);
		HRESULT LoadMaterial(ModelConverter::FullModelMaterial& material, const std::wstring& modelPath);
		HRESULT SetMaterial(ID3D11DeviceContext*& context);

		#ifdef useAssimp
		static Material* Create(aiMaterial* material, const std::wstring& modelPath);
		HRESULT LoadMaterial(aiMaterial* material, const std::wstring& modelPath);
		#endif // useAssimp

	private:
        float4 diffuseColor;		// 디퓨즈 색상
        float4 specularColor;		// 스펙큘러 색상
        FLOAT shininess;			// 반사도(Shininess)

        ID3D11ShaderResourceView* diffuseTexture;	// 디퓨즈 텍스처
        ID3D11ShaderResourceView* specularTexture;	// 스펙큘러 텍스처
        ID3D11ShaderResourceView* normalTexture;	// 스펙큘러 텍스처
        int hasSpecularTexture;						// 1이면 specularTexture 있음, 0이면 없음

		MaterialBufferType buffer;
	};
}
