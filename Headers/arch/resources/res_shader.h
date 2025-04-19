#include <arch/core.h>

class Shader {
public:
	enum SHADERTYPE {
		VERTEX,
		FRAG,
		// wip or whenever I feel like it.
		GEOMETRY,
		TESELLATION_CONTROL,
		TESSELATION_EVALUATION,
		COMPUTE
	};

public:
	void Load(std::string, std::string);
	void Compile();

	void Load();
	void Unload();
private:
	int m_shaderID { 0 };
	std::unordered_map<std::string, GLint> m_uniformMap;
	


};