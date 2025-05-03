#pragma once
#include "Shader.h"

namespace Engine
{
	class DefaultDeferredShader : public Shader
	{
	private:
		DefaultDeferredShader(void) = delete;
		DefaultDeferredShader(ID3D11Device*& device, ID3D11DeviceContext*& context);
		DefaultDeferredShader(const DefaultDeferredShader& other) = delete;
		virtual ~DefaultDeferredShader(void) = default;
		virtual void Free(void) override;
	public:
		static DefaultDeferredShader* Create(ID3D11Device*& device, ID3D11DeviceContext*& context);

		virtual void SetNamePreset(void) override;
	private:

	};
}

