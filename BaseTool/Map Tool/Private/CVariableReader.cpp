#include "pch.h"
#include "CVariableReader.h"


void CVariableReader::VariableReaderForImgui(Engine::CComponent* component)
{
	std::string componentName = typeid(*component).name();
		for (auto& var : *(component->Link_VariableList()))
		{
			ShowComponentVariableForImgui(var, componentName);
		}

}

void CVariableReader::CopyVariable(Engine::Variable& variable, Engine::Variable& targetVariable)
{
	if (variable.type != targetVariable.type)
		return;

	if (variable.name != targetVariable.name)
		return;

	Engine::VariableType type = variable.type;

	switch (type)
	{
	case Engine::VariableType::string:
	{
		std::string& data = *static_cast<std::string*>(variable.data);
		std::string& otherData = *static_cast<std::string*>(targetVariable.data);
		data = otherData;
	}
	break;
	case Engine::VariableType::wstring:
	{
		std::wstring& data = *static_cast<std::wstring*>(variable.data);
		std::wstring& otherData = *static_cast<std::wstring*>(targetVariable.data);
		data = otherData;
	}
	break;
	case Engine::VariableType::Bool:
	{
		*static_cast<bool*>(variable.data) = *static_cast<bool*>(targetVariable.data);
	}
	break;
	case Engine::VariableType::Byte:
		*static_cast<char*>(variable.data) = *static_cast<char*>(targetVariable.data);
		break;
	case Engine::VariableType::UByte:
		*static_cast<unsigned char*>(variable.data) = *static_cast<unsigned char*>(targetVariable.data);
		break;
	case Engine::VariableType::Tchar:
		*static_cast<wchar_t*>(variable.data) = *static_cast<wchar_t*>(targetVariable.data);
		break;
	case Engine::VariableType::Short:
		*static_cast<short*>(variable.data) = *static_cast<short*>(targetVariable.data);
		break;
	case Engine::VariableType::UShort:
		*static_cast<unsigned short*>(variable.data) = *static_cast<unsigned short*>(targetVariable.data);
		break;
	case Engine::VariableType::Int:
		*static_cast<int*>(variable.data) = *static_cast<int*>(targetVariable.data);
	break;
	case Engine::VariableType::UInt:
		*static_cast<unsigned int*>(variable.data) = *static_cast<unsigned int*>(targetVariable.data);
		break;
	case Engine::VariableType::Long:
		*static_cast<long*>(variable.data) = *static_cast<long*>(targetVariable.data);
		break;
	case Engine::VariableType::ULong:
		*static_cast<unsigned long*>(variable.data) = *static_cast<unsigned long*>(targetVariable.data);
		break;
	case Engine::VariableType::Float:
		*static_cast<float*>(variable.data) = *static_cast<float*>(targetVariable.data);
	break;
	case Engine::VariableType::Double:
		*static_cast<double*>(variable.data) = *static_cast<double*>(targetVariable.data);
	break;
	case Engine::VariableType::Vec2:
		*static_cast<vec2*>(variable.data) = *static_cast<vec2*>(targetVariable.data);
	break;
	case Engine::VariableType::Vec3:
		*static_cast<vec3*>(variable.data) = *static_cast<vec3*>(targetVariable.data);
	break;
	case Engine::VariableType::Vec4:
		*static_cast<vec4*>(variable.data) = *static_cast<vec4*>(targetVariable.data);
	break;
	case Engine::VariableType::Matrix:
		*static_cast<matrix*>(variable.data) = *static_cast<matrix*>(targetVariable.data);
		break;
	case Engine::VariableType::Color:
		*static_cast<DWORD*>(variable.data) = *static_cast<DWORD*>(targetVariable.data);
		break;
	case Engine::VariableType::vertexSet:
	{
		int count = 0;
		Engine::VertexSet& vertexSet = *static_cast<Engine::VertexSet*>(variable.data);
		Engine::VertexSet& otherVertexSet = *static_cast<Engine::VertexSet*>(targetVariable.data);
		vertexSet.rhw = otherVertexSet.rhw;
		vertexSet.triangleCount = otherVertexSet.triangleCount;

		for (auto& item : otherVertexSet.index)
		{
			vertexSet.index.push_back(item);
		}
		for (auto& item : otherVertexSet.simpleVertex)
		{
			vertexSet.simpleVertex.push_back(item);
		}
		for (auto& item : otherVertexSet.textureVertex)
		{
			vertexSet.textureVertex.push_back(item);
		}
		for (auto& item : otherVertexSet.cubeVertex)
		{
			vertexSet.cubeVertex.push_back(item);
		}
	}
	break;
	default:
		break;
	}



}

void CVariableReader::ShowVectorInnerVariableForImgui(const Engine::Variable & variable, std::string componentName)
{
	Engine::VariableType type = variable.listInnerType;

	switch (type)
	{
	case Engine::VariableType::string:
	{
		std::vector<std::string>& stringList = *static_cast<std::vector<std::string>*>(variable.data);

		for (auto& string : stringList)
		{
			std::vector<char> buffer(string.begin(), string.end());
			buffer.push_back('\0');

			ImGui::Text(variable.name.c_str());
			if (ImGui::InputText(("##" + componentName + "::string::" + variable.name).c_str(), buffer.data(), buffer.size()))
			{
				string = std::string(buffer.data());
			}
		}
	}
	break;
	case Engine::VariableType::wstring:
	{
		std::string convertedText = To_String(*static_cast<std::wstring*>(variable.data));

		char buffer[256];
		strncpy_s(buffer, convertedText.c_str(), sizeof(buffer));

		ImGui::Text(variable.name.c_str());
		if (ImGui::InputText(("##" + componentName + "::wstring::" + variable.name).c_str(), buffer, sizeof(buffer)))
		{
			*static_cast<std::wstring*>(variable.data) = To_WString(std::string(buffer));
		}
	}
	break;
	case Engine::VariableType::Bool:
	{

		ImGui::Text(variable.name.c_str());
		ImGui::SameLine();
		ImGui::RadioButton(("##" + componentName + "::bool::" + variable.name).c_str(), static_cast<bool*>(variable.data));
	}
	break;
	case Engine::VariableType::Byte:
		break;
	case Engine::VariableType::UByte:
		break;
	case Engine::VariableType::Tchar:
		break;
	case Engine::VariableType::Short:
		break;
	case Engine::VariableType::UShort:
		break;
	case Engine::VariableType::Int:
	{

		ImGui::Text(variable.name.c_str());
		ImGui::SameLine();
		ImGui::InputInt(("##" + componentName + "::bool::" + variable.name).c_str(), static_cast<int*>(variable.data));
	}
	break;
	case Engine::VariableType::UInt:
		break;
	case Engine::VariableType::Long:
		break;
	case Engine::VariableType::ULong:
		break;
	case Engine::VariableType::Float:
	{

		ImGui::Text(variable.name.c_str());
		ImGui::SameLine();
		ImGui::InputFloat(("##" + componentName + "::bool::" + variable.name).c_str(), static_cast<float*>(variable.data));
	}
	break;
	case Engine::VariableType::Double:
	{

		ImGui::Text(variable.name.c_str());
		ImGui::SameLine();
		ImGui::InputDouble(("##" + componentName + "::bool::" + variable.name).c_str(), static_cast<double*>(variable.data));
	}
	break;
	case Engine::VariableType::Vec2:
	{
		ImGui::Text(variable.name.c_str());
		ImGui::SameLine();
		ImGui::InputFloat2(("##" + componentName + "::vec3::" + variable.name).c_str(), *static_cast<vec2*>(variable.data));
	}
	break;
	case Engine::VariableType::Vec3:
	{
		float windowWidth = ImGui::GetWindowWidth();  // ���� â�� ���� ũ��
		float fixedWidth = windowWidth / 4.0f;        // â�� ���� ũ�� 4�� ���� ��

		ImVec2 textSize = ImGui::CalcTextSize(variable.name.c_str());
		float xPos = ImGui::GetCursorPosX() + fixedWidth - textSize.x;
		ImGui::SetCursorPosX(xPos);

		// �ؽ�Ʈ ���
		ImGui::Text(variable.name.c_str());
		ImGui::SameLine();
		ImGui::InputFloat3(("##" + componentName + "::vec3::" + variable.name).c_str(), *static_cast<vec3*>(variable.data));
	}
	break;
	case Engine::VariableType::Vec4:
	{
		ImGui::Text(variable.name.c_str());
		ImGui::SameLine();
		ImGui::InputFloat4(("##" + componentName + "::vec3::" + variable.name).c_str(), *static_cast<vec4*>(variable.data));
	}
	break;
	case Engine::VariableType::Matrix:
		break;
	case Engine::VariableType::Color:
		break;
	default:
		break;
	}

}

void CVariableReader::ShowComponentVariableForImgui(const Engine::Variable& variable, std::string componentName)
{
	Engine::VariableType type = variable.type;

	switch (type)
	{
	case Engine::VariableType::string:
	{
		std::string& text = *static_cast<std::string*>(variable.data);

		char buffer[1024];
		strncpy_s(buffer, text.c_str(), sizeof(buffer));

		ImGui::Text(variable.name.c_str());
		if (ImGui::InputText(("##" + componentName + "::string::" + variable.name).c_str(), buffer, sizeof(buffer)))
		{
			*static_cast<std::string*>(variable.data) = std::string(buffer);
		}
	}
		break;
	case Engine::VariableType::wstring:
	{
		std::string convertedText = To_String(*static_cast<std::wstring*>(variable.data));

		char buffer[1024];
		strncpy_s(buffer, convertedText.c_str(), sizeof(buffer));

		ImGui::Text(variable.name.c_str());
		if (ImGui::InputText(("##" + componentName + "::wstring::" + variable.name).c_str(), buffer, sizeof(buffer)))
		{
			*static_cast<std::wstring*>(variable.data) = To_WString(std::string(buffer));
		}
	}
		break;
	case Engine::VariableType::Bool:
	{
		bool* data = static_cast<bool*>(variable.data);
		ImGui::Text(variable.name.c_str());
		ImGui::SameLine();
		if (ImGui::RadioButton(("##" + componentName + "::bool::" + variable.name).c_str(), *data)) *data = !(*data);
	}
		break;
	case Engine::VariableType::Byte:
		break;
	case Engine::VariableType::UByte:
		break;
	case Engine::VariableType::Tchar:
		break;
	case Engine::VariableType::Short:
		break;
	case Engine::VariableType::UShort:
		break;
	case Engine::VariableType::Int:
	{

		ImGui::Text(variable.name.c_str());
		ImGui::SameLine();
		ImGui::InputInt(("##" + componentName + "::bool::" + variable.name).c_str(), static_cast<int*>(variable.data));
	}
		break;
	case Engine::VariableType::UInt:
		break;
	case Engine::VariableType::Long:
		break;
	case Engine::VariableType::ULong:
		break;
	case Engine::VariableType::Float:
	{

		ImGui::Text(variable.name.c_str());
		ImGui::SameLine();
		ImGui::InputFloat(("##" + componentName + "::bool::" + variable.name).c_str(), static_cast<float*>(variable.data));
	}
		break;
	case Engine::VariableType::Double:
	{

		ImGui::Text(variable.name.c_str());
		ImGui::SameLine();
		ImGui::InputDouble(("##" + componentName + "::bool::" + variable.name).c_str(), static_cast<double*>(variable.data));
	}
		break;
	case Engine::VariableType::Vec2:
	{
		ImGui::Text(variable.name.c_str());
		ImGui::SameLine();
		ImGui::InputFloat2(("##" + componentName + "::vec3::" + variable.name).c_str(), *static_cast<vec2*>(variable.data));
	}
		break;
	case Engine::VariableType::Vec3:
	{
		float windowWidth = ImGui::GetWindowWidth();
		float fixedWidth = windowWidth / 4.0f;

		ImVec2 textSize = ImGui::CalcTextSize(variable.name.c_str());
		float xPos = ImGui::GetCursorPosX() + fixedWidth - textSize.x;
		ImGui::SetCursorPosX(xPos);

		ImGui::Text(variable.name.c_str());
		ImGui::SameLine();
		ImGui::InputFloat3(("##" + componentName + "::vec3::" + variable.name).c_str(), *static_cast<vec3*>(variable.data));
	}
		break;
	case Engine::VariableType::Vec4:
	{
		ImGui::Text(variable.name.c_str());
		ImGui::SameLine();
		ImGui::InputFloat4(("##" + componentName + "::vec3::" + variable.name).c_str(), *static_cast<vec4*>(variable.data));
	}
		break;
	case Engine::VariableType::Matrix:
		break;
	case Engine::VariableType::Color:
		break;
	case Engine::VariableType::stdVector:
	{

	}
		break;
	case Engine::VariableType::stdList:
		break;
	case Engine::VariableType::vertexSet:
	{
		int count = 0;
		Engine::VertexSet& vertexSet = *static_cast<Engine::VertexSet*>(variable.data);

		if (ImGui::TreeNode(("index##" + componentName + "::VertexSet::index::" + variable.name).c_str()))
		{
			if (ImGui::Button(("Delete index##" + componentName + "::VertexSet::index::" + variable.name).c_str()))
			{
				if (!vertexSet.index.empty())
				vertexSet.index.pop_back();
			}

			for (auto& index : vertexSet.index)
			{
				ImGui::Text("indexSet");
				ImGui::SameLine();
				// unsigned short 값을 임시로 저장할 int 배열
				int tempValues[3] = { static_cast<int>(index._0),
									  static_cast<int>(index._1),
									  static_cast<int>(index._2) };

				if (ImGui::InputInt3((to_string(count) + "##" + componentName + "::VertexSet::index::vec3::" + variable.name).c_str(), tempValues))
				{
					for (int i = 0; i < 3; ++i)
					{
						if (tempValues[i] < 0) tempValues[i] = 0;
					}
					index._0 = static_cast<unsigned short>(tempValues[0]);
					index._1 = static_cast<unsigned short>(tempValues[1]);
					index._2 = static_cast<unsigned short>(tempValues[2]);
				}

				count++;
			}

			if (ImGui::MenuItem(("Add new##" + componentName + "::VertexSet::SimpleVertex::new::" + variable.name).c_str()))
			{
				Engine::INDEX16 newIndex = { 0,0,0 };
				vertexSet.index.push_back(newIndex);
			}
			ImGui::TreePop();

		}

		if (ImGui::TreeNode(("SimpleVertex##" + componentName + "::VertexSet::SimpleVertex::" + variable.name).c_str()))
		{
			if (ImGui::Button(("Delete vertex##SimpleVertex" + componentName + "::VertexSet::SimpleVertex::" + variable.name).c_str()))
			{
				if (!vertexSet.simpleVertex.empty())
				vertexSet.simpleVertex.pop_back();
			}


			for (auto& vertex : vertexSet.simpleVertex)
			{
				ImGui::Text("position");
				ImGui::SameLine();
				ImGui::InputFloat3((to_string(count) + "##" + componentName + "::VertexSet::SimpleVertex::vec3::" + variable.name).c_str(), vertex.vPosition);

				//ImGui::Text("color");
				//ImGui::SameLine();
				//ImGui::InputFloat3(("##" + componentName + "::VertexSet::SimpleVertex::vec3::" + variable.name).c_str(), vertex.dwColor);
				count++;
			}

			if (ImGui::MenuItem(("Add new##" + componentName + "::VertexSet::SimpleVertex::new::" + variable.name).c_str()))
			{
				Engine::tagVertexColor newVertex = { {0.0f,0.0f,0.0f}, D3DCOLOR_ARGB(255,255,255,255) };
				vertexSet.simpleVertex.push_back(newVertex);
			}
			ImGui::TreePop();

		}

		count = 0;
		if (ImGui::TreeNode(("textureVertex##" + componentName + "::VertexSet::textureVertex::" + variable.name).c_str()))
		{

			if (ImGui::Button(("Delete vertex##textureVertex" + componentName + "::VertexSet::textureVertex::" + variable.name).c_str()))
			{
				if (!vertexSet.textureVertex.empty())
				vertexSet.textureVertex.pop_back();
			}

			for (auto& vertex : vertexSet.textureVertex)
			{
				ImGui::Text("position");
				ImGui::SameLine();
				ImGui::InputFloat3((to_string(count) + "##" + componentName + "::VertexSet::textureVertex::vec3::XY::" + variable.name).c_str(), vertex.vPosition);

				ImGui::Text("uvPosition");
				ImGui::SameLine();
				ImGui::InputFloat2((to_string(count) + "##" + componentName + "::VertexSet::textureVertex::vec3::UV::" + variable.name).c_str(), vertex.vTexUV);
				count++;
			}

			if (ImGui::MenuItem(("Add new##" + componentName + "::VertexSet::textureVertex::new::" + variable.name).c_str()))
			{
				Engine::tagVertexTexture newVertex = { {0.0f,0.0f,0.0f}, {0.0f,0.0f,0.0f}, {0.0f,0.0f} };
				vertexSet.textureVertex.push_back(newVertex);
			}
			ImGui::TreePop();
		}

		count = 0;
		if (ImGui::TreeNode(("cubeVertex##" + componentName + "::VertexSet::cubeVertex::" + variable.name).c_str()))
		{
			if (ImGui::Button(("Delete vertex##cubeVertex" + componentName + "::VertexSet::cubeVertex::" + variable.name).c_str()))
			{
				if(!vertexSet.cubeVertex.empty())
					vertexSet.cubeVertex.pop_back();
			}

			for (auto& vertex : vertexSet.cubeVertex)
			{
				ImGui::Text("position");
				ImGui::SameLine();
				ImGui::InputFloat3((to_string(count) + "##" + componentName + "::VertexSet::cubeVertex::vec3::XY::" + variable.name).c_str(), vertex.vPosition);

				ImGui::Text("uvPosition");
				ImGui::SameLine();
				ImGui::InputFloat3((to_string(count) + "##" + componentName + "::VertexSet::cubeVertex::vec3::UV::" + variable.name).c_str(), vertex.vTexUV);
				count++;
			}

			if (ImGui::MenuItem(("Add new##" + componentName + "::VertexSet::cubeVertex::new::" + variable.name).c_str()))
			{
				Engine::tagVertexCubeTexture newVertex = { {0.0f,0.0f,0.0f}, {0.0f,0.0f,0.0f} };
				vertexSet.cubeVertex.push_back(newVertex);
			}
			ImGui::TreePop();
		}
	}
	break;
	default:
		break;
	}
}

void CVariableReader::ShowListInnerVariableForImgui(const Engine::Variable& variable, std::string componentName)
{
	Engine::VariableType type = variable.listInnerType;

	switch (type)
	{
	case Engine::VariableType::string:
	{
		std::vector<char> buffer(static_cast<std::string*>(variable.data)->begin(), static_cast<std::string*>(variable.data)->end());
		buffer.push_back('\0');

		ImGui::Text(variable.name.c_str());
		ImGui::SameLine();
		if (ImGui::InputText(("##" + componentName + "::int::" + variable.name).c_str(), buffer.data(), 256))
		{
			*static_cast<std::string*>(variable.data) = std::string(buffer.data());
		}
	}
	break;
	case Engine::VariableType::wstring:
	{
		std::string convertedText = To_String(*static_cast<std::wstring*>(variable.data));

		char buffer[256];
		strncpy_s(buffer, convertedText.c_str(), sizeof(buffer));

		ImGui::Text(variable.name.c_str());
		ImGui::SameLine();
		if (ImGui::InputText(("##" + componentName + "::wstring::" + variable.name).c_str(), buffer, sizeof(buffer)))
		{
			*static_cast<std::wstring*>(variable.data) = To_WString(std::string(buffer));
		}
	}
	break;
	case Engine::VariableType::Bool:
	{

		ImGui::Text(variable.name.c_str());
		ImGui::SameLine();
		ImGui::RadioButton(("##" + componentName + "::bool::" + variable.name).c_str(), static_cast<bool*>(variable.data));
	}
	break;
	case Engine::VariableType::Byte:
		break;
	case Engine::VariableType::UByte:
		break;
	case Engine::VariableType::Tchar:
		break;
	case Engine::VariableType::Short:
		break;
	case Engine::VariableType::UShort:
		break;
	case Engine::VariableType::Int:
	{

		ImGui::Text(variable.name.c_str());
		ImGui::SameLine();
		ImGui::InputInt(("##" + componentName + "::bool::" + variable.name).c_str(), static_cast<int*>(variable.data));
	}
	break;
	case Engine::VariableType::UInt:
		break;
	case Engine::VariableType::Long:
		break;
	case Engine::VariableType::ULong:
		break;
	case Engine::VariableType::Float:
	{

		ImGui::Text(variable.name.c_str());
		ImGui::SameLine();
		ImGui::InputFloat(("##" + componentName + "::bool::" + variable.name).c_str(), static_cast<float*>(variable.data));
	}
	break;
	case Engine::VariableType::Double:
	{

		ImGui::Text(variable.name.c_str());
		ImGui::SameLine();
		ImGui::InputDouble(("##" + componentName + "::bool::" + variable.name).c_str(), static_cast<double*>(variable.data));
	}
	break;
	case Engine::VariableType::Vec2:
	{
		ImGui::Text(variable.name.c_str());
		ImGui::SameLine();
		ImGui::InputFloat2(("##" + componentName + "::vec3::" + variable.name).c_str(), *static_cast<vec2*>(variable.data));
	}
	break;
	case Engine::VariableType::Vec3:
	{
		float windowWidth = ImGui::GetWindowWidth();
		float fixedWidth = windowWidth / 4.0f;

		ImVec2 textSize = ImGui::CalcTextSize(variable.name.c_str());
		float xPos = ImGui::GetCursorPosX() + fixedWidth - textSize.x;
		ImGui::SetCursorPosX(xPos);

		ImGui::Text(variable.name.c_str());
		ImGui::SameLine();
		ImGui::InputFloat3(("##" + componentName + "::vec3::" + variable.name).c_str(), *static_cast<vec3*>(variable.data));
	}
	break;
	case Engine::VariableType::Vec4:
	{
		ImGui::Text(variable.name.c_str());
		ImGui::SameLine();
		ImGui::InputFloat4(("##" + componentName + "::vec3::" + variable.name).c_str(), *static_cast<vec4*>(variable.data));
	}
	break;
	case Engine::VariableType::Matrix:
		break;
	case Engine::VariableType::Color:
		break;
	default:
		break;
	}



}
//
//void CVariableReader::ShowVectorInnerVariableForImgui(const Engine::Variable& variable, std::string componentName)
//{
//	Engine::VariableType type = variable.listInnerType;
//
//	switch (type)
//	{
//	case Engine::VariableType::string:
//	{
//		std::vector<std::string>& stringList = *static_cast<std::vector<std::string>*>(variable.data);
//
//		for (auto& string : stringList)
//		{
//			std::vector<char> buffer(string.begin(), string.end());
//			buffer.push_back('\0');
//
//			ImGui::Text(variable.name.c_str());
//			ImGui::SameLine();
//			if (ImGui::InputText(("##" + componentName + "::string::" + variable.name).c_str(), buffer.data(), buffer.size()))
//			{
//				string = std::string(buffer.data());
//			}
//		}
//	}
//	break;
//	case Engine::VariableType::wstring:
//	{
//		std::string convertedText = To_String(*static_cast<std::wstring*>(variable.data));
//	
//		char buffer[256];
//		strncpy_s(buffer, convertedText.c_str(), sizeof(buffer));
//	
//		ImGui::Text(variable.name.c_str());
//		ImGui::SameLine();
//		if (ImGui::InputText(("##" + componentName + "::wstring::" + variable.name).c_str(), buffer, sizeof(buffer)))
//		{
//			*static_cast<std::wstring*>(variable.data) = To_WString(std::string(buffer));
//		}
//	}
//	break;
//	case Engine::VariableType::Bool:
//	{
//	
//		ImGui::Text(variable.name.c_str());
//		ImGui::SameLine();
//		ImGui::RadioButton(("##" + componentName + "::bool::" + variable.name).c_str(), static_cast<bool*>(variable.data));
//	}
//	break;
//	case Engine::VariableType::Byte:
//		break;
//	case Engine::VariableType::UByte:
//		break;
//	case Engine::VariableType::Tchar:
//		break;
//	case Engine::VariableType::Short:
//		break;
//	case Engine::VariableType::UShort:
//		break;
//	case Engine::VariableType::Int:
//	{
//	
//		ImGui::Text(variable.name.c_str());
//		ImGui::SameLine();
//		ImGui::InputInt(("##" + componentName + "::bool::" + variable.name).c_str(), static_cast<int*>(variable.data));
//	}
//	break;
//	case Engine::VariableType::UInt:
//		break;
//	case Engine::VariableType::Long:
//		break;
//	case Engine::VariableType::ULong:
//		break;
//	case Engine::VariableType::Float:
//	{
//	
//		ImGui::Text(variable.name.c_str());
//		ImGui::SameLine();
//		ImGui::InputFloat(("##" + componentName + "::bool::" + variable.name).c_str(), static_cast<float*>(variable.data));
//	}
//	break;
//	case Engine::VariableType::Double:
//	{
//	
//		ImGui::Text(variable.name.c_str());
//		ImGui::SameLine();
//		ImGui::InputDouble(("##" + componentName + "::bool::" + variable.name).c_str(), static_cast<double*>(variable.data));
//	}
//	break;
//	case Engine::VariableType::Vec2:
//	{
//		ImGui::Text(variable.name.c_str());
//		ImGui::SameLine();
//		ImGui::InputFloat2(("##" + componentName + "::vec3::" + variable.name).c_str(), *static_cast<vec2*>(variable.data));
//	}
//	break;
//	case Engine::VariableType::Vec3:
//	{
//		float windowWidth = ImGui::GetWindowWidth();  // ���� â�� ���� ũ��
//		float fixedWidth = windowWidth / 4.0f;        // â�� ���� ũ�� 4�� ���� ��
//	
//		ImVec2 textSize = ImGui::CalcTextSize(variable.name.c_str());
//		float xPos = ImGui::GetCursorPosX() + fixedWidth - textSize.x;
//		ImGui::SetCursorPosX(xPos);
//	
//		// �ؽ�Ʈ ���
//		ImGui::Text(variable.name.c_str());
//		ImGui::SameLine();
//		ImGui::InputFloat3(("##" + componentName + "::vec3::" + variable.name).c_str(), *static_cast<vec3*>(variable.data));
//	}
//	break;
//	case Engine::VariableType::Vec4:
//	{
//		ImGui::Text(variable.name.c_str());
//		ImGui::SameLine();
//		ImGui::InputFloat4(("##" + componentName + "::vec3::" + variable.name).c_str(), *static_cast<vec4*>(variable.data));
//	}
//	break;
//	case Engine::VariableType::Matrix:
//		break;
//	case Engine::VariableType::Color:
//		break;
//	default:
//		break;
//	}
//
//
//}
