#include "pch.h"
#include <arch/systems/sys_transform.h>
#include <arch/components/comp_transform.h>

void TransformSystem::Start() {
	std::cout << typeid(TransformSystem).name() << ": Start" << std::endl;
}

void TransformSystem::Update() {
	// std::cout << typeid(TransformSystem).name() << ": Update" << std::endl;
}
