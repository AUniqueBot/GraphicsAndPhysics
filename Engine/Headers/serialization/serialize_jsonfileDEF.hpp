#include <serialization/serialize_jsonfile.h>

namespace Serialization {

	template<typename T> struct JsonTypeTraits;
	template<> struct JsonTypeTraits<int> { static bool Is(const rapidjson::Value& v) { return v.IsInt(); } };
	template<> struct JsonTypeTraits<bool> { static bool Is(const rapidjson::Value& v) { return v.IsBool(); } };
	template<> struct JsonTypeTraits<double> { static bool Is(const rapidjson::Value& v) { return v.IsDouble(); } };
	template<> struct JsonTypeTraits<std::string> { static bool Is(const rapidjson::Value& v) { return v.IsString(); } };



	template <typename T>
	std::optional<T> JSONFile::Get(const char* _itemName) {
		if (!HasMember(_itemName)) return std::nullopt;
		const auto& value = m_document[_itemName];
		
		if (!JsonTypeTraits<T>::Is(value)) return std::nullopt;

		if constexpr (std::is_same_v<T, std::string>) {
			return std::string(value.GetString()); // Copy out of JSON buffer safely
		}
		else {
			return value.Get<T>(); // Uses RapidJSON's internal template getter
		}

	}
}