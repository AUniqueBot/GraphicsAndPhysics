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

namespace Reflection {
	static void Init();
	static void RegisterResources();
	static void RegisterComponents();
}


namespace FileReading {
	std::string GetRawTextFromFile(std::filesystem::path _path);

}