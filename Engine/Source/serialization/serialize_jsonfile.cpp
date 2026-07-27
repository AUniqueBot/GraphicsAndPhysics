#include <fstream>
#include <serialization/serialize_jsonfile.h>

// - components ----------------------------------------------------------------------------
#include <arch/components/comp_headers.h>


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

	rapidjson::Value& JSONFile::GetMember(const char* _itemName) {
		return m_document[_itemName];
	}

	const rapidjson::Value& JSONFile::GetMember(const char* _itemName) const{
		return m_document[_itemName];
	}


#define GETCOMPONENTDATA() \
	const rapidjson::Value& componentData = _entityData["components"];

#define LOADCOMPONENT(comp) \
	if (componentData.HasMember(#comp))  { \
		const rapidjson::Value& compData = componentData[#comp]; \
		entity.AddComponent<comp>();\
		\
	} 


	EntityID LoadEntity(const rapidjson::Value& _entityData) {
		EntityView ev = Core::GetInstance().GetRegistry().Instantiate();
		if (!ev) {
			return EntityID::ENTITYID_INVALID;
		}
		Entity& entity = *ev;
				// do something
		
		if (_entityData.HasMember("name")) {
			entity.Name(_entityData["name"].GetString());
		}
		if (_entityData.HasMember("components")) {
			GETCOMPONENTDATA();
			LOADCOMPONENT(Transform);
			LOADCOMPONENT(MeshRenderer);
			LOADCOMPONENT(Light);
		}
		else {
			LOG_INFO("Data has no component data. Ignoring.");
		}

		return entity.GetID();
	}

	void LoadEntity(const rapidjson::Value& _entityData, EntityID _id) {

		// id does nothing.
		EntityView ev = Core::GetInstance().GetRegistry().Instantiate(_id);
		if (!ev) return;
		Entity& entity = *ev;
		// do something

		if (_entityData.HasMember("name")) {
			entity.Name(_entityData["name"].GetString());
		}
		if (_entityData.HasMember("components")) {
			GETCOMPONENTDATA();
			LOADCOMPONENT(Transform);
			LOADCOMPONENT(Camera);
			LOADCOMPONENT(MeshRenderer);
			LOADCOMPONENT(Light);
		}
		else {
			LOG_INFO("Data has no component data. Ignoring.");
		}
	}

}