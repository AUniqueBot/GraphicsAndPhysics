#pragma once
#include <pch.h>
#include <arch/resources/res_resource.h>


namespace Serialization {
	struct MetafileData {
		RES_ID id{ ResourceConstants::C_RES_INVALID_ID };
		std::filesystem::path path{ };
		inline bool IsValid() const { return !path.empty() && id != ResourceConstants::C_RES_INVALID_ID; }
	};
}