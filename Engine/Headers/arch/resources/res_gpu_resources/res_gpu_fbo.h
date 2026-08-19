#pragma once
#include <arch/resources/res_gpu_resources/res_gpu_resource.h>

class GPU_FrameBufferObject : public GPU_Resource {

public: 
	void Create() override;
	void Destroy() override;
public: 
	void SetBindLocation(int _location);

private:
	std::set<int> m_usedBindLocations;
};