#pragma once
#include <pch.h>
#include <serialization/serialize_metafile.h>

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>


namespace Serialization {

	class MetafileSerializer {
	public:
		static rapidjson::Document CreateMetafileData(const MetafileData& _data);
		static MetafileData ParseMetafile(const std::filesystem::directory_entry& _path);

		static std::vector<Serialization::MetafileData>
			ScanForMetafilesInPath(const std::filesystem::path& _path, bool _recursive = true);

	private:
		static rapidjson::Document CreateMetafileData_V1(const MetafileData& _data);
		static MetafileData ParseMetafile_V1(const std::filesystem::directory_entry& _path);

	};
}
