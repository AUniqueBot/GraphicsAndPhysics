#pragma once
#include <pch.h>
#include <arch/resources/res_resource.h>

namespace Serialization {
	class AssetSerializer {

    public:
		virtual ~AssetSerializer() = default;
        virtual bool Serialize(
            const BaseResource& resource,
            const std::filesystem::path& path
        ) = 0;

        virtual bool Deserialize(
            BaseResource& resource,
            const std::filesystem::path& path
        ) = 0;
	};
}