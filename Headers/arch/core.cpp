#include <pch.h>
#include <arch/core.h>

#include <arch/systems/sys_headers.h>
#include <arch/components/comp_headers.h>



std::vector<System*> Core::m_systemInstances{};
EntityRegistry Core::m_registry{};


void Core::Init() {
	RegisterComponents();
	RegisterSystems();
	// initialise here.
	for (System* s : m_systemInstances) {
		s->Init();
	}
}

void Core::Start() {

}

void Core::PreUpdate() {
	for (auto s : m_systemInstances) {
		s->PreUpdate();
	}
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


	m_registry.RegisterType<Transform>();
	m_registry.RegisterType<RigidBody>();



}

void Core::RegisterSystems() {
	RegisterSystem<TransformSystem>();
	RegisterSystem<RenderSystem>();
}