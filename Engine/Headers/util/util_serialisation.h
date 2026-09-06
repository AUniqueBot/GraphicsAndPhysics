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
#include <nameof.hpp>


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
#else
        std::string result = name;
#endif

        // Remove compiler-specific type prefixes.
        constexpr std::string_view prefixes[] = {
            "class ",
            "struct ",
            "enum ",
            "union "
        };

        for (const std::string_view prefix : prefixes) {
            if (result.starts_with(prefix)) {
                result.erase(0, prefix.size());
                break;
            }
        }

        // Remove namespace qualification.
        const size_t namespacePos = result.rfind("::");
        if (namespacePos != std::string::npos)
            result.erase(0, namespacePos + 2);

        return result;
    }



	
	template <typename T>
	inline std::string TypeName() {
        return static_cast<std::string>(nameof::nameof_short_type<T>());
		//return Demangle(typeid(T).name());
	}



}


namespace FileReading {
	std::string GetRawTextFromFile(std::filesystem::path _path);

}