#include <util/util_serialisation.h>



namespace Reflection {
	void Init()	{
	
	};
	void RegisterResources() {
	
	};
	void RegisterComponents() {
	
	};
}

namespace FileReading {

	std::string GetRawTextFromFile(std::filesystem::path _path) {
		std::ifstream ifs{};
		ifs.open(_path);
		if (!ifs.good()) {

			std::string errString{ "failed to open file <" };
			errString += _path.filename().string();
			errString += ">";
			LOG_WARN(errString);
			ifs.close();
			return "";
		}


		std::string str{"reading file \'"};
		str += _path.filename().string() + "\' successful";
		LOG_INFO(str);
		return (std::stringstream{} << ifs.rdbuf()).str();
	}
}