#pragma once
#include <pch.h>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>



namespace Serialization {
	class JSONFile {
	public:
		JSONFile(std::filesystem::path _path);

		// ctors.
	public:
		// static functions
		bool HasMember(const char* _itemName) const;
		bool IsObject(const char* _itemName) const;
		bool IsArray(const char* _itemName) const;
		bool IsInt(const char* _itemName) const;
		bool IsFloat(const char* _itemName) const;
		bool IsString(const char* _itemName) const;
		bool IsBool(const char* _itemName) const;

		template <typename T>
		std::optional<T> Get(const char* _itemName);

	private:
		rapidjson::Document m_document;

	};
}

#include <serialization/serialize_jsonfileDEF.hpp>