#include <arch/resources/res_shaderManager.h>

void ShaderManager::AddShader(
	std::string _identifier, 
	Shader _program
) {
	m_shaderDatabase.Add(std::move(_program), _identifier);
}

void ShaderManager::RemoveShader(
	std::string _identifier
) {
	m_shaderDatabase.Remove(_identifier);
}

SparseSetView<Shader> ShaderManager::GetShader(std::string _identifier) {
	return m_shaderDatabase.At(_identifier);
}
SparseSetView<const Shader> ShaderManager::GetShader(std::string _identifier) const {
	return m_shaderDatabase.At(_identifier);
}

const ShaderDatabase& ShaderManager::GetDatabase() const {
	return m_shaderDatabase;
}
