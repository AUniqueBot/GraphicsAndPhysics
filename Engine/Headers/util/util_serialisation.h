/* -------------------------------------------------------------------- */
/*
	@author		auniquebot
	@filename	util_serialisation.h
	@date		25 04 2025
	@brief		serialisation library for this engine.
*/
/* -------------------------------------------------------------------- */
#pragma once
#include<pch.h>
#include <string>
#include <typeinfo>
#include <string_view>

#if defined(__GNUC__) || defined(__clang__)
#include <cxxabi.h>
#include <cstdlib>
#endif


namespace Reflection {
	static void Init();
	static void RegisterResources();
	static void RegisterComponents();

	inline std::string Demangle(const char* name) {
#if defined(__GNUC__) || defined(__clang__)
		int status = 0;
		char* demangled = abi::__cxa_demangle(name, nullptr, nullptr, &status);
		std::string result = (status == 0 && demangled != nullptr) ? demangled : name;
		std::free(demangled);
		return result;
#else
		// MSVC: typeid(T).name() is already human-readable
		return name;
#endif
	}

	
	template <typename T>
	inline std::string TypeName() {
		return Demangle(typeid(T).name());
	}



}


namespace FileReading {
	std::string GetRawTextFromFile(std::filesystem::path _path);

}