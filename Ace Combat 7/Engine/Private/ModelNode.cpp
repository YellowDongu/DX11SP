#include "Foundation.h"
#include "ModelNode.h"
#include "Bone.h"

using namespace Engine;

ModelNode::ModelNode(void) : parentNode(nullptr), bone(nullptr), defaultTransform{}, boneIndex(-1), finalTransform{}
{
	DirectX::XMStoreFloat4x4(&defaultTransform, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&finalTransform, DirectX::XMMatrixIdentity());
}

ModelNode::~ModelNode(void)
{
}

ModelNode* ModelNode::Create(void)
{
	ModelNode* newInstance = new ModelNode();
	newInstance->name = "Root";
	newInstance->boneIndex = 0;
	return newInstance;
}
ModelNode* ModelNode::Create(const std::vector<ModelConverter::FullModelNode>& nodeList)
{
	ModelNode* newInstance = new ModelNode();
	if (FAILED(newInstance->CreateNode(nodeList)))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}

	return newInstance;
}

HRESULT ModelNode::LoadNode(const ModelConverter::FullModelNode& node)
{
	memcpy(&defaultTransform, &node.defaultTransformationMatrix, sizeof(Matrix));
	memcpy(&boneIndex, &node.boneIndex, sizeof(UINT));
	name = node.name;

	return S_OK;
}
void ModelNode::LinkBone(Bone* _bone)
{
	//if (this->name != _bone->Name())
	//	return;
	bone = _bone;
}
void ModelNode::pairingBones(std::map<std::string, Bone*>& bones)
{
	if (parentNode != nullptr) // safety
	{
		ModelNode* node = parentNode;
		while (node->parentNode != nullptr)
			node = parentNode;
		node->pairingBones(bones);
		return;
	}

	std::list<ModelNode*> nodes;
	nodes.push_front(this);
	ModelNode* currentNode = nullptr;
	std::map<std::string, Bone*>::iterator boneIterator;
	while (!nodes.empty())
	{
		currentNode = nodes.front();
		nodes.pop_front();

		boneIterator = bones.find(currentNode->name);
		if (boneIterator != bones.end())
		{
			currentNode->LinkBone(boneIterator->second);
			currentNode->boneIndex = boneIterator->second->Index();
			boneIterator->second->BindNode(currentNode);
		}

		for (auto& node : currentNode->childNode)
		{
			nodes.push_front(&node);
		}
	}
}

HRESULT ModelNode::CreateNode(const std::vector<ModelConverter::FullModelNode>& nodeList)
{
	if (nodeList.size() == 0)
		return E_FAIL;


	std::list<const ModelConverter::FullModelNode*> nodeQueue;
	std::vector<ModelNode*> nodeSort(nodeList.size(), nullptr);
	const ModelConverter::FullModelNode* currentLoadedNode = nullptr;
	ModelNode* currentNode = nullptr;

	for (auto& item : nodeList)
	{
		if (item.parentIndex == -1)
			currentLoadedNode = &item;
	}

	nodeSort[currentLoadedNode->index] = this;
	nodeQueue.push_back(currentLoadedNode);

	while (!nodeQueue.empty())
	{
		currentLoadedNode = nodeQueue.front();
		nodeQueue.pop_front();
		currentNode = nodeSort[currentLoadedNode->index];

		currentNode->LoadNode(*currentLoadedNode);

		for (UINT i = 0; i < currentLoadedNode->childIndex.size(); i++)
		{
			ModelNode node;
			node.parentNode = currentNode;
			currentNode->childNode.push_back(node);
		}
		for (UINT i = 0; i < currentLoadedNode->childIndex.size(); i++)
		{
			nodeQueue.push_back(&nodeList[currentLoadedNode->childIndex[i]]);
			nodeSort[currentLoadedNode->childIndex[i]] = &currentNode->childNode[i];
		}
	}


	return S_OK;
}





#ifdef useAssimp

ModelNode* ModelNode::Create(aiNode* rootNode)
{
	ModelNode* newInstance = new ModelNode();
	if (FAILED(newInstance->CreateNode(rootNode)))
	{
		Base::Destroy(newInstance);
		return nullptr;
	}

	return newInstance;
}
HRESULT ModelNode::CreateNode(aiNode* rootNode)
{
	if (rootNode == nullptr)
		return E_FAIL;

	std::list<std::pair<aiNode*, ModelNode*>> aiNodes;
	aiNodes.push_back({ rootNode , this });
	aiNode* currentAiNode = nullptr;
	ModelNode* currentNode = nullptr;

	while (!aiNodes.empty())
	{
		currentAiNode = aiNodes.front().first;
		currentNode = aiNodes.front().second;
		aiNodes.pop_front();

		currentNode->LoadNode(currentAiNode);

		currentNode->childNode.reserve(currentAiNode->mNumChildren);
		for (UINT i = 0; i < currentAiNode->mNumChildren; i++)
		{
			ModelNode node;
			node.parentNode = currentNode;
			currentNode->childNode.push_back(node);
			aiNodes.push_front({ currentAiNode->mChildren[i], &currentNode->childNode[i] });
		}
	}

	return S_OK;
}

HRESULT ModelNode::LoadNode(aiNode* node)
{
	ContvertMatrix(defaultTransform, node->mTransformation);
	DirectX::XMStoreFloat4x4(&defaultTransform, DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&defaultTransform)));
	name = node->mName.C_Str();

	return S_OK;
}
#endif // useAssimp
