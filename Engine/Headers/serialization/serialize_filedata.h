#pragma once
#include <pch.h>


namespace Serialization {
	class FileData {
	public:
		// ctors.
	public:
		// static functions
		static bool IsObject();
		static bool IsArray();
		static bool IsInt();
		static bool IsFloat();
		static bool IsString();


	};
}