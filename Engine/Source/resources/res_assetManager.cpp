#include <arch/resources/res_assetManager.h>




void AssetManager::Init() {

	RegisterManager<Shader, ShaderManager>();
	RegisterManager<ShaderProgram, ShaderProgramManager>();
	RegisterManager<Material, MaterialManager>();
	RegisterManager<TextureRes, TextureManager>();
	RegisterManager<MeshRes, MeshManager>();

	GetShaderProgramManager().InitShaderPrograms(GetShaderManager());

	ScanResourcesInPath("./Assets");
}

void AssetManager::Cleanup() {
	for (auto& manager : m_managerList) {
		manager->Cleanup();
	}
}

void AssetManager::ScanResourcesInPath(const std::filesystem::path& _path, bool _recursive) {
	namespace fs = std::filesystem;
	using namespace Serialization;
	// phase 1 -> meta file scanning.
	std::vector<Serialization::MetafileData> metadata = 
		MetafileSerializer::ScanForMetafilesInPath(_path, _recursive);
	// - phase 2 - parsing entries
	for (const Serialization::MetafileData& metafile : metadata) {
		LoadResource(metafile);
	}

}



ShaderManager& AssetManager::GetShaderManager() { 
	auto mgr = std::static_pointer_cast<ShaderManager>(
		*m_managerList.At(Shader::GetResourceTypeID())
	);
	return *mgr;
}
const ShaderManager& AssetManager::GetShaderManager() const { 
	auto mgr = std::static_pointer_cast<ShaderManager>(
		*m_managerList.At(Shader::GetResourceTypeID())
	);
	return *mgr;
}

ShaderProgramManager& AssetManager::GetShaderProgramManager() {
	auto mgr = std::static_pointer_cast<ShaderProgramManager>(
		*m_managerList.At(ShaderProgram::GetResourceTypeID())
	);
	return *mgr;
}

const ShaderProgramManager& AssetManager::GetShaderProgramManager() const {
	auto mgr = std::static_pointer_cast<ShaderProgramManager>(
		*m_managerList.At(ShaderProgram::GetResourceTypeID())
	);
	return *mgr;
}

MaterialManager& AssetManager::GetMaterialManager() { 
	auto mgr = std::static_pointer_cast<MaterialManager>(
		*m_managerList.At(Material::GetResourceTypeID())
	);
	return *mgr;
}
const MaterialManager& AssetManager::GetMaterialManager() const { 
	auto mgr = std::static_pointer_cast<MaterialManager>(
		*m_managerList.At(Material::GetResourceTypeID())
	);
	return *mgr;
}

TextureManager& AssetManager::GetTextureManager() { 
	auto mgr = std::static_pointer_cast<TextureManager>(
		*m_managerList.At(TextureRes::GetResourceTypeID())
	);
	return *mgr; 
}
const TextureManager& AssetManager::GetTextureManager() const { 
	auto mgr = std::static_pointer_cast<TextureManager>(
		*m_managerList.At(TextureRes::GetResourceTypeID())
	);
	return *mgr; 
}

MeshManager& AssetManager::GetMeshManager() {
	auto mgr = std::static_pointer_cast<MeshManager>(
		*m_managerList.At(MeshRes::GetResourceTypeID())
	);
	return *mgr;
}

const MeshManager& AssetManager::GetMeshManager() const {
	auto mgr = std::static_pointer_cast<MeshManager>(
		*m_managerList.At(MeshRes::GetResourceTypeID())
	);
	return *mgr;
}



void AssetManager::SaveMetafileData(const Serialization::MetafileData& _data) {
	std::ofstream ofs(_data.path);

	if (!ofs) {
		ofs.close();
		return;
	}

	rapidjson::Document doc = Serialization::MetafileSerializer::CreateMetafileData(_data);
	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);

	ofs << buffer.GetString();
	ofs.close();
	
}

void AssetManager::LoadResource(const Serialization::MetafileData& _metafile) {
	const std::filesystem::path& path = _metafile.path;
	std::string extension = path.extension().string();
	if (extension.length() == 0) {
		//LOG_INFO("file has no extension: " << entry.path().filename());
		return;
	}

	for (auto& manager : m_managerList) {
		if (!manager || !manager->AcceptsFileExtension(extension)) {
			continue;
		}
		manager->LoadResource(_metafile);
		return;
	}
	

}

