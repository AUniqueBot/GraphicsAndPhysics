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
	m_registry.name = "hi"; // for debugging purposes.
	


	std::reference_wrapper<Entity> obj1R = m_registry.Instantiate().value();
	std::reference_wrapper<Entity> obj2R = m_registry.Instantiate().value();
	Entity& obj1 = obj1R.get();
	Entity& obj2 = obj2R.get();

	// adding and removing
	obj1.AddComponent<Transform>();
	obj2.AddComponent<Transform>();

	obj1.GetComponent<Transform>().value().get().Position(glm::vec3(0, 1, 2));
	obj2.GetComponent<Transform>().value().get().Position(glm::vec3(3, 4, 5));
	

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
	m_registry.RegisterType<Camera>();


	// register all types here.
	m_registry.RegisterComponentsForSerialisation();
}

void Core::RegisterSystems() {
	RegisterSystem<TransformSystem>();
	RegisterSystem<RenderSystem>();
	
}