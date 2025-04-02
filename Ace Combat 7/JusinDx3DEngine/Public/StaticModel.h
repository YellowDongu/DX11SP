#pragma once
#include "Component.h"
#include "Mesh.h"
#include "Material.h"

namespace Engine
{
	class ENGINEDLL StaticModel : public Component
	{
	private:
		StaticModel(void) = delete;
		StaticModel(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContex);
		StaticModel(const StaticModel& other);
		virtual ~StaticModel(void) = default;
		virtual void Free(void) override;
		virtual Component* Clone(void) override { return new StaticModel(*this); };
	public:
		static StaticModel* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContex, const std::string filePath);

		//void Start(void);
		//void Update(void);
		//virtual void Render(void) override;
		virtual void Render(void);

		HRESULT LoadModel(const std::string& filePath);

	private:
		bool origin = false;
		std::vector<Mesh*> submesh;
		std::vector<Material*> materials;
	};

}
