#include <serialization/serialize_helperfunctions.h>

namespace Serialization {

	std::filesystem::path GetUniquePath(const std::filesystem::path& path) {
		if (!std::filesystem::exists(path)) {
			return path;
		}

		std::filesystem::path parent = path.parent_path();
		std::string stem = path.stem().string();
		std::string extension = path.extension().string();

		int counter = 1;

		while (true) {
			std::filesystem::path candidate =
				parent / (stem + " (" + std::to_string(counter) + ")" + extension);

			if (!std::filesystem::exists(candidate)) {
				return candidate;
			}

			++counter;
		}

	}
}