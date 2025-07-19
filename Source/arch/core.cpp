#include <pch.h>
#include <arch/core.h>

#include <arch/systems/sys_headers.h>
#include <arch/components/comp_headers.h>



Clock Core::m_clock {};

void Core::Init() {
	if (!IsWindowSet()) {
		LOG_WARN("Window is not set - check if Core::SetWindow is called");
		return;
	}

	CoreInit();




	// test out this stuff.
	m_registry.name = "hi"; // for debugging purposes.
	


	std::reference_wrapper<Entity> obj1R = m_registry.Instantiate().value();
	std::reference_wrapper<Entity> obj2R = m_registry.Instantiate().value();
	std::reference_wrapper<Entity> camR = m_registry.Instantiate().value();
	Entity& obj1 = obj1R.get();
	Entity& obj2 = obj2R.get();
	Entity& cam = camR.get();




	// adding and removing
	obj1.AddComponent<Transform>();
	obj2.AddComponent<Transform>();
	cam.AddComponent<Transform>();
	
	obj1.AddComponent<MeshRenderer>();	// object
	obj2.AddComponent<Light>();			// light
	cam.AddComponent<Camera>();			// cameara




	obj1.GetComponent<Transform>().value().get().Position(glm::vec3(0, 0, 2));
	obj2.GetComponent<Transform>().value().get().Position(glm::vec3(0, 0, 0));
	cam.GetComponent<Transform>().value().get().Position(glm::vec3(3, 4, -5));
	

	
	

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

void Core::Run() {
	PreUpdate();
	glfwPollEvents();
	Update();
	PostUpdate();
}

void Core::PreUpdate() {
	for (auto s : m_systemInstances) {
		s->PreUpdate();
	}
}

void Core::Update() {

	m_inputSystem.Update();
	m_clock.Update();


	for (auto s : m_systemInstances) {
		s->Update();
	}
}

void Core::PostUpdate() {
	m_inputSystem.PostUpdate();
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

void Core::SetWindow(GLFWwindow* _window) {
	if (_window == m_window) return;
	m_window = _window;
	if (!IsWindowSet()) {
		LOG_INFO("note window is unset");
	}
}

double Core::DeltaTime() {
	return m_clock.DeltaTime();
}



void Core::CoreInit() {
	RegisterComponents();
	RegisterSystems();


	// - input -------------------------------
	glfwSetWindowUserPointer(m_window, this);
	m_inputSystem.Init(m_window);

}

void Core::RegisterComponents() {
	m_registry.RegisterType<Transform>();
	m_registry.RegisterType<RigidBody>();
	m_registry.RegisterType<Camera>();
	m_registry.RegisterType<MeshRenderer>();
	m_registry.RegisterType<Light>();


	// register all types here.
	m_registry.RegisterComponentsForSerialisation();
}

void Core::RegisterSystems() {
	RegisterSystem<TransformSystem>();
	RegisterSystem<RenderSystem>();
}