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
		rapidjson::Document CreateMetafileData(const MetafileData& _data);
		MetafileData ParseMetafile(const std::filesystem::directory_entry& _path);

	private:

		rapidjson::Document CreateMetafileData_V1(const MetafileData& _data);
		MetafileData ParseMetafile_V1(const std::filesystem::directory_entry& _path);

	};
}
