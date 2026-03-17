#pragma once
#include <pch.h>
#include <arch/resources/res_shader.h>
#include <arch/datatypes/type_sparseSet.h>
#include <arch/resources/res_material.h>




using ShaderDatabase = SparseSet<std::string, Shader>;
class ShaderManager {
public:

	void AddShader(std::string _identifier, Shader _program);
	void RemoveShader(std::string _identifier);

	SparseSetView<Shader> GetShader(std::string _identifier);
	SparseSetView<const Shader> GetShader(std::string _identifier) const;

	const SparseSet<std::string, Shader>& GetDatabase() const;


private:
	SparseSet<std::string, Shader> m_shaderDatabase;
};