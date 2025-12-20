#pragma once
#include <pch.h>
#include <arch/resources/res_resource.h>

class ResourceManager {

private:

public:


public:
	void Load();
	void Unload();

private:
	using RESOURCE_TYPE = Resource::RESOURCE_TYPE;
	std::unordered_map<RESOURCE_TYPE, std::vector<std::shared_ptr<Resource>>> m_resourcePool;

};

