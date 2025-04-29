#pragma once
#include "Shader.h"


namespace Engine
{
	class ENGINEDLL DefaultCubeShader final : public Shader
	{
	private:
		DefaultCubeShader(void) = delete;
		DefaultCubeShader(ID3D11Device*& device, ID3D11DeviceContext*& context);
		virtual ~DefaultCubeShader(void) = default;
		virtual void Free(void) override;
	public:
		static DefaultCubeShader* Create(ID3D11Device*& device, ID3D11DeviceContext*& context);


		virtual HRESULT CreateBuffers(ID3D11Device* device, std::map<std::wstring, ID3D11Buffer*>& constantBuffers);
		virtual void SetNamePreset(void);

	private:

	};

}