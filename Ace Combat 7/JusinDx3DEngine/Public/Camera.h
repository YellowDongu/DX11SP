#pragma once
#include "Component.h"

namespace Engine
{
	class Transform;
	class ENGINEDLL Camera : public Component
	{
	private:
		Camera(void) = delete;
		Camera(ID3D11Device*& dxDevice, ID3D11DeviceContext*& dxDeviceContext);
		Camera(const Camera& other) = delete;
		virtual ~Camera(void) = default;
		virtual void Free(void);
	public:
		static Camera* Create(ID3D11Device*& dxDevice, ID3D11DeviceContext*& dxDeviceContext, Transform*& transformComponent, float fov = 45.0f, float aspectRatio = 1.3f, float nearZ = 0.1f, float farZ = 1000.0f);
		virtual Component* Clone(void) override;

		HRESULT SetCamera(float fov, float aspectRatio, float nearZ , float farZ);

		void Update(void);
		void Render(void);
		void Render(const Matrix& matrix);

		void LinkTransform(Transform*& transformComponent);

		Vector3 Look(void) { return lookAt.normalize(); }
		Vector3 Up(void) { return up; }
		FLOAT FOV(void) { return fov; }
		FLOAT AspectRaito(void) { return aspectRatio; }
		FLOAT NearZ(void) { return nearZ; }
		FLOAT FarZ(void) { return farZ; }

		void FOV(FLOAT value) { fov = value; }
		void AspectRaito(FLOAT value) { aspectRatio = value; }
		void NearZ(FLOAT value) { nearZ = value; }
		void FarZ(FLOAT value) { farZ = value; }


	private:
		FLOAT fov, aspectRatio, nearZ, farZ;
		Vector3* position, lookAt, up, focusPosition;
		const Vector3* angle;
		Matrix viewProjectionMatrix;

		Transform* transformComponent;
	};

}
