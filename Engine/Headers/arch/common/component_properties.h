#pragma once
#include <string>
#include <functional>
namespace PropertyMD {
	enum class PropertyType {
		Int,
		Float,
		Double,
		Boolean,
		String,
		Color,
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
		PropertyType m_type;
		Shape m_shape;
		int m_componentCount;
		bool m_draggable;
		std::function<void(void*, void*)> m_get;
		std::function<void(void*, const void*)> m_set;

		struct List {
			std::function<int(void*)> m_size;
			std::function<void* (void*, int)> m_get;
			std::function<void(void*)> m_add;
			std::function<void(void*, int)> m_remove;

			bool m_valid = false;
		} m_list;

		Property(
			std::string name,
			PropertyType type,
			Shape shape,
			int count,
			std::function<void(void*, void*)> get,
			std::function<void(void*, const void*)> set,
			bool draggable = false
		) : m_name(std::move(name))
			, m_type(type)
			, m_shape(shape)
			, m_componentCount(count)
			, m_get(std::move(get))
			, m_set(std::move(set))
			, m_draggable(draggable)
		{
		}
	};


	template<typename T, typename Getter, typename Setter>
	Property MakeProperty(
		const char* name,
		PropertyType base,
		Shape shape,
		int count,
		Getter getter,
		Setter setter,
		bool draggable = false
	)
	{
		using ReturnT = std::invoke_result_t<Getter, const T*>;
		using ValueT = std::remove_cv_t<std::remove_reference_t<ReturnT>>;

		return Property(
			name,
			base,
			shape,
			count,

			// getter
			[getter](void* obj, void* out) {
				T* t = static_cast<T*>(obj);
				*(ValueT*)out = std::invoke(getter, t);
			},

			// setter
			[setter](void* obj, const void* value) {
				T* t = static_cast<T*>(obj);
				std::invoke(setter, t, *(const ValueT*)value);
			},
			draggable
		);
	}
}