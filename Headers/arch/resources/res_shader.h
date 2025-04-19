#include <arch/core.h>

class ShaderProgram {
public:
	enum SHADERTYPE {
		VERTEX,
		FRAG,
		// wip or whenever I feel like it.
		GEOMETRY,
		TESELLATION_CONTROL,
		TESSELATION_EVALUATION,
		COMPUTE,
		_COUNT
	};

public:
	void Load(std::string, std::string);
	void Compile();

	void Load();
	void Unload();



private:
	static void LogCompileStatus(int _statusCode, int _shaderID);
	bool PreCompileCheck();

private:
	int m_shaderID { 0 };
	std::unordered_map<std::string, GLint> m_uniformMap;
	std::unordered_map<SHADERTYPE, std::filesystem::path> m_shaderMap{}; // map of paths, not the src.

	std::bitset<_COUNT> m_shaderLoadStatus;

};

