#pragma once
#include <arch/common/singleton.h>

class System {
public:
	
	// generic functions

	//! initialisation function - creation of a system here.
	virtual void Init()			= 0;


	virtual void Start()		{std::cout << "Start" << std::endl;};
	virtual void PreUpdate()	{std::cout << "PreU" << std::endl;};
	virtual void Update()		= 0;
	virtual void PostUpdate()	{std::cout << "PostU" << std::endl;};
	virtual void Stop()			{std::cout << "Stop" << std::endl;};
	virtual void Cleanup()		{std::cout << "Clean" << std::endl;};


	// --- 
	
};