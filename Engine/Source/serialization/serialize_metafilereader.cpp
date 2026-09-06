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
	

	std::vector<Serialization::MetafileData>
		MetafileSerializer::ScanForMetafilesInPath(
			const std::filesystem::path& _path, bool _recursive
		) {
		namespace fs = std::filesystem;
		// phase 1 -> meta file scanning.
		std::vector<Serialization::MetafileData> metadata;


		if (!fs::exists(_path) || !fs::is_directory(_path)) {
			LOG_ERROR("Provided path is not a directory \"" << _path << "\"");
			return metadata;
		}
		LOG_INFO("Scanned Path: " << _path);


		if (_recursive) {
			for (const fs::directory_entry& entry : fs::recursive_directory_iterator(_path)) {
				if (entry.is_directory()) {
					//LOG_INFO("Scanning path: " << entry.path());
					continue;
				}
				// scan for associated types.
				// match file extensions to asset types
				// grab normalized extension.
				std::string extension = entry.path().extension().string();
				if (extension != ".meta") {
					continue;
				}

				Serialization::MetafileData data = Serialization::MetafileSerializer::ParseMetafile(entry);
				if (data.IsValid()) {
					metadata.push_back(data);
				}
			}
		}
		else {
			for (const fs::directory_entry& entry : fs::directory_iterator(_path)) {
				std::string extension = entry.path().extension().string();
				if (extension.length() == 0) {
					//LOG_INFO("file has no extension: " << entry.path().filename());
					continue;
				}

				Serialization::MetafileData data = Serialization::MetafileSerializer::ParseMetafile(entry);
				if (data.IsValid()) {
					metadata.push_back(data);
				}
			}
		}
		return metadata;
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