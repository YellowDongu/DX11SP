#include "Foundation.h"
#include "ModelAnimation.h"
#include "Model.h"


using namespace Engine;

ModelAnimation::ModelAnimation(void) : linkedModel(nullptr), defaultAnimation(nullptr)
{
}

void ModelAnimation::Free(void)
{
}


ModelAnimation* ModelAnimation::Create(Model* model)
{
	std::vector<ModelConverter::FullModelAnimation> animations;
	ModelAnimation* newInstance = new ModelAnimation();
	newInstance->linkedModel = model;
	if (FAILED(newInstance->LoadAnimation(animations)))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
	return newInstance;
}

ModelAnimation* ModelAnimation::Create(std::vector<ModelConverter::FullModelAnimation>& animations, Model* model)
{
	ModelAnimation* newInstance = new ModelAnimation();
	newInstance->linkedModel = model;
	if (FAILED(newInstance->LoadAnimation(animations)))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
	return newInstance;
}


HRESULT ModelAnimation::LoadAnimation(std::vector<ModelConverter::FullModelAnimation>& animations)
{
	Animation newAnimation;
	if (animations.empty())
	{
		newAnimation.name = "Static";
		newAnimation.ticksPerSecond = 0.0f;
		newAnimation.duration = 0.0f;
		std::vector<ModelConverter::FullModelChannel> channels;
		AnimationNode newChannel;
		newChannel.nodeName = "Root";
		newChannel.positions.push_back({0.0f, {0.0f,0.0f,0.0f} });
		newChannel.quaternions.push_back({0.0f, {0.0f,0.0f,0.0f,0.0f} });
		newChannel.scales.push_back({ 0.0f, {0.0f,0.0f,0.0f} });

		animationContainer[newAnimation.name] = newAnimation;
	}

	for (auto& item : animations)
	{
		newAnimation.name = item.name;
		memcpy(&newAnimation.ticksPerSecond, &item.ticksPerSecond, sizeof(FLOAT));
		memcpy(&newAnimation.duration, &item.duration, sizeof(FLOAT));

		for (auto& channel : item.channels)
		{
			LoadAnimationData(channel, newAnimation.animationContainer);
		}

		animationContainer[newAnimation.name] = newAnimation;
	}

	if (animationContainer.size() == 0)
		return E_FAIL;

	defaultAnimation = &animationContainer.begin()->second;
	return S_OK;
}

HRESULT ModelAnimation::LoadAnimationData(ModelConverter::FullModelChannel& nodeAnimation, std::vector<AnimationNode>& animationContainer)
{
	animationContainer.push_back(AnimationNode());
	AnimationNode& animationNode = animationContainer.back();
	animationNode.nodeName = nodeAnimation.boneName;

	animationNode.positions.resize(nodeAnimation.positions.size());
	animationNode.quaternions.resize(nodeAnimation.quaternions.size());
	animationNode.scales.resize(nodeAnimation.scales.size());


	for (size_t i = 0; i < animationNode.positions.size(); i++)
	{
		std::memcpy(&animationNode.positions[i].first, &nodeAnimation.positions[i].keyFrame, sizeof(FLOAT));
		std::memcpy(static_cast<DirectX::XMFLOAT3*>(&animationNode.positions[i].second), &nodeAnimation.positions[i].data, sizeof(DirectX::XMFLOAT3));
	}
	for (size_t i = 0; i < animationNode.quaternions.size(); i++)
	{
		std::memcpy(&animationNode.quaternions[i].first, &nodeAnimation.quaternions[i].keyFrame, sizeof(FLOAT));
		std::memcpy(static_cast<DirectX::XMFLOAT4*>(&animationNode.quaternions[i].second), &nodeAnimation.quaternions[i].data, sizeof(DirectX::XMFLOAT4));
	}
	for (size_t i = 0; i < animationNode.scales.size(); i++)
	{
		std::memcpy(&animationNode.scales[i].first, &nodeAnimation.scales[i].keyFrame, sizeof(FLOAT));
		std::memcpy(static_cast<DirectX::XMFLOAT3*>(&animationNode.scales[i].second), &nodeAnimation.scales[i].data, sizeof(DirectX::XMFLOAT3));
	}

	return S_OK;
}

HRESULT ModelAnimation::SetAnimation(UINT frame, const std::string& animationKey, std::map<std::string, Bone*>& bones)
{
	auto iterator = animationContainer.find(animationKey);
	if (iterator == animationContainer.end())
		return E_FAIL;

	return SetAnimation(frame, iterator->second, bones);
}

HRESULT ModelAnimation::SetAnimation(FLOAT keyFrame, const std::string& animationKey, std::map<std::string, Bone*>& bones)
{
	return E_FAIL; // not finished
	auto animationIterator = animationContainer.find(animationKey);
	if (animationIterator == animationContainer.end())
		return E_FAIL;

	return SetAnimation(keyFrame, animationIterator->second, bones);
}

HRESULT ModelAnimation::SetAnimation(UINT frame, Animation& animation, std::map<std::string, Bone*>& bones)
{
	Matrix matrix;
	ModelNode* node;
	std::map<std::string, Bone*>::iterator boneIterator;
	for (auto& channel : animation.animationContainer)
	{
		boneIterator = bones.find(channel.nodeName);
		if (boneIterator == bones.end())
		{
			node = linkedModel->GetNode(channel.nodeName);
			if (node == nullptr)
				continue;
			else
			{
				GetData(channel, frame, matrix);
				node->DefaultTransformMatrix(DirectX::XMLoadFloat4x4(&matrix));
				continue;
			}
		}
		GetData(channel, frame, matrix);
		boneIterator->second->SetBoneMatrix(matrix);
	}

	return S_OK;
}

HRESULT ModelAnimation::SetAnimation(FLOAT keyFrame, Animation& animation, std::map<std::string, Bone*>& bones)
{
	std::map<std::string, Bone*>::iterator boneIterator;
	float3 position;
	float4 quaternion;
	float3 scale;
	ModelNode* node;

	for (auto animationNode : animation.animationContainer)
	{
		boneIterator = bones.find(animationNode.nodeName);
		if (boneIterator == bones.end())
		{
			node = linkedModel->GetNode(animationNode.nodeName);
			if(node == nullptr)
			continue;
			else
			{
				GetInterpolateData(animationNode, keyFrame, position, quaternion, scale);
				node->DefaultTransformMatrix(DirectX::XMMatrixAffineTransformation(DirectX::XMLoadFloat3(&scale), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), DirectX::XMLoadFloat4(&quaternion), DirectX::XMLoadFloat3(&position)));
				continue;
			}
		}

		GetInterpolateData(animationNode, keyFrame, position, quaternion, scale);
		boneIterator->second->SetBoneMatrix(DirectX::XMMatrixAffineTransformation(DirectX::XMLoadFloat3(&scale), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), DirectX::XMLoadFloat4(&quaternion), DirectX::XMLoadFloat3(&position)));
	}
	return S_OK;
}

Animation* ModelAnimation::GetAnimation(const std::string& key)
{
	auto iterator = animationContainer.find(key);
	if (iterator == animationContainer.end())
		return nullptr;

	return &iterator->second;
}

HRESULT ModelAnimation::SetDefaultAnimation(const std::string& key)
{
	Animation* animation = GetAnimation(key);
	if (animation == nullptr)
		return E_FAIL;

	defaultAnimation = animation;
	return S_OK;
}

void ModelAnimation::CreateAnimationMatrix(FLOAT keyFrame)
{
	return; // not finished

	if (defaultAnimation == nullptr)
		defaultAnimation = &animationContainer.begin()->second;

	Bone* bone = nullptr;
	float3 position = {};
	float4 quaternion = {};
	float3 scale = {};

	for (auto animationNode : defaultAnimation->animationContainer)
	{
		bone = linkedModel->GetBone(animationNode.nodeName);
		if (bone == nullptr)
			continue;

		GetInterpolateData(animationNode, keyFrame, position, quaternion, scale);
		bone->SetBoneMatrix(DirectX::XMMatrixAffineTransformation(DirectX::XMLoadFloat3(&scale), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), DirectX::XMLoadFloat4(&quaternion), DirectX::XMLoadFloat3(&position)));
	}
}

void ModelAnimation::GetData(AnimationNode& node, UINT frameTime, Matrix& matrix)
{
	float3 position;
	float4 quaternion;
	float3 scale;

	if (frameTime >= (INT)node.positions.size())
		position = node.positions.back().second;
	else
		position = node.positions[frameTime].second;

	if (frameTime >= (INT)node.quaternions.size())
		quaternion = node.quaternions.back().second;
	else
		quaternion = node.quaternions[frameTime].second;

	if (frameTime >= (INT)node.scales.size())
		scale = node.scales.back().second;
	else
		scale = node.scales[frameTime].second;

	DirectX::XMMATRIX boneMatrix = DirectX::XMMatrixAffineTransformation(DirectX::XMLoadFloat3(&scale), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), DirectX::XMLoadFloat4(&quaternion), DirectX::XMLoadFloat3(&position));
	DirectX::XMStoreFloat4x4(&matrix, boneMatrix);

}

void ModelAnimation::GetInterpolateData(AnimationNode& node, float frameTime, float3& position, float4& quaternion, float3& scale)
{
	bool notFound = true;
	size_t size = node.positions.size() - 1;
	for (size_t i = 0; i < size; i++)
	{
		if (node.positions[i].first < frameTime)
			continue;

		position = Interpolation(node.positions[i].second, node.positions[i + 1].second, node.positions[i].first - frameTime, node.positions[i + 1].first - node.positions[i].first);
		notFound = false;
		break;
	}
	if(notFound)
		position = node.positions.end()->second;

	notFound = true;
	size = node.quaternions.size() - 1;
	for (size_t i = 0; i < size; i++)
	{
		if (node.quaternions[i].first < frameTime)
			continue;

		quaternion = Interpolation(node.quaternions[i].second, node.quaternions[i + 1].second, node.quaternions[i].first - frameTime, node.quaternions[i + 1].first - node.quaternions[i].first);
		notFound = false;
		break;
	}
	if (notFound)
		quaternion = node.quaternions.end()->second;

	notFound = true;
	size = node.scales.size() - 1;
	for (size_t i = 0; i < size; i++)
	{
		if (node.positions[i].first < frameTime)
			continue;

		scale = Interpolation(node.scales[i].second, node.scales[i + 1].second, node.scales[i].first - frameTime, node.scales[i + 1].first - node.scales[i].first);
		notFound = false;
		break;
	}
	if (notFound)
		scale = node.scales.end()->second;
}

void ModelAnimation::CreateAnimationMatrix(INT keyFrame)
{
	if (defaultAnimation == nullptr)
		defaultAnimation = &animationContainer.begin()->second;

	Bone* bone = nullptr;
	float3 position;
	float4 quaternion;
	float3 scale;

	for (auto animationNode : defaultAnimation->animationContainer)
	{
		bone = linkedModel->GetBone(animationNode.nodeName);
		if (bone == nullptr)
			continue;

		if (keyFrame >= (INT)animationNode.positions.size())
			position = animationNode.positions.back().second;
		else
			position = animationNode.positions[keyFrame].second;

		if (keyFrame >= (INT)animationNode.quaternions.size())
			quaternion = animationNode.quaternions.back().second;
		else
			quaternion = animationNode.quaternions[keyFrame].second;

		if (keyFrame >= (INT)animationNode.scales.size())
			scale = animationNode.scales.back().second;
		else
			scale = animationNode.scales[keyFrame].second;

		DirectX::XMMATRIX boneMatrix = DirectX::XMMatrixAffineTransformation(DirectX::XMLoadFloat3(&scale), DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), DirectX::XMLoadFloat4(&quaternion), DirectX::XMLoadFloat3(&position));
		bone->SetBoneMatrix(boneMatrix);
	}
}

float3 ModelAnimation::Interpolation(float3& one, float3& other, float amount, float max)
{
	return one;
}

float4 ModelAnimation::Interpolation(float4& one, float4& other, float amount, float max)
{
	return one;
}





#ifdef useAssimp

ModelAnimation* ModelAnimation::Create(const aiScene* scene, Model* model)
{
	ModelAnimation* newInstance = new ModelAnimation();
	newInstance->linkedModel = model;
	if (FAILED(newInstance->LoadAnimation(scene)))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}
	return newInstance;
}
HRESULT ModelAnimation::LoadAnimation(const aiScene* scene, std::map<std::string, Animation>& animationContainer)
{
	if (!scene->HasAnimations())
		return E_FAIL;
	aiAnimation* aiAni = nullptr;
	for (UINT i = 0; i < scene->mNumAnimations; i++)
	{
		aiAni = scene->mAnimations[i];
		Animation newAnimation;
		newAnimation.name = aiAni->mName.C_Str();
		newAnimation.duration = (FLOAT)aiAni->mDuration;
		newAnimation.ticksPerSecond = (FLOAT)aiAni->mTicksPerSecond;

		for (UINT j = 0; j < aiAni->mNumChannels; j++)
		{
			LoadAnimationData(aiAni->mChannels[j], newAnimation.animationContainer);
		}
		animationContainer[newAnimation.name] = newAnimation;
	}

	if (animationContainer.size() == 0)
		return E_FAIL;

	defaultAnimation = &animationContainer.begin()->second;
	return S_OK;
}
HRESULT ModelAnimation::LoadAnimationData(aiNodeAnim* channel, std::vector<AnimationNode>& animationContainer)
{
	AnimationNode animationNode;
	animationNode.nodeName = channel->mNodeName.C_Str();/*
	animationNode.positions.resize(channel->mNumPositionKeys);
	animationNode.quaternions.resize(channel->mNumRotationKeys);
	animationNode.scales.resize(channel->mNumScalingKeys);
	for (UINT key = 0; key < channel->mNumPositionKeys; ++key)
	{
		animationNode.positions[key].first = static_cast<FLOAT>(channel->mPositionKeys[key].mTime);
		std::memcpy(reinterpret_cast<FLOAT*>(static_cast<DirectX::XMFLOAT3*>(&animationNode.positions[key].second)), reinterpret_cast<FLOAT*>(&channel->mPositionKeys[key].mValue), sizeof(FLOAT) * 3);
	}
	for (UINT key = 0; key < channel->mNumRotationKeys; ++key)
	{
		animationNode.quaternions[key].first = static_cast<FLOAT>(channel->mRotationKeys[key].mTime);
		std::memcpy(reinterpret_cast<FLOAT*>(static_cast<DirectX::XMFLOAT4*>(&animationNode.quaternions[key].second)), reinterpret_cast<FLOAT*>(&channel->mRotationKeys[key].mValue), sizeof(FLOAT) * 4);
	}
	for (UINT key = 0; key < channel->mNumScalingKeys; ++key)
	{
		animationNode.scales[key].first = static_cast<FLOAT>(channel->mScalingKeys[key].mTime);
		std::memcpy(reinterpret_cast<FLOAT*>(static_cast<DirectX::XMFLOAT3*>(&animationNode.scales[key].second)), reinterpret_cast<FLOAT*>(&channel->mScalingKeys[key].mValue), sizeof(FLOAT) * 3);
	}*/

	for (UINT key = 0; key < channel->mNumPositionKeys; ++key)
	{
		animationNode.positions.push_back({ (float)channel->mPositionKeys[key].mTime, { (float)channel->mPositionKeys[key].mValue.x, (float)channel->mPositionKeys[key].mValue.y, (float)channel->mPositionKeys[key].mValue.z } });
	}
	for (UINT key = 0; key < channel->mNumRotationKeys; ++key)
	{
		animationNode.quaternions.push_back({ (float)channel->mRotationKeys[key].mTime , { (float)channel->mRotationKeys[key].mValue.x, (float)channel->mRotationKeys[key].mValue.y, (float)channel->mRotationKeys[key].mValue.z, (float)channel->mRotationKeys[key].mValue.w } });
	}
	for (UINT key = 0; key < channel->mNumScalingKeys; ++key)
	{
		animationNode.scales.push_back({ (float)channel->mScalingKeys[key].mTime , { (float)channel->mScalingKeys[key].mValue.x, (float)channel->mScalingKeys[key].mValue.y, (float)channel->mScalingKeys[key].mValue.z } });
	}
	#pragma region Substitution
	//for (UINT key = 0; key < channel->mNumPositionKeys; ++key)
	//{
	//	animationNode.positions[key].first = static_cast<FLOAT>(channel->mPositionKeys[key].mTime);
	//	animationNode.positions[key].second.x = (float)channel->mPositionKeys[key].mValue.x;
	//	animationNode.positions[key].second.y = (float)channel->mPositionKeys[key].mValue.y;
	//	animationNode.positions[key].second.z = (float)channel->mPositionKeys[key].mValue.z; // 대입
	// 
	//	animationNode.positions.push_back({ (float)channel->mPositionKeys[key].mTime, { (float)channel->mPositionKeys[key].mValue.x, (float)channel->mPositionKeys[key].mValue.y, (float)channel->mPositionKeys[key].mValue.z } });
	//}
	//for (UINT key = 0; key < channel->mNumRotationKeys; ++key)
	//{
	//	animationNode.quaternions[key].first = static_cast<FLOAT>(channel->mRotationKeys[key].mTime);
	//	animationNode.quaternions[key].second.x = (float)channel->mRotationKeys[key].mValue.x;
	//	animationNode.quaternions[key].second.y = (float)channel->mRotationKeys[key].mValue.y;
	//	animationNode.quaternions[key].second.z = (float)channel->mRotationKeys[key].mValue.z;
	//	animationNode.quaternions[key].second.w = (float)channel->mRotationKeys[key].mValue.w; // 대입
	// 
	//	animationNode.quaternions.push_back({ (float)channel->mRotationKeys[key].mTime , { (float)channel->mRotationKeys[key].mValue.x, (float)channel->mRotationKeys[key].mValue.y, (float)channel->mRotationKeys[key].mValue.z, (float)channel->mRotationKeys[key].mValue.w } });
	//}
	//for (UINT key = 0; key < channel->mNumScalingKeys; ++key)
	//{
	//	animationNode.scales[key].first = static_cast<FLOAT>(channel->mScalingKeys[key].mTime);
	//	animationNode.scales[key].second.x = (float)channel->mScalingKeys[key].mValue.x;
	//	animationNode.scales[key].second.y = (float)channel->mScalingKeys[key].mValue.y;
	//	animationNode.scales[key].second.z = (float)channel->mScalingKeys[key].mValue.z; // 대입
	// 
	//	animationNode.scales.push_back({ (float)channel->mScalingKeys[key].mTime , { (float)channel->mScalingKeys[key].mValue.x, (float)channel->mScalingKeys[key].mValue.y, (float)channel->mScalingKeys[key].mValue.z } });
	//}
	#pragma endregion Substitution

	animationContainer.push_back(animationNode);
	return S_OK;
}

#endif // useAssimp
