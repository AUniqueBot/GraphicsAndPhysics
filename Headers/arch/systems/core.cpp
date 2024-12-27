#include <pch.h>
#include <arch/core.h>

#include <arch/systems/sys_transform.h>



std::vector<System*> Core::m_systemInstances{};

void Core::Init() {

	RegisterSystem<TransformSystem>();


	// initialise here.
	for (System* s : m_systemInstances) {
		s->Init();
	}
}

void Core::Start() {
}

void Core::PreUpdate() {

}

void Core::Update() {

	for (auto s : m_systemInstances) {
		s->Update();
	}
	

}

void Core::PostUpdate() {
}

void Core::Stop() {
}

void Core::Cleanup() {
}


