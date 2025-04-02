#pragma once

#include "Component.h"

namespace Engine
{
	struct BoneSet
	{
		std::string SetName;
		std::string animationName;
		UINT currentPositionFrame;
		UINT currentRotationFrame;
		UINT currentScaleFrame;

		UINT boneIndex;
		UINT animationNodeIndex;
		FLOAT animationTime;
		FLOAT minTime;
		FLOAT maxTime;
	};

	class CModel;
	class ENGINEDLL BoneHandler abstract : public Component
	{
	protected:
		BoneHandler(void) = delete;
		BoneHandler(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
		BoneHandler(const BoneHandler& other);
		virtual ~BoneHandler(void) = default;
		virtual void Free(void) {}
	public:
		//static BoneHandler* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, class Model* model);


		virtual void LateUpdate(void);

		void UpdateBones(std::map<std::string, BoneSet>& boneSets, class Model* model);
		void GetData(struct AnimationNode& node, BoneSet& boneSet, Matrix& matrix);
	protected:
		class Model* model;
		std::map<std::string, BoneSet> boneSets;

	};
}