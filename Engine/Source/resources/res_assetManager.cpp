#include <arch/resources/res_assetManager.h>
#include <rapidjson/istreamwrapper.h>

namespace {
	// metafile points to the actual resource file.



	MetafileData ParseMetafile(const std::filesystem::directory_entry& _path) {
		std::ifstream ifs(_path.path());
		if (!ifs.good()) return MetafileData();
		rapidjson::IStreamWrapper isw(ifs);
		rapidjson::Document doc;
		doc.ParseStream(isw);
		ifs.close();

		MetafileData metadata;
		if (doc.HasMember("guid")) {
			metadata.id = doc["guid"].GetUint64();
		}
		metadata.path = _path.path();
		metadata.path.replace_extension();
		return metadata;
	}
}



void AssetManager::Init() {
	m_shaderManager.Init();
	m_shaderPrgManager.Init();
	m_materialManager.Init();
	m_textureManager.Init();
	m_meshManager.Init();

	ScanResourcesInPath("./Assets");
}

void AssetManager::Cleanup() {
	m_shaderManager.Cleanup();
	m_shaderPrgManager.Cleanup();
	m_materialManager.Cleanup();
	m_textureManager.Cleanup();
	m_meshManager.Cleanup();
}

void AssetManager::ScanResourcesInPath(const std::filesystem::path& _path, bool _recursive) {
	namespace fs = std::filesystem;
	// phase 1 -> meta file scanning.
	std::vector<MetafileData> metadata;

	
	if (!fs::exists(_path) || !fs::is_directory(_path)) {
		LOG_ERROR("Provided path is not a directory \"" << _path << "\"");
		return;
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

			MetafileData data = ParseMetafile(entry);
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

			MetafileData data = ParseMetafile(entry);
			if (data.IsValid()) {
				metadata.push_back(data);
			}
		}
	}



	// - phase 2 - parsing entries


	for (const MetafileData& metafile : metadata) {
		LoadResource(metafile);
	}

}

ShaderManager& AssetManager::GetShaderManager() { 
	return m_shaderManager; 
}
const ShaderManager& AssetManager::GetShaderManager() const { 
	return m_shaderManager; 
}

ShaderProgramManager& AssetManager::GetShaderProgramManager() {
	return m_shaderPrgManager;
}

const ShaderProgramManager& AssetManager::GetShaderProgramManager() const {
	return m_shaderPrgManager;
}

MaterialManager& AssetManager::GetMaterialManager() { 
	return m_materialManager; 
}
const MaterialManager& AssetManager::GetMaterialManager() const { 
	return m_materialManager; 
}

TextureManager& AssetManager::GetTextureManager() { 
	return m_textureManager; 
}
const TextureManager& AssetManager::GetTextureManager() const { 
	return m_textureManager; 
}

MeshManager& AssetManager::GetMeshManager() {
	return m_meshManager;
}

const MeshManager& AssetManager::GetMeshManager() const {
	return m_meshManager;
}



void AssetManager::LoadResource(const MetafileData& _metafile) {
	const std::filesystem::path& path = _metafile.path;
	std::string extension = path.extension().string();
	if (extension.length() == 0) {
		//LOG_INFO("file has no extension: " << entry.path().filename());
		return;
	}
	if (m_materialManager.AcceptsFileExtension(extension)) {
		LOG_INFO("Reading material path: " << path);
		m_materialManager.LoadResource(_metafile);
	}
	else if (m_meshManager.AcceptsFileExtension(extension)) {
		LOG_INFO("Reading mesh path: " << path);
		m_meshManager.LoadResource(_metafile);
	}
	else if (m_shaderManager.AcceptsFileExtension(extension)) {
		LOG_INFO("Reading shader path: " << path);
		m_shaderManager.LoadResource(_metafile);
	}
	else if (m_textureManager.AcceptsFileExtension(extension)) {
		LOG_INFO("Reading texture path: " << path);
		m_textureManager.LoadResource(_metafile);
	}

}
