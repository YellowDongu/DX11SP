#pragma once

#include "Shader.h"

namespace Engine
{
	class ENGINEDLL DefaultParticleShader : public Shader
	{
	private:
		DefaultParticleShader(void) = delete;
		DefaultParticleShader(ID3D11Device*& device, ID3D11DeviceContext*& context);
		DefaultParticleShader(const DefaultParticleShader& other) = delete;
		virtual ~DefaultParticleShader(void) = default;
		virtual void Free(void) override;
	public:
		static DefaultParticleShader* Create(ID3D11Device*& device, ID3D11DeviceContext*& context);
		virtual void SetNamePreset(void) override;
		virtual HRESULT CreateBuffers(ID3D11Device* device, std::map<std::wstring, ID3D11Buffer*>& constantBuffers) override;
	private:

	};
}

