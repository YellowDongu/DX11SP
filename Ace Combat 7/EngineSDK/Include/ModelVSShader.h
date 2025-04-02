#pragma once
#include "Shader.h"

namespace Engine
{
	class ENGINEDLL DefaultModelShader : public Shader
	{
	protected:
		DefaultModelShader(void) = default;
		DefaultModelShader(ID3D11Device*& device, ID3D11DeviceContext*& context);
		DefaultModelShader(const DefaultModelShader& other) = delete;
		virtual ~DefaultModelShader(void) = default;
		virtual void Free(void) override;
	public:
		static DefaultModelShader* Create(ID3D11Device*& device, ID3D11DeviceContext*& context);

		virtual HRESULT CreateBuffers(ID3D11Device* device, std::map<std::wstring, ID3D11Buffer*>& constantBuffers);
		virtual void SetNamePreset(void) override;
	private:


	};
}
typedef ENGINEDLL Engine::DefaultModelShader DefaultModelShader;