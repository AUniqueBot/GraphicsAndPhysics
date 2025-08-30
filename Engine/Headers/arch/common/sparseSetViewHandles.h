#pragma once
#include <pch.h>
#include <arch/common/component.h>



class Entity;

template <typename T>
class SparseSetView {
	std::optional<std::reference_wrapper<T>> m_ref;

public:
	SparseSetView(std::optional<std::reference_wrapper<T>> ref) : m_ref(std::move(ref)) {}

	explicit operator bool() const { return m_ref.has_value(); }

	T& operator*() const { return m_ref->get(); }
	T* operator->() const { return &m_ref->get(); }
};


using EntityView = SparseSetView<Entity>;
using EntityViewConst = SparseSetView<const Entity>;

template <TemplateComponentType T>
using ComponentView = SparseSetView<T>;
