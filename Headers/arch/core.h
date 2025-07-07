#pragma once
#include <pch.h>
#include "arch_headers.h"
#include <arch/common/singleton.h>
#include <arch/ecs/ecs_fwdDecl_entityRegistry.h>

#include <arch/core/core_coordinatesystem.h>

class Core : public Singleton<Core> {
	
public:
	
	void Init();

	void Start();

	void PreUpdate();
	void Update();
	void PostUpdate();


	void Stop();
	void Cleanup();
	
	EntityRegistry& Registry()					{  return m_registry; }
	// - window -------------------------------------


public:
	GLFWwindow* m_window;


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
	

	EntityRegistry m_registry;
	CoordinateSystem m_coordinateSystem;

	std::vector<System*> m_systemInstances;




};