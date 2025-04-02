#pragma once
#include "Component.h"
namespace Engine
{
	class ENGINEDLL Transform : public Component
	{
	protected:
		Transform(void) = delete;
		Transform(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
		Transform(const Transform& other);
		virtual ~Transform(void) = default;

		virtual void Free(void) override;
	public:
		static Transform* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
		virtual Component* Clone(void);

		virtual void LateUpdate(void);
		virtual void Render(void);

		void Translate(const float3& value) { ResetState(); position += value; }
		void Rotate(const float3 value);
		void RelativeRotate(const float3 value);
		void Rotate(const float3& axis, const float angleValue);

		Vector3 EulerAngle(DirectX::FXMVECTOR quaternion) { float4 quaternionVector; XMStoreFloat4(&quaternionVector, quaternion); return EulerAngle(quaternionVector); }
		Vector3 EulerAngle(const DirectX::XMFLOAT4& quaternion);
		void EulerAngle(DirectX::XMFLOAT3& angle, DirectX::FXMVECTOR quaternion) { float4 quaternionVector; XMStoreFloat4(&quaternionVector, quaternion); EulerAngle(angle, quaternionVector); }
		void EulerAngle(DirectX::XMFLOAT3& angle, const DirectX::XMFLOAT4& quaternion);

		void UpdateWorldMatrix(void) { CreateWorldMatrix(worldMatrix, position, angle, scale, right, up, forward); }
		DirectX::XMMATRIX CreateWorldMatrix(float4x4& worldMatrix, const Vector3& position, const Vector3& angle, const Vector3& scale);
		DirectX::XMMATRIX CreateWorldMatrix(float4x4& storage, const Vector3& position, const Vector3& angle, const Vector3& scale, Vector3& right, Vector3& up, Vector3& forward);
		Vector3 AnglesToDirection(Vector3 angle);
		Vector3 RadiansToDirection(Vector3 radian);
		void SetAngle(const float4& quaternion, Vector3& forward, bool& forwardCalculate, Vector3& right, bool& rightCalculate, Vector3& up, bool& upCalculate);
		void UpdateForward(DirectX::FXMVECTOR quaternion, Vector3& forward, bool& forwardCalculate);
		void UpdateUp(DirectX::FXMVECTOR quaternion, Vector3& up, bool& upCalculate);
		void UpdateRight(DirectX::FXMVECTOR quaternion, Vector3& right, bool& rightCalculate);
		inline void ResetState(void) { forwardCalculate = false; upCalculate = false; rightCalculate = false; matrixCalculate = false; }

		void SetAngle(DirectX::FXMVECTOR quaternion);
		void SetAngle(FLOAT x, FLOAT y, FLOAT z);
		void SetAngle(const Vector3 angleValue) { SetAngle(angleValue.x, angleValue.y, angleValue.z); }
		void SetAngle(const float4& quaternion) { SetAngle(DirectX::XMLoadFloat4(&quaternion)); }
		void LinkParent(Transform& object) { parentWorldMatrix = &object.worldMatrix; parent = &object; object.childs.push_back(this); }
		void LinkChild(Transform& object) { object.parentWorldMatrix = &worldMatrix; object.parent = this; childs.push_back(&object); }
		void UnLinkChild(UINT index);
		void UnLinkChild(Transform* object);
		void UnLinkParent(void) { if (parent == nullptr) return; parent->UnLinkChild(this); }
		void SetAngleForTemp(const Vector3& value) { angle = value; }

		Vector3& Position(void) { return position; }
		Vector3& Scale(void) { return scale; }

		const Vector3& Angle(void) const { return angle; }
		const float4& Quaternion(void) const { return quaternion; }
		const Matrix& WorldMatrix(void) const { return worldMatrix; }

		const Vector3& Forward(void) const { return forward; }
		const Vector3& Up(void) const { return up; }
		const Vector3& Right(void) const { return right; }

		Transform* GetParent(void) { return parent; }
		Transform* GetChild(UINT index) { if (index >= static_cast<UINT>(childs.size())) return nullptr; return childs[index]; }

	protected:
		bool forwardCalculate, upCalculate, rightCalculate, matrixCalculate;

		Vector3 position;
		Vector3 angle;
		Vector3 scale;
		float4 quaternion;

		Vector3 forward;
		Vector3 up;
		Vector3 right;

		const Matrix* parentWorldMatrix;
		Matrix worldMatrix;

		Transform* parent;
		std::vector<Transform*> childs;
	};
}

typedef Engine::Transform Transform;
