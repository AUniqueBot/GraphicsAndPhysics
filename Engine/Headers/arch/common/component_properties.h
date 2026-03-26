
#include <string>
#include <functional>
namespace PropertyMD {
	enum class BaseType {
		Int,
		Unsigned,
		Float,
		Double,
		Boolean,
		String,
		// not sure how to handle these just yet.
		Object,
		Pointer
	};

	enum class Shape {
		Scalar,
		FixedArray,
		DynamicList
	};

	struct Property {

		std::string m_name;
		BaseType m_type;
		Shape m_shape;
		int m_componentCount;

		std::function<void* (void*)> m_getter;
		std::function<void(void*, const void*)> m_setter;

		struct {
			std::function<int(void*)> m_size;
			std::function<void* (void*, int)> m_get;
			std::function<void(void*)> m_add;
			std::function<void(void*, int)> m_remove;

			bool m_valid = false;
		} m_list;
	};
}