#include <pch.h>
#include <arch/core.h>

#include <arch/systems/sys_headers.h>
#include <arch/components/comp_headers.h>
#include <arch/resources/res_material_presets/res_material_lambert.h>


Clock Core::m_clock {};

void Core::Init() {
	if (!IsWindowSet()) {
		LOG_WARN("Window is not set - check if Core::SetWindow is called");
		return;
	}

	CoreInit();




	// test out this stuff.
	m_registry.name = "hi"; // for debugging purposes.
	


	Entity& obj1 = *(m_registry.Instantiate());
	Entity& ambientLight = *(m_registry.Instantiate());
	Entity& dirLight = *(m_registry.Instantiate());
	Entity& cam =  *(m_registry.Instantiate());


	
	obj1.AddComponent<MeshRenderer>();	// object
	ambientLight.AddComponent<Light>();			// ambient light
	dirLight.AddComponent<Light>();			// directional light
	cam.AddComponent<Camera>();			// cameara

	Light& _ambientLight = *(ambientLight.GetComponent<Light>());
	_ambientLight.Power(0.3);
	_ambientLight.Type(LightType::AMBIENT);

	obj1.GetComponent<Transform>()->Position(glm::vec3(0, 0, 2));
	ambientLight.GetComponent<Transform>()->Position(glm::vec3(2, 0, 4));
	cam.GetComponent<Transform>()->Position(glm::vec3(3, 4, -5));
	
	Light& _dirLight = *(dirLight.GetComponent<Light>());

	_dirLight.Type(DIRECTIONAL);
	_dirLight.Power(1);

	dirLight.GetComponent<Transform>()->Forward(glm::vec3(0.5, 1, -1));


	LambertMaterial& mat = static_cast<LambertMaterial&>(obj1.GetComponent<MeshRenderer>()->GetDefaultMaterial());
	mat.Color(0xaaaaeeff);
 	

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