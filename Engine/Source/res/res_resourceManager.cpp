#include <arch/resources/res_resourceManager.h>
#include <random>


ResourceManager::ResourceManager() {
	
}

void ResourceManager::AddResource(std::shared_ptr<Resource> _resource, std::filesystem::path _path) {
	m_resourcePool[_resource->ResourceType()].push_back(_resource);

	
	_resource->ResourceID(GenerateID(_resource->ResourceType()));

	
}


void ResourceManager::Init() {
	LOG_INFO("Init ResourceManager");
	// loads a json (if any) of all possible asset paths configured by the engine.
	
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
			if (entry.is_directory()) {
				LOG_INFO("Scanning path: "<< entry.path());
			}
			// scan for associated types.
			// match 
		}
	}
	else {
		for (const fs::directory_entry& entry : fs::directory_iterator(_filePath)) {


		}
	}


}


void ResourceManager::RemoveResource(RES_ID _id) {

}

void ResourceManager::GetResource(RES_ID _id) {

}

void ResourceManager::PackResources() {
	LOG_INFO("Stub Function.");
}

void ResourceManager::LoadPaths() {
	// find a specific .
	



}

RES_ID ResourceManager::GenerateID(Resource::RESOURCE_TYPE _rsc) {
	unsigned idx = ++m_nextID[_rsc];
	RESOURCE_TYPE rst = _rsc;
	
	// [8-bit rst][24-bit index]
	// guid

	return idx;
}


