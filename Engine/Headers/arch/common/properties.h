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

	enum class DynamicListType {
		Vector,
		SparseSet,
		None
	};

	struct Option {
		const char* label;
		int value;
	};






	struct Property {
		std::string m_name;
		PropertyType m_type;
		Shape m_shape;
		int m_componentCount;
		bool m_draggable = false;
		std::function<void(void*, void*)> m_get = nullptr;
		std::function<void(void*, const void*)> m_set = nullptr;



		bool m_isEnum = false;
		std::vector<Option> m_options;


		struct List {
			std::function<int(void*)> m_size = nullptr;
			std::function<void* (void*, int)> m_get = nullptr;
			std::function<void*(void*)> m_listAccessor = nullptr;
			std::function<void(void*)> m_add = nullptr;
			std::function<void(void*, int)> m_remove = nullptr;
			std::function<void* (void*)> m_getObject = nullptr;
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

		Property prop = Property(
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

		prop.m_isEnum = false;
		return prop;
	}

	template<typename T, typename Enum>
	Property MakeEnumProperty(
		const std::string& name,
		const Enum& (T::* getter)() const,
		void (T::* setter)(const Enum&),
		std::vector<Option> options
	)
	{
		Property prop(
			name,
			PropertyType::Int,
			Shape::Scalar,
			1,

			// GET
			[getter](void* obj, void* out) {
				T* t = static_cast<T*>(obj);
				int value = static_cast<int>((t->*getter)());
				*static_cast<int*>(out) = value;
			},

			// SET
			[setter](void* obj, const void* in) {
				T* t = static_cast<T*>(obj);
				int value = *static_cast<const int*>(in);
				(t->*setter)(static_cast<Enum>(value));
			}
		);

		prop.m_isEnum = true;
		prop.m_options = std::move(options);

		return prop;
	}

	template<typename T, typename Element>
	Property MakeListProperty(
		const char* name,
		std::function<std::vector<Element>& (T*)> listAccessor
	)
	{
		Property prop(
			name,
			PropertyType::Object,          // or Element type if you want stricter typing
			Shape::DynamicList,
			1,
			nullptr,
			nullptr
		);
		Property::List& ls{prop.m_list};

		ls.m_valid = true;
		ls.m_listAccessor = [listAccessor](void* obj) {
			T* t = static_cast<T*>(obj);
			auto& list = std::invoke(listAccessor, t);
			return &list;
		};
		ls.m_size = [listAccessor](void* obj) -> int {
			T* t = static_cast<T*>(obj);
			return (int)std::invoke(listAccessor, t).size();
			};

		ls.m_get = [listAccessor](void* obj, int index) -> void* {
			T* t = static_cast<T*>(obj);
			return &std::invoke(listAccessor, t)[index];
			};

		ls.m_add = [listAccessor](void* obj) {
			T* t = static_cast<T*>(obj);
			std::invoke(listAccessor, t).emplace_back();
			};

		ls.m_remove = [listAccessor](void* obj, int index) {
			T* t = static_cast<T*>(obj);
			auto& vec = std::invoke(listAccessor, t);
			vec.erase(vec.begin() + index);
			};
		
		


		return prop;
	}





}