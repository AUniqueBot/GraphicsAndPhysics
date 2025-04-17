#include <arch/core.h>

class Shader {
public:
	void Load(std::string, std::string);
	void Compile();

	void Load();
	void Unload();
private:

};