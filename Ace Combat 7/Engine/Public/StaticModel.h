#pragma once
#include "Component.h"
#include "Mesh.h"
#include "Material.h"

namespace Engine
{
	class ModelLoader;
	class ENGINEDLL StaticModel : public Component
	{
	private:
		StaticModel(void) = delete;
		StaticModel(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContex);
		StaticModel(const StaticModel& other);
		virtual ~StaticModel(void) = default;
		virtual void Free(void) override;
	public:
		static StaticModel* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, const std::wstring filePath, ModelLoader modelLoader, bool saveVertex = false);
		virtual Component* Clone(void) override { return new StaticModel(*this); };

		void SetBoneMatrix(void);
		virtual void Render(void);

		HRESULT LoadModel(const std::wstring& filePath, ModelLoader& modelLoader, bool saveVertex = false);
		void SetOffsetMatrix(const Matrix& matrix) { memcpy(&offsetMatrix, &matrix, sizeof(Matrix)); }
		void SetOffsetMatrix(fxmMatrix matrix) { DirectX::XMStoreFloat4x4(&offsetMatrix, matrix); }
		bool RayToMeshCollision(const Vector3& rayPosition, const Vector3& rayDirection, Vector3& outHitPoint);

		#ifdef useAssimp
		static StaticModel* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContex, const std::string filePath);
		HRESULT LoadModel(const std::string& filePath);
		#endif

	private:
		bool origin = false, vertexSaved = false;
		std::vector<Mesh*> submesh;
		std::vector<Material*> materials;
		Matrix offsetMatrix, baseMatrix;
	};

}
