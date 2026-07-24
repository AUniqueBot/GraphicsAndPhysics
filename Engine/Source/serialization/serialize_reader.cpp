#include <serialization/serialize_reader.h>
#include <rapidjson/reader.h>


namespace Serialization {
	Reader::Reader(std::filesystem::path _path) : m_path { _path } {
		SetFolderPath(_path);
	}


	void Reader::SetFolderPath(std::filesystem::path _path) {
		namespace fs = std::filesystem;
		m_path = _path;
		if (!fs::is_directory(m_path)) {
			m_path = m_path.parent_path();
		}

	}

	void Reader::LoadJSONFile(fs::path _path, bool _isRelative) {
		namespace fs = std::filesystem;
		fs::path filePath = m_path;
		if (!_isRelative) {
			filePath = _path;
		}
		else {
			// figure something out.
		}
		

		
		
		

	}



}
