#pragma once
#include <arch/common/singleton.h>

class System : public Singleton<System>{
public:
	
	// generic functions

	//! initialisation function - creation of a system here.
	virtual void Init()			{};


	virtual void Start()		{};
	virtual void PreUpdate()	{};
	virtual void Update()		{};
	virtual void PostUpdate()	{};
	virtual void Stop()			{};
	virtual void Cleanup()		{};


	// --- 
	
};