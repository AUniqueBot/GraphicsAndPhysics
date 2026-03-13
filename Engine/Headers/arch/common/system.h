#pragma once
#include <arch/common/singleton.h>
#include <arch/common/entity.h>




class System {
public:
	
	// generic functions

	//! initialisation function - creation of a system here.
	virtual void Init()			= 0;


	virtual void Start()		{ /*if (!DISABLELOGGING) LOG_WITH_CONTEXT("Start()");*/ }
	virtual void PreUpdate()	{ /*if (!DISABLELOGGING) LOG_WITH_CONTEXT("PreUpdate()");*/ };
	virtual void Update()		= 0;
	virtual void FixedUpdate()  { /*if (!DISABLELOGGING) LOG_WITH_CONTEXT()*/ };
	virtual void PostUpdate()	{ /*if (!DISABLELOGGING) LOG_WITH_CONTEXT("PostUpdate()");*/ };
	virtual void Stop()			{ /*if (!DISABLELOGGING) LOG_WITH_CONTEXT("Stop()");*/ };
	virtual void Cleanup()		{ /*if (!DISABLELOGGING) LOG_WITH_CONTEXT("Cleanup()");*/ };


	// --- 
private:
	
};