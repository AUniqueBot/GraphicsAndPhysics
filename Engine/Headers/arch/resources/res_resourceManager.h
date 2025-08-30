#pragma once
#include <pch.h>
#include <arch/resources/res_resource.h>

class ResourceManager {

private:

public:
	void AddAsset(std::filesystem::path _path);
	void RemoveAsset(); 

public:

private:
	using RESOURCE_TYPE = Resource::RESOURCE_TYPE;
	std::unordered_map<RESOURCE_TYPE, std::vector<std::shared_ptr<Resource>>> m_resourcePool;
};




/*

	res manager
	- 



*/