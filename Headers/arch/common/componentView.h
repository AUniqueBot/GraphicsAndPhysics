#pragma once
#include <pch.h>



template <typename T>
class ComponentView {
	std::optional<std::reference_wrapper<T>> refOpt;
public:
	ComponentView(std::optional<std::reference_wrapper<T>> o) : refOpt(std::move(o)) {}

	explicit operator bool() const { return refOpt.has_value(); }

	T& operator*() { return refOpt->get(); }
	T* operator->() { return &refOpt->get(); }

	const T& operator*() const { return refOpt->get(); }
	const T* operator->() const { return &refOpt->get(); }
};