#pragma once
#include "MultiObjectBuffer.h"

namespace Engine
{
	struct ParticleMatrix
	{
		static const std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayout;
	};


	class ParticleBuffer : public MultiObjectBuffer
	{
	private:
		ParticleBuffer(void) = delete;
		ParticleBuffer(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
		ParticleBuffer(const ParticleBuffer& other);
		virtual ~ParticleBuffer(void) = default;
		virtual void Free(void) override;
	public:
		static ParticleBuffer* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
		virtual Component* Clone(void) override;

		HRESULT Start(void);

		virtual void Update(void) override;
		virtual void LateUpdate(void) override;
		virtual void FixedUpdate(void) override;

		HRESULT CreateBuffers(void);
		HRESULT CreateWorldBuffer(ID3D11Device* dxDevice, ID3D11Buffer*& vertexBuffer, ID3D11Buffer*& indexBuffer, std::vector<ParticleMatrix>& data);
		HRESULT CreateIndexBuffer(ID3D11Device* dxDevice, ID3D11Buffer*& indexBuffer, UINT instanceNumber, std::vector<UINT>& indices);
		HRESULT CreateVertexBuffer(ID3D11Device* dxDevice, ID3D11Buffer*& indexBuffer);
		HRESULT CreateWorldBuffer(ID3D11Device* dxDevice, ID3D11Buffer*& worldBuffer, UINT instanceNumber, std::vector<ModelVertex>& vertices);
		
	private:

		D3D11_BUFFER_DESC bufferDescription;

		ID3D11Buffer* worldBuffer;
	};

}