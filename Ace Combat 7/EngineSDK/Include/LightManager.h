#pragma once
#include "Base.h"

namespace Engine
{
	enum class LightType
	{
		Directional,
		Point,
		Spot,
		END
	};

	struct Light
	{
		LightType type;
		FLOAT range;
		float4 direction;
		float4 position;

		float4 diffuseColor;
		float4 ambientColor;
		float4 specularColor;
	};


	class LightManager : public Base
	{
	private:
		LightManager(void) = delete;
		LightManager(ID3D11Device* device, ID3D11DeviceContext* context);
		virtual ~LightManager(void) = default;
		virtual void Free(void);
	public:
		static LightManager* Create(ID3D11Device* device, ID3D11DeviceContext* context);

		HRESULT Add_Light(Light& lightStruct, const std::wstring& key);

		HRESULT BindLight(class Shader* shader, Light& light);
		HRESULT BindWorldLight(class Shader* shader);
	private:
		std::map<std::wstring, Light> lights;
		Light worldLights;

		ID3D11Device* device;
		ID3D11DeviceContext* context;
	};
}