/* -------------------------------------------------------------------- */
/*
	@author		auniquebot
	@filename	util_serialisation.h
	@date		25 04 2025
	@brief		serialisation library for this engine.
*/
/* -------------------------------------------------------------------- */
#pragma once
#include <pch.h>
#include <util/util_logging.h>

namespace Reflection {



	static void Init() {
		

	}


	
	static void RegisterResources() {

	}

	static void RegisterComponents() {
		// register all components automatically.

		// for (auto& components : ) {
		// Component::Register();
		//}
	}






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
			return;
		}

		return (std::stringstream{} << ifs.rdbuf()).str();
	}

}