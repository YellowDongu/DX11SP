#pragma once
#include "Transform.h"

namespace Engine
{
	class RigidBody : public Transform
	{
	public:
		RigidBody(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
		RigidBody(const RigidBody& other);
		~RigidBody(void) = default;

	private:
		Vector3 velocity;
		float mass;
		float gravity;
		


		// 항공역학용

	public:
		float thrust;
		float drag;
		float lift;

		float rollSpeed;
		float yawSpeed;
		float pitchSpeed;


		void CurrentVelocity(Vector3& velocity, const Vector3& attitude, float deltaTime, float mass, float gravity, float drag, float lift, float thrust);

		void input(const Vector3& input, float rollSpeed, float yawSpeed, float pitchSpeed);
	};

}