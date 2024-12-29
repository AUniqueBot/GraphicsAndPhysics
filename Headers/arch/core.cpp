#include <pch.h>
#include <arch/core.h>

#include <arch/systems/sys_headers.h>
#include <arch/components/comp_headers.h>



std::vector<System*> Core::m_systemInstances{};

void Core::Init() {
	RegisterComponents();

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


void Core::RegisterComponents() {
	// register components

	m_registry.RegisterType<Transform>();




	RegisterSystem<TransformSystem>();
	RegisterSystem<RenderSystem>();
}