#pragma once
#include "Base.h"

namespace Engine
{
	class ModelLoader
	{
	public:
		ModelLoader(void) = default;
		virtual ~ModelLoader(void) = default;

		HRESULT LoadFile(const std::wstring filePath, ModelConverter::FullModel& model);
		HRESULT LoadModel(std::ifstream& file, ModelConverter::FullModel& model);

		void BinaryToString(std::string& text, std::ifstream& fileStream);

		HRESULT LoadModel(ModelConverter::FullModel& model, std::ifstream& fileStream);
		HRESULT LoadMesh(ModelConverter::FullModelMesh& mesh, std::ifstream& fileStream);
		HRESULT LoadMaterial(ModelConverter::FullModelMaterial& material, std::ifstream& fileStream);
		HRESULT LoadBone(ModelConverter::FullModelBone& bone, std::ifstream& fileStream);
		HRESULT LoadNode(ModelConverter::FullModelNode& node, std::ifstream& fileStream);
		HRESULT LoadAnimation(ModelConverter::FullModelAnimation& material, std::ifstream& fileStream);
		HRESULT LoadChannel(ModelConverter::FullModelChannel& channel, std::ifstream& fileStream);

	private:
	};

}