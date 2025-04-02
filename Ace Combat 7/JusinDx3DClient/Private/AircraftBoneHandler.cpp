#include "pch.h"
#include "AircraftBoneHandler.h"
#include "InputManager.h"
#include "FlightMovement.h"

AircraftBoneHandler::AircraftBoneHandler(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext) : Engine::BoneHandler(dxDevice, dxDeviceContext), landingGearApply(false)
{
}

AircraftBoneHandler::AircraftBoneHandler(const AircraftBoneHandler& other) : Engine::BoneHandler(other)
{
}

void AircraftBoneHandler::Free(void)
{
	Engine::BoneHandler::Free();
}

void AircraftBoneHandler::Update(void)
{
	return;
	if (yoke != nullptr)
	{
		Elevator(yoke->x); Rudder(yoke->y); Aileron(yoke->z);
	}
	else
	{
		#define buttonInput(key) Input()->getButton(KeyType::key)
		Rudder(static_cast<FLOAT>(buttonInput(Z)) - static_cast<FLOAT>(buttonInput(X)));
		Aileron(static_cast<FLOAT>(buttonInput(C)) - static_cast<FLOAT>(buttonInput(B)));
		Elevator(static_cast<FLOAT>(buttonInput(F)) - static_cast<FLOAT>(buttonInput(V)));
		#undef buttonInput
	}
}

void AircraftBoneHandler::FixedUpdate(void)
{
}

void AircraftBoneHandler::LateUpdate(void)
{
	if (body != nullptr)
	{
		UpdateBones(boneSets, body);
	}
	if (landingGear != nullptr)
	{
		UpdateBones(gearSets, landingGear);
	}
}


#define CheckMinMax(boneSet)						\
if (boneSet->animationTime < boneSet->minTime)		\
	boneSet->animationTime = boneSet->minTime;		\
if (boneSet->animationTime > boneSet->maxTime)		\
	boneSet->animationTime = boneSet->maxTime;		\

void AircraftBoneHandler::Aileron(float inputValue)
{
	if (abs(inputValue) > 1.0f)
		inputValue /= abs(inputValue);

	if (aileronRParts.size() == 0 || aileronLParts.size() == 0)
		return;

	if (inputValue == 0)
	{
		float value;
		float middleValue = (aileronLParts[0]->maxTime - aileronLParts[0]->minTime) / 2.0f;
		if (aileronLParts[0]->animationTime != middleValue)
		{
			value = middleValue - aileronLParts[0]->animationTime;
			value /= abs(value);

			aileronLParts[0]->animationTime += value;
		}
		if (aileronRParts[0]->animationTime != middleValue)
		{
			value = middleValue - aileronRParts[0]->animationTime;
			value /= abs(value);

			aileronRParts[0]->animationTime += value;
		}
	}
	else
	{
		aileronLParts[0]->animationTime += inputValue;
		CheckMinMax(aileronLParts[0]);

		aileronRParts[0]->animationTime += inputValue * -1.0f;
		CheckMinMax(aileronRParts[0]);
	}
}

void AircraftBoneHandler::Rudder(float inputValue)
{
	if (rudderParts.size() == 0)
		return;

	if (abs(inputValue) > 1.0f)
		inputValue /= abs(inputValue);

	if (inputValue == 0)
	{
		float middleValue = (rudderParts[0]->maxTime - rudderParts[0]->minTime) / 2.0f;
		if (rudderParts[0]->animationTime != middleValue)
		{
			float value = middleValue - rudderParts[0]->animationTime;
			value /= abs(value);

			rudderParts[0]->animationTime += value;
			for (size_t i = 1; i < rudderParts.size(); i++)
				rudderParts[i]->animationTime = rudderParts[0]->animationTime;
		}
	}
	else
	{
		rudderParts[0]->animationTime += inputValue;
		CheckMinMax(rudderParts[0]);
		for (size_t i = 1; i < rudderParts.size(); i++)
			rudderParts[i]->animationTime = rudderParts[0]->animationTime;
	}

}

void AircraftBoneHandler::Elevator(float inputValue)
{
	if (elevatorParts.size() == 0)
		return;

	if (abs(inputValue) > 1.0f)
		inputValue /= abs(inputValue);

	if (inputValue == 0)
	{
		float middleValue = (elevatorParts[0]->maxTime - elevatorParts[0]->minTime) / 2.0f;
		if (elevatorParts[0]->animationTime != middleValue)
		{
			float value = middleValue - elevatorParts[0]->animationTime;
			value /= abs(value);

			elevatorParts[0]->animationTime += value;
			for (size_t i = 1; i < elevatorParts.size(); i++)
				elevatorParts[i]->animationTime = elevatorParts[0]->animationTime;
		}
	}
	else
	{
		elevatorParts[0]->animationTime += inputValue;
		CheckMinMax(elevatorParts[0]);
		for (size_t i = 1; i < elevatorParts.size(); i++)
			elevatorParts[i]->animationTime = elevatorParts[0]->animationTime;
	}
}

void AircraftBoneHandler::ChangeAnimationFrame(FLOAT value, const std::string channelName)
{
	auto boneSetIterator = boneSets.find(channelName);
	if (boneSetIterator == boneSets.end())
		return;

	boneSetIterator->second.animationTime += value;
	if (boneSetIterator->second.animationTime < boneSetIterator->second.minTime)
		boneSetIterator->second.animationTime = boneSetIterator->second.minTime;
	if (boneSetIterator->second.animationTime > boneSetIterator->second.maxTime)
		boneSetIterator->second.animationTime = boneSetIterator->second.maxTime;
}

void AircraftBoneHandler::CreateBoneSets(std::map<std::string, Engine::BoneSet>& boneSets, Engine::Model* model, Engine::Animation* targetAnimation)
{
	Engine::BoneSet newStruct;
	newStruct.animationTime = 0;
	newStruct.minTime = 0;
	newStruct.maxTime = 100;
	newStruct.animationName = targetAnimation->name;
	newStruct.currentPositionFrame = 0;
	newStruct.currentRotationFrame = 0;
	newStruct.currentScaleFrame = 0;

	Engine::Bone* bone = nullptr;
	UINT index = 0;
	for (auto& channel : targetAnimation->animationContainer)
	{
		newStruct.animationNodeIndex = index;
		newStruct.SetName = channel.nodeName;

		bone = model->GetBone(channel.nodeName);
		if (bone == nullptr) { index++; continue; }

		newStruct.boneIndex = bone->Index();
		boneSets[newStruct.SetName] = newStruct;
		index++;
	}
}

#undef CheckMinMax