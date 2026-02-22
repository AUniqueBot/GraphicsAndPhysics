#pragma once
#include <pch.h>
#include "arch_headers.h"
#include <arch/common/singleton.h>
#include <arch/ecs/ecs_fwdDecl_entityRegistry.h>
#include <arch/ecs/ecs_entityFactory.h>

#include <arch/core/core_coordinatesystem.h>
#include <arch/core/core_clock.h>
#include <arch/core/core_input.h>
#include <arch/core/core_inputRouter.h>

#include <arch/resources/res_resourceManager.h>




class Core : public Singleton<Core> {
	
public:
	
	void Init();

	void Start();


	void Run();


	void PreUpdate();
	void Update();
	void PostUpdate();


	void Stop();
	void Cleanup();
	
	EntityRegistry& GetRegistry()					{  return m_registry; }
	const EntityRegistry& GetRegistry() const		{  return m_registry; }

	EntityFactory& GetEntityFactory()			{ return m_entityFactory; }
	const EntityFactory& GetEntityFactory() const { return m_entityFactory; }


	// - window -------------------------------------
	void SetWindow(GLFWwindow* _window);
	bool IsWindowSet()							{ return m_window != nullptr; }
	GLFWwindow* GetWindow()						{ return m_window; }



	// - input system -------------------------------
	InputSystem& GetInputSystem()				{ return m_inputSystem; }
	const InputSystem& GetInputSystem()	const	{ return m_inputSystem; }

	InputRouter& GetInputRouter()				{ return m_inputRouter; }
	const InputRouter& GetInputRouter()	const	{ return m_inputRouter; }

	// - delta time ---------------------------------
	static double DeltaTime();


	ResourceManager& GetResourceManager() { return m_resourceManager; };
	const ResourceManager& GetResourceManager() const { return m_resourceManager; };






public:
	// - public parameters --------------------------

public:
	// static void AddSystem(System* newSys);

	template <std::derived_from<System> T>
	void RegisterSystem() {
		m_systemInstances.push_back(&T::GetInstance());
	}


	template <std::derived_from<System> T>
	T* GetSystem() {
		for (System* sys : m_systemInstances) {
			if (auto* casted = dynamic_cast<T*>(sys)) {
				return casted;
			}
		}
		return nullptr;
	}

private:

	void CoreInit();

	void RegisterComponents();
	void RegisterSystems();
	

	GLFWwindow* m_window{};

	std::vector<System*> m_systemInstances;



	// - modules ------------------------------------
	InputSystem m_inputSystem;
	InputRouter m_inputRouter;
	EntityRegistry m_registry;
	EntityFactory m_entityFactory			{ m_registry };
	ResourceManager m_resourceManager;
	CoordinateSystem m_coordinateSystem;
	static Clock m_clock;


};