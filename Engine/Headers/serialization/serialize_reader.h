#pragma once
#include <pch.h>
#include <arch/ecs/ecs_entityFactory.h>

namespace Serialization {

	namespace fs = std::filesystem;
	class Reader {
	public:
		Reader(fs::path _path = "");
		void SetFolderPath(fs::path _path);

		void LoadJSONFile(fs::path _path, bool _isRelative = false);

	private:
		std::filesystem::path m_path; // path to a directory.
		// some sort of data here.



	};
}