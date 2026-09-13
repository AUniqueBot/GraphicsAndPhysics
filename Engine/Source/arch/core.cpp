#include <pch.h>
#include <arch/core.h>

#include <arch/systems/sys_headers.h>
#include <arch/components/comp_headers.h>
#include <arch/resources/res_material_presets/res_material_lambert.h>
#include <arch/resources/res_material_presets/res_material_phong.h>
#include <arch/resources/res_material_presets/res_material_blinnphong.h>

#include <rapidjson/istreamwrapper.h>


Clock Core::m_clock {};

void Core::Init() {
	if (!IsWindowSet()) {
		LOG_WARN("Window is not set - check if Core::SetWindow is called");
		return;
	}

	CoreInit();




	// create an entity via loading here...
	AssetManager& asMgr = m_assetManager;
	MaterialManager& matMgr = asMgr.GetMaterialManager();
	MeshManager& meshMgr = asMgr.GetMeshManager();
	

	// using the default scene data.
	// test scene 1 - 4051038013427589782
	// test scene 2 - 5576267115073075432
	m_sceneManager.SetCurrentScene(5576267115073075432);
	SceneHandle scene = m_sceneManager.GetCurrentScene();
	m_sceneManager.LoadCurrentScene();
	
	// test out this stuff.	
	Entity& obj1 = *(scene->Instantiate());
	obj1.Name("Mesh Object");
	
	obj1.AddComponent<MeshRenderer>();	// object
	const auto& component = obj1.GetComponent<MeshRenderer>();
	if (component) {
		MeshHandle mesh = meshMgr.LoadMesh("./Assets/Models/sampleModel.obj");
		// need to assign mesh to meshrender, not have it initialised with the meshrenderer.
		component->SetMesh(mesh);
		BlinnPhongMaterialHandle mat = matMgr.CreateBlinnMaterial();
		mat->Color(0xaaaaeeff);
		component->AddMaterial(mat); // purposeful downcast.
	}
	obj1.GetComponent<Transform>()->Position(glm::vec3(0, 0, 2));



	//Entity& cam =  *(m_registry.Instantiate());
	//cam.Name("Camera");
	//cam.AddComponent<Camera>();			// cameara
	//cam.GetComponent<Transform>()->Position(glm::vec3(3, 4, -5));
	

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
	FixedUpdate();
	PostUpdate();
}

void Core::PreUpdate() {
	for (auto s : m_systemInstances) {
		s->PreUpdate();
	}
	m_inputRouter.RequestOwnership("Core", 50);
	m_inputSystem.PreUpdate();
}

void Core::Update() {

	m_inputSystem.Update();
	m_clock.Update();


	for (auto s : m_systemInstances) {
		s->Update();
	}
}

void Core::FixedUpdate() {

	static double currentDelta = 0;
	currentDelta += DeltaTime();

	if (currentDelta < m_fixedDeltaTime) return;
	int updateCount	{ static_cast<int>(currentDelta/m_fixedDeltaTime) };


	for (int i{}; i < updateCount; ++i) {
		for (auto s : m_systemInstances) {
			s->FixedUpdate();
		}
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
	m_resourceManager.Cleanup();
	m_gpuResourceManager.Cleanup();
	m_assetManager.Cleanup();
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
	m_resourceManager.Init(); 
	m_assetManager.Init();
	m_sceneManager.Init();

	

	// - loading of resources ----------------
	std::filesystem::path assetsPath{"./Assets"};
	m_assetManager.Load(assetsPath);
	m_sceneManager.Load(assetsPath);


}



void Core::RegisterComponents() {
	m_registry.RegisterComponent<Transform>();
	m_registry.RegisterComponent<RigidBody>();
	m_registry.RegisterComponent<Camera>();
	m_registry.RegisterComponent<MeshRenderer>();
	m_registry.RegisterComponent<Light>();
;

	// register all types here.
	m_registry.RegisterComponentsForSerialisation();
}

void Core::RegisterSystems() {
	RegisterSystem<TransformSystem>();
	RegisterSystem<RenderSystem>();
	RegisterSystem<PhysicsSystem>();
}