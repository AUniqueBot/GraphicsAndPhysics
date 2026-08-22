#include <configparser.h>

ConfigData ParseConfigFile(std::filesystem::path _path) {

	std::filesystem::path crpth = std::filesystem::current_path();
	bool res = std::filesystem::exists(_path);

	std::ifstream ifs(_path);
	if (!ifs) {
		ifs.close();
		LOG_ERROR("Problem opening config file: " << _path);
		return {};
	}


	/*
		sample config file
		[sectionheader]
		attribute=value
		// parser v1 assumes no comments.
		// all section headers start with [ and end with ]
		// all attributes contain values that are separated by the '=' symbol
		// we assume anything th
	
	*/
	std::string line;
	std::string sectionName;
	ConfigData data;
	while (std::getline(ifs, line)) {
		// strip comments
		size_t eraseSection = line.find("//");
		if (eraseSection != std::string::npos) {
			line.erase(eraseSection);
		}
		eraseSection = line.find_first_not_of(" \t");
		// skip empty lines.
		if (eraseSection == std::string::npos) {
			continue;
		}

		// strip all empty spaces.
		line.erase(std::remove_if(line.begin(), line.end(), [](unsigned char ch) {
			return std::isspace(ch);
			}), line.end());

		

		// section detection
		if (line.front() == '[' && line.back() == ']' && line.size() > 2) {
			ConfigSection section;
			sectionName = line.substr(1, line.size() - 2);
			//section.name = sectionName;
			data[sectionName] = section;
			continue;
		}
		size_t separatorVal = line.find_first_of("=");

		if (separatorVal != std::string::npos) {
			if (sectionName.empty()) {
				// Optional: LOG_ERROR("Attribute found before any section header!");
				continue;
			}
			// split the string to 2 sections
			// xxxxxx=yyyyyy
			std::string attribute = line.substr(0, separatorVal);
			std::string value = line.substr(separatorVal + 1);
			data[sectionName].props[attribute] = value;
		}

	}

	ifs.close();
	LOG_INFO("finished reading config file");
	return data;

}

void SaveConfigData(const ConfigData& _data, std::filesystem::path _path) {
	std::ofstream ofs(_path);
	if (!ofs) {
		LOG_ERROR("Failed to open path: " << _path);
		ofs.close();
		return;
	}
	
	// no comments.
	for (const auto& [header, data] : _data) {
		ofs << "[" << header << "] // test comment" << std::endl;
		for (const auto& value : data.props) {
			std::string attrString = value.first + "=" + value.second;
			ofs << attrString << std::endl;
		}
		ofs << std::endl;
	}

	ofs.close();
}