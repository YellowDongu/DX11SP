#include "Foundation.h"
#include "RigidBody.h"

using namespace Engine;

void RigidBody::CurrentVelocity(Vector3& velocity, const Vector3& attitude, float deltaTime, float mass, float gravity, float drag, float lift, float thrust)
{
    float velocityScalar = velocity.magnitude();

    if (velocityScalar > 0.0f)
    {
        float vSquare = velocityScalar * velocityScalar;

        Vector3 upVector, directionVector = AnglesToDirection(attitude);
        DirectX::XMStoreFloat3(&upVector, DirectX::XMVector3Cross(DirectX::XMVectorSet(directionVector.x, directionVector.y, directionVector.z, 0.0f), DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f)));
        upVector.normalize();
        directionVector.normalize();

        upVector *= lift * vSquare;
        Vector3 thrustVector = directionVector * thrust;
        Vector3 dragVector = directionVector * -1.0f * drag * vSquare;
        Vector3 gravityVector = Vector3{ 0.0f,-1.0f,0.0f } * gravity * mass;

        velocity += (upVector + thrustVector + dragVector + gravityVector) * deltaTime;
    }
    else
    {
        velocity = Vector3::zero();
    }
}

void RigidBody::input(const Vector3& input, float rollSpeed, float yawSpeed, float pitchSpeed)
{






}
