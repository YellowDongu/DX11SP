#include "Foundation.h"
#include "Transform.h"
#include "GameInstance.h"

using namespace Engine;
using namespace DirectX;


static Vector3 worldForward = { 0.0f, 0.0f, 1.0f };
static Vector3 worldUp = { 0.0f, 1.0f, 0.0f };
static Vector3 worldRight = { 1.0f, 0.0f, 0.0f };

Transform::Transform(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Component(dxDevice, dxDeviceContext), forwardCalculate(false), upCalculate(false), rightCalculate(false), matrixCalculate(false), position({0.0f,0.0f,0.0f}), angle({0.0f,0.0f,0.0f}), scale({1.0f,1.0f,1.0f}), worldMatrix{}, forward(worldForward), up(worldUp), right(worldRight), quaternion(), parent(nullptr), parentWorldMatrix(nullptr), childs{}
{
	XMStoreFloat4(&quaternion, XMQuaternionIdentity());
	XMStoreFloat4x4(&worldMatrix, XMMatrixIdentity());
}
Transform::Transform(const Transform& other) : Component(other), forwardCalculate(other.forwardCalculate), upCalculate(other.upCalculate), rightCalculate(other.rightCalculate), matrixCalculate(other.matrixCalculate), position(other.position), angle(other.angle), scale(other.scale), worldMatrix(other.worldMatrix), forward(other.forward), up(other.up), right(other.right), quaternion(other.quaternion), parent(nullptr), parentWorldMatrix(nullptr), childs{}
{
}

void Transform::Free(void)
{
}

Transform* Transform::Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext)
{
	Transform* newInstance = new Transform(dxDevice, dxDeviceContext);

	//if (FAILED(newInstance->Start()))
	//{
	//	Base::Destroy(newInstance);
	//	return nullptr;
	//}

	return newInstance;
}

Component* Transform::Clone(void)
{
	return new Transform(*this);
}

void Transform::LateUpdate(void)
{

}

void Transform::Render(void)
{
	UpdateWorldMatrix();
	SetMatrix(GetCurrentShader()->worldMatrixA, worldMatrix);
}

void Transform::Rotate(const float3 value)
{
	xmVector quaternionAfter = XMQuaternionMultiply(XMLoadFloat4(&quaternion), XMQuaternionRotationRollPitchYaw(XMConvertToRadians(value.x), XMConvertToRadians(value.y), XMConvertToRadians(value.z)));
	XMStoreFloat4(&quaternion, quaternionAfter);

	XMStoreFloat3(&right, DirectX::XMVector3Rotate(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), quaternionAfter));
	XMStoreFloat3(&up, DirectX::XMVector3Rotate(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), quaternionAfter));
	XMStoreFloat3(&forward, DirectX::XMVector3Rotate(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), quaternionAfter));

	EulerAngle(angle, quaternion);
}

void Transform::RelativeRotate(const float3 value)
{
	xmVector quaternionX = XMQuaternionRotationAxis(XMLoadFloat3(&right), value.x);
	xmVector quaternionY = XMQuaternionRotationAxis(XMLoadFloat3(&up), value.y);
	xmVector quaternionZ = XMQuaternionRotationAxis(XMLoadFloat3(&forward), value.z);

	xmVector quaternionAfter = XMQuaternionMultiply(XMLoadFloat4(&quaternion), XMQuaternionMultiply(quaternionZ, XMQuaternionMultiply(quaternionY, quaternionX)));
	XMStoreFloat4(&quaternion, quaternionAfter);

	XMStoreFloat3(&right, DirectX::XMVector3Rotate(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), quaternionAfter));
	XMStoreFloat3(&up, DirectX::XMVector3Rotate(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), quaternionAfter));
	XMStoreFloat3(&forward, DirectX::XMVector3Rotate(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), quaternionAfter));

	EulerAngle(angle, quaternion);
}

void Transform::Rotate(const float3& axis, const float angleValue)
{
	// 회전 행렬 생성
	XMVECTOR rotationQuaternion = XMQuaternionRotationAxis(XMLoadFloat3(&axis), angleValue);
	xmVector quaternionAfter = XMQuaternionMultiply(XMLoadFloat4(&quaternion), rotationQuaternion);
	XMStoreFloat4(&quaternion, quaternionAfter);

	XMStoreFloat3(&right, DirectX::XMVector3Rotate(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), quaternionAfter));
	XMStoreFloat3(&up, DirectX::XMVector3Rotate(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), quaternionAfter));
	XMStoreFloat3(&forward, DirectX::XMVector3Rotate(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), quaternionAfter));

	EulerAngle(angle, quaternion);
}

XMMATRIX Transform::CreateWorldMatrix(float4x4& storage, const Vector3& position, const Vector3& angle, const Vector3& scale)
{
	XMMATRIX worldMatrix = XMMatrixScaling(scale.x, scale.y, scale.z);
	worldMatrix *= XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYaw(XMConvertToRadians(angle.x), XMConvertToRadians(angle.y), XMConvertToRadians(angle.z)));
	worldMatrix.r[3] = XMVectorSet(position.x, position.y, position.z, 1.0f);
	XMStoreFloat4x4(&storage, worldMatrix);
	matrixCalculate = true;
	return worldMatrix;
}

XMMATRIX Transform::CreateWorldMatrix(float4x4& storage, const Vector3& position, const Vector3& angle, const Vector3& scale, Vector3& right, Vector3& up, Vector3& forward)
{
	XMMATRIX worldMatrix;
	worldMatrix.r[0] = XMVectorSet(right.x * scale.x, right.y * scale.x, right.z * scale.x, 0.0f);
	worldMatrix.r[1] = XMVectorSet(up.x * scale.y, up.y * scale.y, up.z * scale.y, 0.0f);
	worldMatrix.r[2] = XMVectorSet(forward.x * scale.z, forward.y * scale.z, forward.z * scale.z, 0.0f);
	worldMatrix.r[3] = XMVectorSet(position.x, position.y, position.z, 1.0f);

	if (parentWorldMatrix != nullptr)
	{
		worldMatrix = XMMatrixMultiply(XMLoadFloat4x4(parentWorldMatrix), worldMatrix);
	}

	XMStoreFloat4x4(&storage, worldMatrix);

	return worldMatrix;
}

Vector3 Transform::AnglesToDirection(Vector3 angle)
{
	return RadiansToDirection(Vector3{ XMConvertToRadians(angle.x), XMConvertToRadians(angle.y), XMConvertToRadians(angle.z) });
}

Vector3 Transform::RadiansToDirection(Vector3 radian)
{
	Vector3 result;
	XMStoreFloat3(&result, XMVector3TransformNormal(XMLoadFloat3(&worldForward), XMMatrixRotationRollPitchYaw(radian.x, radian.y, radian.z)));
	return result;
}

void Transform::SetAngle(DirectX::FXMVECTOR quaternion)
{
	UpdateForward(quaternion, forward, forwardCalculate);
	UpdateUp(quaternion, up, upCalculate);
	UpdateRight(quaternion, right, rightCalculate);

	EulerAngle(angle, quaternion);
}

void Transform::SetAngle(const float4& quaternion, Vector3& forward, bool& forwardCalculate, Vector3& right, bool& rightCalculate, Vector3& up, bool& upCalculate)
{
	XMVECTOR quaternionVector = XMLoadFloat4(&quaternion);
	UpdateForward(quaternionVector, forward, forwardCalculate);
	UpdateUp(quaternionVector, up, upCalculate);
	UpdateRight(quaternionVector, right, rightCalculate);
}

void Transform::UpdateForward(DirectX::FXMVECTOR quaternion, Vector3& forward, bool& forwardCalculate)
{
	XMStoreFloat3(&forward, DirectX::XMVector3Rotate(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), quaternion));
	forwardCalculate = true;
}

void Transform::UpdateUp(DirectX::FXMVECTOR quaternion, Vector3& up, bool& upCalculate)
{
	XMStoreFloat3(&up, DirectX::XMVector3Rotate(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), quaternion));
	upCalculate = true;
}

void Transform::UpdateRight(DirectX::FXMVECTOR quaternion, Vector3& right, bool& rightCalculate)
{
	XMStoreFloat3(&right, DirectX::XMVector3Rotate(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), quaternion));
	rightCalculate = true;
}


void Transform::SetAngle(FLOAT x, FLOAT y, FLOAT z)
{
	xmVector newQuaternion = XMQuaternionRotationRollPitchYaw(XMConvertToRadians(x), XMConvertToRadians(y), XMConvertToRadians(z));
	SetAngle(newQuaternion);
}

void Transform::UnLinkChild(UINT index) 
{
	if (index >= static_cast<UINT>(childs.size()))
		return;
	childs[index]->parent = nullptr;
	childs[index]->parentWorldMatrix = nullptr;
	auto iterator = childs.begin();
	for (UINT i = 0; i < index; i++)
		iterator++;
	childs.erase(iterator);

}

void Transform::UnLinkChild(Transform* object)
{
	auto iterator = childs.begin();
	for (auto iterator = childs.begin(); iterator < childs.end(); iterator++)
	{
		if (*iterator != object)
			continue;

		(*iterator)->parent = nullptr;
		(*iterator)->parentWorldMatrix = nullptr;

		childs.erase(iterator);
		break;
	}
}

Vector3 Transform::EulerAngle(const DirectX::XMFLOAT4& quaternion)
{
	float yaw = atan2f(2.0f * (quaternion.w * quaternion.y + quaternion.x * quaternion.z), 1.0f - 2.0f * (quaternion.y * quaternion.y + quaternion.z * quaternion.z));
	float pitch = -asinf(2.0f * (quaternion.w * quaternion.x - quaternion.z * quaternion.y));  // LH 좌표계이므로 부호 반전
	float roll = atan2f(2.0f * (quaternion.w * quaternion.z + quaternion.y * quaternion.x), 1.0f - 2.0f * (quaternion.x * quaternion.x + quaternion.z * quaternion.z));

	// 결과 적용
	return Vector3(XMConvertToDegrees(pitch), XMConvertToDegrees(yaw), XMConvertToDegrees(roll));
}

void Transform::EulerAngle(DirectX::XMFLOAT3& angle, const DirectX::XMFLOAT4& quaternion)
{
	float yaw = atan2f(2.0f * (quaternion.w * quaternion.y + quaternion.x * quaternion.z), 1.0f - 2.0f * (quaternion.y * quaternion.y + quaternion.z * quaternion.z));
	float pitch = -asinf(2.0f * (quaternion.w * quaternion.x - quaternion.z * quaternion.y));  // LH 좌표계이므로 부호 반전
	float roll = atan2f(2.0f * (quaternion.w * quaternion.z + quaternion.y * quaternion.x), 1.0f - 2.0f * (quaternion.x * quaternion.x + quaternion.z * quaternion.z));

	// 결과 적용
	angle.x = XMConvertToDegrees(pitch);
	angle.y = XMConvertToDegrees(yaw);
	angle.z = XMConvertToDegrees(roll);
}
