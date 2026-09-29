#include "arch/resources/res_resource.h"

BaseResource::BaseResource(RESTYPE_ID _type) 
	: m_resType{ _type }
{

}

void BaseResource::LoadAsset() {
	if (m_isLoaded) return;
	m_isLoaded = true;
}

void BaseResource::LoadAsset(std::filesystem::path _path) {

}

void BaseResource::UnloadAsset() {
	if (!m_isLoaded) return;
	m_isLoaded = false;
}

bool BaseResource::IsAssetLoaded() const {
	return m_isLoaded;
}

RES_ID BaseResource::ResourceID() const {
	return m_resourceId;
}
void BaseResource::ResourceID(RES_ID _newId) {
	m_resourceId = _newId;
}


std::filesystem::path BaseResource::ResourcePath() const {
	return m_pathToAsset;
}

void BaseResource::ResourcePath(std::filesystem::path _path) {
	m_pathToAsset = _path;
	
}
const std::string& BaseResource::Name() const {
	return m_name;
}
void BaseResource::Name(const std::string& _name) {
	m_name = _name;
}

std::vector<PropertyMD::Property>& BaseResource::GetProps() {
	using namespace PropertyMD;
	static std::vector<Property> props{
		MakeProperty<BaseResource>(
			"Name",
			PropertyType::String,
			Shape::Scalar,
			1,
			static_cast<const std::string & (BaseResource::*)()const>(&BaseResource::Name),
			static_cast<void(BaseResource::*)(const std::string&)>(&BaseResource::Name)
		)
	};
	return props;
}
