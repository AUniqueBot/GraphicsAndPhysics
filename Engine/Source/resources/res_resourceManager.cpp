#include <arch/resources/res_resourceManager.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/filewritestream.h>
#include <random>
#include <algorithm>
#include <cctype>


#include <arch/resources/res_mesh/res_mesh.h>
#include <arch/resources/res_shader.h>
#include <arch/resources/res_material.h>
#include <arch/resources/res_scene.h>





void ResourceManager::Init() {
	LOG_INFO("Init ResourceManager");
	// loads a json (if any) of all possible asset paths configured by the engine.
	
	


	RegisterResourceType<MeshRes>();
	RegisterResourceType<Shader>();
	RegisterResourceType<ShaderProgram>();
	RegisterResourceType<Material>();
	RegisterResourceType<Scene>();

	
	// registering the default file extensions
	RegisterFileExtension(".stl", MeshRes::GetResourceTypeID());
	RegisterFileExtension(".obj", MeshRes::GetResourceTypeID());
	RegisterFileExtension(".frag", ShaderProgram::GetResourceTypeID());
	RegisterFileExtension(".vert", ShaderProgram::GetResourceTypeID());

	RegisterFileExtension(".material", Material::GetResourceTypeID());
	
	LoadDefaultResources();


	//ScanResourcesInPath("Assets", true); // scan for typical setup


	
}


void ResourceManager::Cleanup() {
	
	for (std::shared_ptr<BaseResource>& res : m_resourcePool.Data()) {
		res->Destroy();// final cleanup
	}
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
			//LoadResource(entry);
		}
	}
	else {
		for (const fs::directory_entry& entry : fs::directory_iterator(_filePath)) {
			std::string extension = entry.path().extension().string();
			if (extension.length() == 0) {
				LOG_INFO("file has no extension: " << entry.path().filename());
				continue;
			}
			//LoadResource(entry);
		}
	}


}


ResourceIdentifier ResourceManager::AddResourceInternal(
	std::shared_ptr<BaseResource> _resource, 
	RESTYPE_ID _type
) {
	const RESTYPE_ID type = _resource->ResourceType();
	RES_ID resId = _resource->ResourceID();
	if (resId == ResourceConstants::C_RES_INVALID_ID) {
		resId = GenerateID();
		_resource->ResourceID(resId);
	}
	ResourceIdentifier idr = GenerateResourceIdentifier(_resource);
	LOG_INFO("Registering Resource: [" << _resource->Name() << ", res-id: " << resId << "]");
	m_resourcePool.Add(std::move(_resource), resId);
	m_resourceTypeManifest[_type].push_back(resId);
	return idr;
}

ResourceIdentifier ResourceManager::GenerateResourceIdentifier(
	std::shared_ptr<BaseResource> _resource
) const {
	const ResourceIdentifier ret{
		_resource->m_resourceId,
		_resource->m_resType,
		const_cast<ResourceManager*>(this),
		_resource->m_name
	};
	return ret;
}




void ResourceManager::RemoveResource(std::string _name) {
	std::shared_ptr<BaseResource> res = GetResource(_name);
	if (!res) return;
	RemoveResource(res->ResourceID());
}

bool ResourceManager::RegisterResourceKey(RES_ID _resId, std::string _name) {
	//
	return false;
}

void ResourceManager::RemoveResource(RES_ID _id) {
	// get the resource
	const std::shared_ptr<BaseResource>& res = *m_resourcePool.At(_id);
	const std::string name = res->m_pathToAsset.filename().string();
	
	// caches to clear
	RESTYPE_ID typeId = res->ResourceType();
	res->Destroy();


	// erasing from primary containers
	m_resourcePool.Remove(_id);

	// erasing from secondary containers
	auto& resIdVector{ m_resourceTypeManifest[typeId] };
	

	// rotate and pop
	const auto& itr{ std::find_if(resIdVector.begin(), resIdVector.end(), [_id](RES_ID& a) {return a == _id; }) };
	if (itr == resIdVector.end()) return;

	std::rotate(itr, itr + 1, resIdVector.end());
	resIdVector.pop_back();
}




std::shared_ptr<BaseResource> ResourceManager::GetResource(RES_ID _id) {
	SparseSetView<std::shared_ptr<BaseResource>> itr = m_resourcePool.At(_id);
	if (!itr) return nullptr;
	return *itr;
}
std::shared_ptr<BaseResource> ResourceManager::GetResource(std::string _resName) {
	for (std::shared_ptr<BaseResource>& res : m_resourcePool.Data()) {
		if (res->m_name == _resName) return res;
	}
	return nullptr;
}

std::shared_ptr<BaseResource> ResourceManager::GetResource(ResourceIdentifier _id) {
	return GetResource(_id.m_resourceId);
}

ResourceIdentifier ResourceManager::GetResourceIdentifier(RES_ID _id) {
	SparseSetView<std::shared_ptr<BaseResource>> itr = m_resourcePool.At(_id);
	if (!itr) return ResourceIdentifier();
	return GenerateResourceIdentifier(*itr);
}

std::deque<std::shared_ptr<BaseResource>>& ResourceManager::GetResourcePool() {
	return m_resourcePool.Data();
}

const std::deque<std::shared_ptr<BaseResource>>& ResourceManager::GetResourcePool() const {
	return m_resourcePool.Data();
}


void ResourceManager::LoadDefaultResources() {
	// load the cube here.
	
	LOG_INFO("Do nothing");

}



void ResourceManager::PackResources() {
	LOG_INFO("Stub Function.");
}

void ResourceManager::ReadResourceMetafile(std::filesystem::path _path) {
	
}

const std::vector<RES_ID>& ResourceManager::GetResourcePoolManifest(RESTYPE_ID _typeId) const {
	return m_resourceTypeManifest.at(_typeId);
}

void ResourceManager::RegisterFileExtension(std::string _extension, RESTYPE_ID _type) {
	LOG_INFO("Extension registered: ["<< _extension<<"]");
	m_fileExtensions[_extension] = _type;
}


void ResourceManager::DeregisterFileExtension(std::string _extension) {
	m_fileExtensions.erase(_extension);
}

RESTYPE_ID ResourceManager::GetResourceType(std::string _extension) const {
	return 
		m_fileExtensions.find(_extension) != m_fileExtensions.end() ? 
		m_fileExtensions.at(_extension) : 
		ResourceConstants::C_RESTYPE_INVALID_ID;
}


void ResourceManager::LoadPaths() {
	// this function will load from the specified files listed in the json if available
	// if it can't find the json paths, it will load from default paths
	// if it can't find the default paths, it will create new paths and build from there

}

RES_ID ResourceManager::GenerateID() {
	RES_ID id;
	do {
		id = m_idGenerator();
	} while (m_resourcePool.At(id) && id == ResourceConstants::C_RES_INVALID_ID);
	return id;
}



RES_ID ResourceManager::GenerateTypedID(RESTYPE_ID _rsc) {
	unsigned idx = ++m_nextIDTyped[_rsc];
	RESTYPE_ID rst = _rsc;
	
	// [8-bit rst][24-bit index]
	// guid

	return idx; // return nothing for now.
}


