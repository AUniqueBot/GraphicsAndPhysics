#pragma once
#include <arch/resources/res_resourceManager.h>


struct MetafileData {
	RES_ID id{ ResourceConstants::C_RES_INVALID_ID };
	std::filesystem::path path{ };
	bool IsValid() const { return !path.empty() && id != ResourceConstants::C_RES_INVALID_ID; }
};