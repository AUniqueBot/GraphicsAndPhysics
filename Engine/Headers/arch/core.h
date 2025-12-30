#pragma once
#include <pch.h>
#include "arch_headers.h"
#include <arch/common/singleton.h>
#include <arch/ecs/ecs_fwdDecl_entityRegistry.h>

#include <arch/core/core_coordinatesystem.h>
#include <arch/core/core_clock.h>
#include <arch/core/core_input.h>

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
	
	EntityRegistry& Registry()					{  return m_registry; }
	const EntityRegistry& Registry() const		{  return m_registry; }

	// - window -------------------------------------
	void SetWindow(GLFWwindow* _window);
	bool IsWindowSet()							{ return m_window != nullptr; }
	GLFWwindow* GetWindow()						{ return m_window; }



	// - input system -------------------------------
	InputSystem& GetInputSystem()				{ return m_inputSystem; }

	// - delta time ---------------------------------
	static double DeltaTime();


	ResourceManager& ResManager() { return m_resourceManager; };
	const ResourceManager& ResManager() const { return m_resourceManager; };




public:
	// - public parameters --------------------------

public:
	// static void AddSystem(System* newSys);

	template <typename T>
	std::enable_if_t<std::is_base_of_v<System, T>, void> RegisterSystem() {
		m_systemInstances.push_back(&T::GetInstance());
	}


private:

	void CoreInit();

	void RegisterComponents();
	void RegisterSystems();
	

	GLFWwindow* m_window;

	std::vector<System*> m_systemInstances;



	// - modules ------------------------------------
	InputSystem m_inputSystem;
	EntityRegistry m_registry;
	ResourceManager m_resourceManager;
	CoordinateSystem m_coordinateSystem;
	static Clock m_clock;


};