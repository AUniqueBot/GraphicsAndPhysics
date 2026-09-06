#include <filesystem>
#include <arch/resources/res_sceneManager.h>
#include <serialization/serialize_metafilereader.h>
#include <serialization/serialize_jsonfile.h>


void SceneManager::Init() {
	RegisterFileExtension(".scene");
	

	using namespace Serialization;
	auto metadata = MetafileSerializer::ScanForMetafilesInPath("./Assets");
	for (const MetafileData& metafile : metadata) {
		LoadResource(metafile);
	}
}




void SceneManager::CreateScene() {

}

struct {

};

void SceneManager::LoadResource(const Serialization::MetafileData& _data) {
	namespace fs = std::filesystem;
	fs::path path = _data.path;
	if (!fs::exists(_data.path)) {
		LOG_WARN("Unable to open resource: " << _data.path);
		return;
	}
	Serialization::JSONFile json = Serialization::LoadJSONFile(_data.path);
	

	
}


/*
	scene json format:

	{

		something here: {

		},

		entities : {
			"guid" : {
				"name": string,
				"parent": guid,
				"components" : {
					... : {
						...
					}
				}
			}

		}
	}


*/