#include <fstream>
#include <serialization/serialize_jsonfile.h>

namespace Serialization {
	namespace fs = std::filesystem;
	JSONFile::JSONFile(fs::path _path) {

		// load file

		std::ifstream ifs(_path);
		std::stringstream ss;
		ss << ifs.rdbuf();
		ifs.close();

		m_document = rapidjson::Document();
		m_document.Parse(ss.str().c_str());
	}

	bool JSONFile::HasMember(const char* _itemName) const {
		return m_document.HasMember(_itemName);
	}

	bool JSONFile::IsObject(const char* _itemName) const {
		if (!HasMember(_itemName)) return false;
		return m_document[_itemName].IsObject();
	}

	bool JSONFile::IsArray(const char* _itemName) const {
		if (!HasMember(_itemName)) return false;
		return m_document[_itemName].IsArray();
	}

	bool JSONFile::IsInt(const char* _itemName) const {
		if (!HasMember(_itemName)) return false;
		return m_document[_itemName].IsInt();
	}

	bool JSONFile::IsFloat(const char* _itemName) const {
		if (!HasMember(_itemName)) return false;
		return m_document[_itemName].IsFloat();
	}

	bool JSONFile::IsString(const char* _itemName) const {
		if (!HasMember(_itemName)) return false;
		return m_document[_itemName].IsString();
	}

	bool JSONFile::IsBool(const char* _itemName) const {
		if (!HasMember(_itemName)) return false;
		return m_document[_itemName].IsBool();
	}


}