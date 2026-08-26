#include <fstream>
#include <serialization/serialize_jsonfile.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>


// - components ----------------------------------------------------------------------------
#include <arch/components/comp_headers.h>


namespace Serialization {
	namespace fs = std::filesystem;

	JSONFile::JSONFile(JSONFileType _type) {
		switch (_type) {
		case JSONFileType::Array:
			m_document.SetArray();
			break;
		case JSONFileType::Object:
			m_document.SetObject();
			break;
		default:
			m_document.SetNull();
		}	
	}


	bool JSONFile::IsObject() const {
		return m_document.IsObject();
	}

	bool JSONFile::IsArray() const {
		return m_document.IsArray();
	}

	bool JSONFile::Parse(const std::filesystem::path& _path) {
		// load file

		std::ifstream ifs(_path);
		if (!ifs) {
			ifs.close();
			return false;
		}	
		std::stringstream ss;
		ss << ifs.rdbuf();
		ifs.close();
		m_document.Parse(ss.str().c_str());

		if (m_document.HasParseError()) {
			return false;
		}
		return IsObject() || IsArray();
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

	rapidjson::Value& JSONFile::GetMember(const char* _itemName) {
		return m_document[_itemName];
	}

	const rapidjson::Value& JSONFile::GetMember(const char* _itemName) const{
		return m_document[_itemName];
	}

	rapidjson::Value& JSONFile::operator[](const char* _itemName) {
		return m_document[_itemName];
	}

	const rapidjson::GenericArray<true, rapidjson::Value>& JSONFile::GetArray() const {
		return m_document.GetArray();
	}

	const rapidjson::Value* JSONFile::begin() const {
		return m_document.GetArray().begin();
	}
	const rapidjson::Value* JSONFile::end() const {
		return m_document.GetArray().end();
	}

	void JSONFile::AddMember(std::string _memberName, rapidjson::Value& _value) {
		auto& allocator = m_document.GetAllocator();
		m_document.AddMember(
			rapidjson::Value().SetString(_memberName.c_str(), allocator), 
			_value, 
			allocator
		);
	}

	void JSONFile::PushBack(rapidjson::Value _value) {
		m_document.PushBack(_value, m_document.GetAllocator());
	}

	rapidjson::Document& JSONFile::GetDocument() {
		return m_document;
	}

	const rapidjson::Document& JSONFile::GetDocument() const {
		return m_document;
	}

	rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> JSONFile::GetAllocator() {
		return m_document.GetAllocator();
	}




	bool SaveJSONFile(const JSONFile& _file, std::filesystem::path _path) {
		// save to thing
		namespace fs = std::filesystem;
		if (!fs::exists(_path.parent_path())) return false;
		std::ofstream ofs(_path);
		if (!ofs) return false;
		rapidjson::StringBuffer buffer;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
		const rapidjson::Document& doc = _file.GetDocument();
		doc.Accept(writer);
		ofs << buffer.GetString();
		ofs.close();

		LOG_INFO("Writing data to: " << _path);

		return true;
	}


	JSONFile LoadJSONFile(std::filesystem::path _path) {
		JSONFile file;
		file.Parse(_path);
		return file;
	}




}