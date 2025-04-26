#include <pch.h>
#include <arch/core.h>

#include <arch/systems/sys_headers.h>
#include <arch/components/comp_headers.h>



std::vector<System*> Core::m_systemInstances{};
EntityRegistry Core::m_registry{};


void Core::Init() {
	RegisterComponents();
	RegisterSystems();

	// test out this stuff.
	m_registry.name = "hi";
	auto& obj1 = m_registry.Instantiate().value().get();
	obj1.AddComponent<Transform>();
	obj1.AddComponent<Transform>();


	auto obj2 = m_registry.Instantiate().value();

	m_registry.PrintDebugInfo();

	

	// initialise here.
	for (System* s : m_systemInstances) {
		s->Init();
	}
}

void Core::Start() {
	for (System* s : m_systemInstances) {
		s->Init();
	}
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
	for (auto s : m_systemInstances) {
		s->Stop();
	}
}

void Core::Cleanup() {
	for (auto s : m_systemInstances) {
		s->Cleanup();
	}


}


void Core::RegisterComponents() {
	m_registry.RegisterType<Transform>();
	m_registry.RegisterType<RigidBody>();


	// register all types here.
	m_registry.RegisterComponentsForSerialisation();
}

void Core::RegisterSystems() {
	RegisterSystem<TransformSystem>();
	RegisterSystem<RenderSystem>();
}