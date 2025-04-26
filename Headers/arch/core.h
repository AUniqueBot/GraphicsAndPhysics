#pragma once
#include <pch.h>
#include "arch_headers.h"
#include <arch/ecs/ecs_fwdDecl_entityRegistry.h>


class Core {
	
public:
	
	void Init();

	void Start();

	void PreUpdate();
	void Update();
	void PostUpdate();


	void Stop();
	void Cleanup();
	

public:

	

	// static void AddSystem(System* newSys);

	template <typename T>
	static std::enable_if_t<std::is_base_of_v<System, T>, void> RegisterSystem() {
		m_systemInstances.push_back(&T::GetInstance());
	}

private:
	void RegisterComponents();
	void RegisterSystems();
	static EntityRegistry m_registry;
	static std::vector<System*> m_systemInstances;

};