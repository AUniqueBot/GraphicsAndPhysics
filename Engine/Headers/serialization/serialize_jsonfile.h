#pragma once
#include <pch.h>

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>


namespace Serialization {
	enum class JSONFileType {
		Array,
		Object,
		Null
	};



	//! @brief the JSONFile class represents a JSON representation in CPU.
	//! saving and loading are handled externally by explicit functions not part of this class.
	//! assume this as an intermediary datatype you can use for other serializer functions.
	class JSONFile {
	public:
		JSONFile(JSONFileType _type  = JSONFileType::Null);


		bool IsObject() const;
		bool IsArray() const;
		// ctors.

	public:
		bool Parse(const std::filesystem::path& _path);
	public:
		// - object doc -----------------------------------
		// object
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

		rapidjson::Value& operator[](const char* _itemName);

	public:
		// - array doc ------------------------------------
		const rapidjson::GenericArray<true, rapidjson::Value>& GetArray() const;
		const rapidjson::Value* begin() const;
		const rapidjson::Value* end() const;


	public: 
		// - adding members -------------------------------
		void AddMember(std::string _memberName, rapidjson::Value& _value);
		void PushBack(rapidjson::Value _value);

	public:
		// - generic handle functions ---------------------
		rapidjson::Document& GetDocument();
		const rapidjson::Document& GetDocument() const;

		rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> GetAllocator();

	private:
		rapidjson::Document m_document;
	};

	bool SaveJSONFile(const JSONFile& _file, std::filesystem::path _path);
	JSONFile LoadJSONFile(std::filesystem::path _path);

}

#include <serialization/serialize_jsonfileDEF.hpp>