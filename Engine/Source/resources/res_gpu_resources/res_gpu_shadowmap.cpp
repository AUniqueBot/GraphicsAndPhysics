#include <arch/resources/res_gpu_resources/res_gpu_shadowmap.h>


void GPU_Shadowmap::Create() {
	GPU_Texture::Create();
	m_fbo.Create();


}

void GPU_Shadowmap::Destroy() {
	GPU_Texture::Destroy();
	m_fbo.Destroy();
}


// ---------------------------------------------------------
int GPU_Shadowmap::AssignID() {
	

	++m_usageCount;
	if (!m_freeIds.empty()) {
		int newId = m_freeIds.front();
		m_freeIds.pop();
		return newId;
	}
	return ++m_idCounter;
}

void GPU_Shadowmap::ReclaimID(int _id)
{
}

