#pragma once
#include <pch.h>


class Shader {
public:
	Shader(std::string _name, std::string _vertexShader, std::string _fragShader) {
		Load(_vertexShader, _fragShader);
	}
	void Load(std::string _vertexShader, std::string _fragShader);


	// - setters ------------------------------------------------------
	std::string GetShaderName() const		{ return m_name; }
	void SetShaderName(std::string _name)	{ m_name = _name; }

	void Compile();

	// - GPU Binding Functions ----------------------------------------
	void Load();
	void Unload();

private:
	std::string m_name		{};
	std::string m_srcCode		{}; //
};	