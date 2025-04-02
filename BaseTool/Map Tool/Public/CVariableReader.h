#pragma once


class CVariableReader
{
private:
	CVariableReader() = default;
	~CVariableReader() = default;

public:

	static void VariableReaderForImgui(Engine::CComponent* component);
	static void CopyVariable(Engine::Variable& variable, Engine::Variable& targetVariable);



private:
	static void ShowComponentVariableForImgui(const Engine::Variable& variables, std::string componentName);
	static void ShowListInnerVariableForImgui(const Engine::Variable& variables, std::string componentName);
	static void ShowVectorInnerVariableForImgui(const Engine::Variable& variables, std::string componentName);
};