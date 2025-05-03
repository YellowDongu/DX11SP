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
		Camera(const Camera& other);
		virtual ~Camera(void) = default;
		virtual void Free(void) override;
	public:
		static Camera* Create(ID3D11Device*& dxDevice, ID3D11DeviceContext*& dxDeviceContext, float fov = 45.0f, float aspectRatio = 1.3f, float nearZ = 0.1f, float farZ = 1000.0f);
		virtual Component* Clone(void) override;

		HRESULT SetCamera(float fov, float aspectRatio, float nearZ , float farZ);

		virtual void LateUpdate(void) override;
		void Render(void);
		void Render(const Matrix& matrix);

		const Matrix& ViewProjectionMatrix(void) const { return viewProjectionMatrix; }
		xmMatrix ViewProjectionxmMatrix(void) const { return DirectX::XMLoadFloat4x4(&viewProjectionMatrix); }
		xmMatrix ProjectionxmMatrix(void) const { return DirectX::XMLoadFloat4x4(&projectionMatrix); }
		const Matrix& ProjectionMatrix(void) const { return projectionMatrix; }
		

		Vector3 Look(void) { return lookAt.normalize(); }
		Vector3 Up(void) { return up; }
		FLOAT FOV(void) { return fov; }
		FLOAT AspectRaito(void) { return aspectRatio; }
		FLOAT NearZ(void) { return nearZ; }
		FLOAT FarZ(void) { return farZ; }

		void FOV(FLOAT value) { SetCamera(value, aspectRatio, nearZ, farZ); }
		void AspectRaito(FLOAT value) { SetCamera(fov, value, nearZ, farZ); }
		void NearZ(FLOAT value) { SetCamera(fov, aspectRatio, value, farZ); }
		void FarZ(FLOAT value) { SetCamera(fov, aspectRatio, nearZ, value); }


	private:
		FLOAT fov, aspectRatio, nearZ, farZ;
		Vector3* position, lookAt, up, focusPosition;
		const Vector3* angle;
		Matrix viewProjectionMatrix, projectionMatrix;
	};

}
