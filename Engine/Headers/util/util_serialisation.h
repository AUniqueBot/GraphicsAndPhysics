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
#include <string_view>

namespace Reflection {
	static void Init();
	static void RegisterResources();
	static void RegisterComponents();


	template<typename T>
	constexpr std::string_view TypeName() {
	#if defined(__clang__) || defined(__GNUC__)
		constexpr std::string_view p = __PRETTY_FUNCTION__;
		constexpr std::string_view prefix = "constexpr std::string_view TypeName() [with T = ";
		constexpr std::string_view suffix = "]";
	#elif defined(_MSC_VER)
		constexpr std::string_view p = __FUNCSIG__;
		constexpr std::string_view prefix = "class std::string_view __cdecl TypeName<";
		constexpr std::string_view suffix = ">(void)";
	#endif
		return p.substr(prefix.size(), p.size() - prefix.size() - suffix.size());
	}


}


namespace FileReading {
	std::string GetRawTextFromFile(std::filesystem::path _path);

}