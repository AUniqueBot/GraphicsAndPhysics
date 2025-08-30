#include "res_resource.h"

Resource::Resource(RESOURCE_TYPE _type) 
	: m_resType{ _type } // generate a simple UID

{
	//

}

void Resource::LoadAsset() {
	if (m_isLoaded) return;
	m_isLoaded = true;



}

void Resource::UnloadAsset() {
	if (!m_isLoaded) return;
	m_isLoaded = false;
}
