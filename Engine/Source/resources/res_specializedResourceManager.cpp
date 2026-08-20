#include <arch/resources/res_specializedResourceManager.h>
#include <arch/resources/res_resourceIdentifier.h>

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>




bool SpecializedResourceManager::Has(RES_ID _id) const { 
	return m_resourceIdPool.contains(_id); 
};
void SpecializedResourceManager::Add(RES_ID _id) { 
	m_resourceIdPool.insert(_id); 
};
void SpecializedResourceManager::Remove(RES_ID _id) { 
	m_resourceIdPool.erase(_id); 
}
std::unordered_set<RES_ID> SpecializedResourceManager::List() const {
	return m_resourceIdPool;
}
;

void SpecializedResourceManager::SetResourceAlias(RES_ID _id, std::string _alias) {
	if (_alias.empty()) {
		LOG_INFO("Provide an alias");
		return;
	}
	if (!m_resourceIdPool.contains(_id)) {
		LOG_INFO("ID is not for this resource type.");
		return;
	}

	auto itr = m_aliasToResId.find(_alias);
	if (itr != m_aliasToResId.end() && !m_aliasToResId.empty()) {
		LOG_INFO("Overwriting an existing alias, \"" << _alias << "\". ID [" << itr->second << "] -> [" << _id << "]");
		RemoveAliasForRes(_id);
	}
	m_aliasToResId[_alias] = _id;
	m_resIdToAlias[_id] = _alias;
	m_resourceManager.GetResource(_id)->Name(_alias);
}


void SpecializedResourceManager::RemoveAliasForRes(RES_ID _id) {
	std::string alias = GetAliasForRes(_id);
	if (alias.empty()) {
		LOG_INFO("No alias found for RES_ID: [" << _id << "]. Exiting.");
		return;
	}
	m_aliasToResId.erase(alias);
	m_resIdToAlias.erase(_id);
}

RES_ID SpecializedResourceManager::GetResIDFromAlias(std::string _alias) const {
	auto itr = m_aliasToResId.find(_alias);
	return itr == m_aliasToResId.end() ? ResourceConstants::C_RES_INVALID_ID : itr->second;
}


MetafileData SpecializedResourceManager::CreateMetafileData(
	const std::filesystem::path& _entry,
	std::shared_ptr<BaseResource> _res
) {
	MetafileData meta;
	meta.id = _res->ResourceID();
	meta.path = _entry;
	return meta;
}

void SpecializedResourceManager::RegisterFileExtension(std::string _ext) {
	std::transform(
		_ext.begin(), _ext.end(), 
		_ext.begin(),
		[](unsigned char c) { return std::tolower(c); }
	);
	m_registeredExtensions.insert(_ext);
}

void SpecializedResourceManager::DeregisterFileExtension(std::string _ext) {
	std::transform(
		_ext.begin(), _ext.end(),
		_ext.begin(),
		[](unsigned char c) { return std::tolower(c); }
	);
	m_registeredExtensions.erase(_ext);
}

bool SpecializedResourceManager::AcceptsFileExtension(std::string _ext) const {
	std::transform(
		_ext.begin(), _ext.end(),
		_ext.begin(),
		[](unsigned char c) { return std::tolower(c); }
	);
	return m_registeredExtensions.contains(_ext);
}



void SpecializedResourceManager::UpdateResourceMetafile(const std::filesystem::path& _file) {
	std::string ext = _file.extension().string();
	if (!m_registeredExtensions.contains(ext)) {
		LOG_ERROR("Cannot register an unsupported file extension.");
		return;
	}
	namespace fs = std::filesystem;
	fs::path metaPath = _file;
	metaPath += ".meta";
	if (!fs::exists(metaPath)) {
		GenerateResourceMetafile(_file);
	}



	
	

}

void SpecializedResourceManager::GenerateResourceMetafile(const std::filesystem::path& _metapath) {
	/*
		typical metafile format

		{
			"version": int,
			"guid": uint64
		}
		
	*/
	
	using namespace rapidjson;
	std::ofstream ofs(_metapath);
	if (ofs.fail()) {
		LOG_ERROR("Problem generating metafile...");
		ofs.close();
		return;
	}
	rapidjson::Document doc;
	doc.SetObject();
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

	RES_ID resId = m_resourceManager.GenerateID(); // create a resource id for you to use!
	doc.AddMember("version",Value().SetInt(1), allocator);
	doc.AddMember("guid",
		rapidjson::Value().SetUint64(resId),
		allocator
	);

	StringBuffer buffer;
	PrettyWriter<StringBuffer> writer(buffer);

	doc.Accept(writer);
	ofs << buffer.GetString();
	ofs.close();
	LOG_INFO("Meta file generated in: " << _metapath << "!");
}




void SpecializedResourceManager::LoadResource(const MetafileData& _data) {
}

ResourceIdentifier SpecializedResourceManager::RegisterResource(std::shared_ptr<BaseResource> _res) {
	ResourceIdentifier idr = m_resourceManager.AddInternalResource(_res);
	Add(idr.m_resourceId);
	return idr;
}

std::string SpecializedResourceManager::GetAliasForRes(RES_ID _id) const {
	auto itr = m_resIdToAlias.find(_id);
	return itr == m_resIdToAlias.end() ? "" : itr->second;
}
