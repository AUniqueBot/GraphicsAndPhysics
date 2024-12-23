#pragma once
#include <pch.h>
#include "arch_headers.h"



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
	static std::vector<System*> m_systemInstances;
};