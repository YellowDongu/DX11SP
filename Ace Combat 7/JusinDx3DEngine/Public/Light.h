#pragma once
#include "BaseObject.h"

enum class LightType
{
	Directional, 
	Point,
	Spot,
	END 
};

namespace Engine
{
	class Light : public BaseObject
	{
	private:
		Light(void) = delete;
		Light(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
		virtual ~Light(void) = default;
		virtual void Free(void)
		{

		}
	public:
		static Light* Create()
		{

		}

	private:
		LightType type;
		FLOAT range;
		float4 direction;
		float4 position;

		float4 diffuseColor;
		float4 ambientColor;
		float4 specularColor;

	};

}