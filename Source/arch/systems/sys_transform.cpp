#include "pch.h"
#include <arch/systems/sys_transform.h>

void TransformSystem::Start() {

}

void TransformSystem::Update() {
	std::cout << typeid(TransformSystem).name() << ": Update" << std::endl;
}
