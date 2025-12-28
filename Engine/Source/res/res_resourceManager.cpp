#include <arch/resources/res_resourceManager.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/filewritestream.h>
#include <random>
#include <algorithm>
#include <cctype>
#include <arch/resources/res_mesh.h>

ResourceManager::ResourceManager() {
	
}




void ResourceManager::Init() {
	LOG_INFO("Init ResourceManager");
	// loads a json (if any) of all possible asset paths configured by the engine.
	
	
	
	// registering the default file extensions
	RegisterFileExtension(".stl", Resource::MESH);
	RegisterFileExtension(".obj", Resource::MESH);
	RegisterFileExtension(".frag", Resource::SHADER);
	RegisterFileExtension(".vert", Resource::SHADER);
	
	LoadDefaultResources();


	ScanResourcesInPath("Assets", true); // scan for typical setup


	
}

void ResourceManager::ScanResourcesInPath(std::filesystem::path _filePath, bool _recursive) {
	// scans the path provided.
	namespace fs = std::filesystem ;
	// if the folder has subfolders, call function again.

	if (!fs::exists(_filePath) || !fs::is_directory(_filePath)) {
		LOG_ERROR("Provided path is not a directory \"" << _filePath << "\"");
		return;
	}
	LOG_INFO("Scanned Path: "<< _filePath);


	if (_recursive) {
		for (const fs::directory_entry& entry : fs::recursive_directory_iterator(_filePath)) {
			
			LOG_INFO(entry.path());
			if (entry.is_directory()) {
				LOG_INFO("Scanning path: "<< entry.path());
				continue;
			}
			// scan for associated types.
			// match file extensions to asset types


			// grab normalized extension.
			std::string extension = entry.path().extension().string();

			if (extension.length() == 0) {
				LOG_INFO("file has no extension: " << entry.path().filename());
				continue;
			}


			LoadResource(entry);

			

		}
	}
	else {
		for (const fs::directory_entry& entry : fs::directory_iterator(_filePath)) {


		}
	}


}





void ResourceManager::AddResource(std::shared_ptr<Resource> _resource, std::filesystem::path _path) {
	m_resourcePool[_resource->ResourceType()].push_back(_resource);


	_resource->ResourceID(GenerateID(_resource->ResourceType()));


	LOG_INFO("Total items in resource pool: " << m_resourcePool[_resource->ResourceType()].size());
}


void ResourceManager::RemoveResource(RES_ID _id) {

}

void ResourceManager::GetResource(RES_ID _id) {

}
void ResourceManager::GetResource(std::string _resName) {

}

void ResourceManager::LoadDefaultResources() {
	// load the cube here.
	
	LOG_INFO("Do nothing");

}



void ResourceManager::PackResources() {
	LOG_INFO("Stub Function.");
}

void ResourceManager::RegisterFileExtension(std::string _extension, Resource::RESOURCE_TYPE _type) {
	LOG_INFO("Extension registered: ["<< _extension<<"]");
	m_fileExtensions[_extension] = _type;
}


void ResourceManager::DeregisterFileExtension(std::string _extension) {
	m_fileExtensions.erase(_extension);
}

void ResourceManager::LoadPaths() {
	// this function will load from the specified files listed in the json if available
	// if it can't find the json paths, it will load from default paths
	// if it can't find the default paths, it will create new paths and build from there

}

void ResourceManager::LoadResource(std::filesystem::path _filePath) {

	// grab normalized extension.
	std::string extension = _filePath.extension().string();

	if (extension.length() == 0) {
		LOG_INFO("file has no extension: " << _filePath.filename());
		return;
	}

	std::transform(extension.begin(), extension.end(), extension.begin(),
		[](unsigned char c) { return std::tolower(c); });

	if (m_fileExtensions.find(extension) == m_fileExtensions.end()) {
		LOG_INFO("File Extension for file: " << _filePath.filename()
			<< " is not registered and will be ignored. (extension: "
			<< extension
			<< ")"
		);
		return;
	}

	// add the resource here.
	LOG_INFO("Registering file " << _filePath);
	const Resource::RESOURCE_TYPE resType = m_fileExtensions.at(extension);
	// for now just load immediately.
	// TODO - figure out how to load on need.



	switch (resType) {
	case RESOURCE_TYPE::MESH:
		Mesh newMesh{};
		newMesh.UseMeshFromPath(_filePath);
		AddResource(std::make_shared<Mesh>(newMesh), _filePath);
		break;
	}





}

RES_ID ResourceManager::GenerateID(Resource::RESOURCE_TYPE _rsc) {
	unsigned idx = ++m_nextID[_rsc];
	RESOURCE_TYPE rst = _rsc;
	
	// [8-bit rst][24-bit index]
	// guid

	return idx;
}


