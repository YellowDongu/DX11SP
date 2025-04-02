#pragma once
#include "Shader.h"

class UIShader : public Engine::Shader
{
private:
	UIShader(void) = default;
	UIShader(ID3D11Device* device, ID3D11DeviceContext* context);
	UIShader(const Shader& other) = delete;
	virtual ~UIShader(void) = default;
	virtual void Free(void) override;
public:
	static UIShader* Create(ID3D11Device* device, ID3D11DeviceContext* context);
	virtual void SetNamePreset(void);
private:





};
