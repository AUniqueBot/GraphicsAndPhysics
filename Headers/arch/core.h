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

private:
	std::vector<System> m_systemInstances;
};