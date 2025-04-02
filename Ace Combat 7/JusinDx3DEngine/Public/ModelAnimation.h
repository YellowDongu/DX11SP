#pragma once
#include "Base.h"

namespace Engine
{
	struct AnimationNode
	{
		std::string nodeName;

		std::vector<std::pair<float, float3>> positions;
		std::vector<std::pair<float, float4>> quaternions;
		std::vector<std::pair<float, float3>> scales;

	};
	struct Animation
	{
		std::string name;
		FLOAT ticksPerSecond;
		FLOAT duration;
		std::vector<AnimationNode> animationContainer;
	};

	class Model;
	class Bone;
	class ModelAnimation : public Base
	{
	private:
		ModelAnimation(void);
		virtual ~ModelAnimation(void) = default;
		virtual void Free(void);
	public:
		static ModelAnimation* Create(Model* owner);
		static ModelAnimation* Create(std::vector<ModelConverter::FullModelAnimation>& animations, Model* owner);

		HRESULT LoadAnimation(std::vector<ModelConverter::FullModelAnimation>& animations);

		HRESULT LoadAnimationData(ModelConverter::FullModelChannel& nodeAnimation, std::vector<AnimationNode>& animationContainer);

		HRESULT SetAnimation(UINT frame, const std::string& animationKey, std::map<std::string, Bone*>& bones);
		HRESULT SetAnimation(UINT frame, Animation& animation, std::map<std::string, Bone*>& bones);
		HRESULT SetAnimation(FLOAT keyFrame, const std::string& animationKey, std::map<std::string, Bone*>& bones);
		HRESULT SetAnimation(FLOAT keyFrame, Animation& animation, std::map<std::string, Bone*>& bones);

		void CreateAnimationMatrix(INT keyFrame);
		void CreateAnimationMatrix(FLOAT keyFrame);
		void GetData(AnimationNode& node, UINT frameTime, Matrix& matrix);
		void GetInterpolateData(AnimationNode& node, float frameTime, float3& position, float4& quaternion, float3& scale);
		float3 Interpolation(float3& one, float3& other, float amount, float max);
		float4 Interpolation(float4& one, float4& other, float amount, float max);

		Animation* GetAnimation(const std::string& key);
		HRESULT SetDefaultAnimation(const std::string& key);
		Animation* DefaultAnimation(void) { if (defaultAnimation == nullptr) defaultAnimation = &animationContainer.begin()->second; return defaultAnimation; }


		#ifdef useAssimp
		static ModelAnimation* Create(const aiScene* scene, Model* owner);
		HRESULT LoadAnimation(const aiScene* scene) { return LoadAnimation(scene, animationContainer); }
		HRESULT LoadAnimation(const aiScene* scene, std::map<std::string, Animation>& animationContainer);
		HRESULT LoadAnimationData(aiNodeAnim* nodeAnimation, std::vector<AnimationNode>& animationContainer);

		#endif // useAssimp


	private:
		std::map<std::string, Animation> animationContainer;
		Animation* defaultAnimation;
		Model* linkedModel;

		//float keyFrame;
		//int targetFrame;
	};

}