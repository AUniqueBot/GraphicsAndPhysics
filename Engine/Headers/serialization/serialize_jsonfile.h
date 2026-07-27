#pragma once
#include <pch.h>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <arch/ecs/ecs_entityFactory.h>

#include <arch/core.h>





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

		rapidjson::Value& GetMember(const char* _itemName);
		const rapidjson::Value& GetMember(const char* _itemName) const;

	private:
		rapidjson::Document m_document;

	};

	EntityID LoadEntity(const rapidjson::Value& _entityData);
	void LoadEntity(const rapidjson::Value& _entityData, EntityID _id);
}

#include <serialization/serialize_jsonfileDEF.hpp>