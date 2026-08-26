#include <serialization/serialize_metafilereader.h>
#include <serialization/serialize_jsonfile.h>
#include <rapidjson/istreamwrapper.h>

namespace Serialization {
	rapidjson::Document MetafileSerializer::CreateMetafileData(const MetafileData& _data) {
		rapidjson::Document doc;
		doc.SetObject();
		auto& allocator = doc.GetAllocator();
		doc.AddMember(
			"version",
			rapidjson::Value().SetInt(1),
			allocator
		);
		// - guid -------------------------------------
		doc.AddMember(
			"guid",
			rapidjson::Value().SetUint64(_data.id),
			allocator
		);
		return doc;
	}

	MetafileData MetafileSerializer::ParseMetafile(const std::filesystem::directory_entry& _path) {
		return ParseMetafile_V1(_path);
	}
	
	
	MetafileData MetafileSerializer::ParseMetafile_V1(const std::filesystem::directory_entry& _path) {
		if (!std::filesystem::exists(_path)) return MetafileData();
		Serialization::JSONFile json = Serialization::LoadJSONFile(_path);
		MetafileData metadata;
		if (json.HasMember("guid")) {
			metadata.id = json["guid"].GetUint64();
		}
		metadata.path = _path.path();
		metadata.path.replace_extension();
		return metadata;
	}
}