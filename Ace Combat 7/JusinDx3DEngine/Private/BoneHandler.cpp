#include "Foundation.h"
#include "BoneHandler.h"
#include "GameObject.h"
#include "Model.h"

using namespace Engine;

BoneHandler::BoneHandler(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Component(dxDevice, dxDeviceContext), model(nullptr), boneSets{}
{
}

BoneHandler::BoneHandler(const BoneHandler& other) : Component(other), model(nullptr), boneSets(other.boneSets)
{
}

void BoneHandler::LateUpdate(void)
{
	if (model == nullptr)
	{
		model = dynamic_cast<Model*>(gameObject->GetComponent(L"Model"));
		if (model == nullptr)
		{
			ErrMsg(L"BoneHandler::NoComponent - Model");
			return;
		}
	}
	UpdateBones(boneSets, model);
}

void BoneHandler::UpdateBones(std::map<std::string, BoneSet>& boneSets, Model* model)
{
	Matrix boneMatrix;
	if (model == nullptr)
		return;

	Animation* currentAnimation = model->CurrentAnimation();
	Bone* bone = nullptr;
	for (auto boneSet : boneSets)
	{
		bone = model->GetBone(boneSet.second.boneIndex);
		if (bone == nullptr)
			continue;
		GetData(currentAnimation->animationContainer[boneSet.second.animationNodeIndex], boneSet.second, boneMatrix);
		bone->SetBoneMatrix(boneMatrix);
	}
}

void BoneHandler::GetData(AnimationNode& node, BoneSet& boneSet, Matrix& matrix)
{
	float3 position;
	float4 quaternion;
	float3 scale;
	FLOAT ratio;
	size_t index;
	FLOAT frameTime{ boneSet.animationTime };

	if (frameTime >= node.positions.back().first)
		position = node.positions.back().second;
	else
	{
		while (frameTime >= node.positions[boneSet.currentPositionFrame + 1].first)
		{
			boneSet.currentPositionFrame++;
			//if (boneSet.currentPositionFrame >= node.positions.size())
			//	boneSet.currentPositionFrame--; break;

		}
		while (frameTime < node.positions[boneSet.currentPositionFrame].first)
		{
			boneSet.currentPositionFrame--;
			//if (boneSet.currentPositionFrame < 0)
			//	boneSet.currentPositionFrame++; break;
		}

		index = boneSet.currentPositionFrame;
		ratio = frameTime - node.positions[index].first / (node.positions[index + 1].first - node.positions[index].first);
		DirectX::XMStoreFloat3(&position, DirectX::XMVectorLerp(XMLoadFloat3(&node.positions[index].second), XMLoadFloat3(&node.positions[index + 1].second), ratio));
	}

	if (frameTime >= node.quaternions.back().first)
		quaternion = node.quaternions.back().second;
	else
	{
		while (frameTime >= node.quaternions[boneSet.currentRotationFrame + 1].first)
		{
			boneSet.currentRotationFrame++;
			//if (boneSet.currentRotationFrame >= node.positions.size())
			//	boneSet.currentRotationFrame--; break;
		}
		while (frameTime < node.quaternions[boneSet.currentRotationFrame].first)
		{
			boneSet.currentRotationFrame--;
			//if (boneSet.currentRotationFrame < 0)
			//	boneSet.currentRotationFrame++; break;
		}

		index = boneSet.currentRotationFrame;
		ratio = frameTime - node.quaternions[index].first / (node.quaternions[index + 1].first - node.quaternions[index].first);
		DirectX::XMStoreFloat4(&quaternion, DirectX::XMQuaternionSlerp(XMLoadFloat4(&node.quaternions[index].second), XMLoadFloat4(&node.quaternions[index + 1].second), ratio));
	}

	if (frameTime >= node.scales.back().first)
		scale = node.scales.back().second;
	else
	{
		while (frameTime >= node.scales[boneSet.currentScaleFrame + 1].first)
		{
			boneSet.currentScaleFrame++;
			//if (boneSet.currentScaleFrame >= node.positions.size())
			//	boneSet.currentScaleFrame--; break;
		}
		while (frameTime < node.scales[boneSet.currentScaleFrame].first)
		{
			boneSet.currentScaleFrame--;
			//if (boneSet.currentScaleFrame < 0)
			//	boneSet.currentScaleFrame++; break;
		}

		index = boneSet.currentScaleFrame;
		ratio = frameTime - node.scales[index].first / (node.scales[index + 1].first - node.scales[index].first);
		DirectX::XMStoreFloat3(&scale, DirectX::XMVectorLerp(XMLoadFloat3(&node.scales[index].second), XMLoadFloat3(&node.scales[index + 1].second), ratio));
	}


	DirectX::XMMATRIX boneMatrix = DirectX::XMMatrixAffineTransformation(DirectX::XMLoadFloat3(&scale), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), DirectX::XMLoadFloat4(&quaternion), DirectX::XMLoadFloat3(&position));
	DirectX::XMStoreFloat4x4(&matrix, boneMatrix);
}
