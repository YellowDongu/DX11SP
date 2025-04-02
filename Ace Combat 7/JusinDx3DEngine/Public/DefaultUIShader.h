#pragma once

#include "Shader.h"

namespace Engine
{
	class ENGINEDLL DefaultUIShader final : public Shader
	{
	private:
		DefaultUIShader(void) = delete; 
		DefaultUIShader(ID3D11Device* device, ID3D11DeviceContext* context);
		DefaultUIShader(const DefaultUIShader&) = delete;
		virtual ~DefaultUIShader(void) = default;
		virtual void Free(void) override;
	public:
		static DefaultUIShader* Create(ID3D11Device* device, ID3D11DeviceContext* context);

		//virtual HRESULT CreateBuffers(ID3D11Device* device, std::map<std::wstring, ID3D11Buffer*>& constantBuffers) override;
		virtual void SetNamePreset(void);
	private:

	};

}