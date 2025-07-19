#pragma once
#include <pch.h>
#include <arch/components/componentList.h>


class Component {
	
public:


	virtual void Init() { LOG_INFO("Initialised Component"); };
	virtual void End()	{};

	// not intended to be set outside.
	void SetComponentID(ComponentType _type)	{ m_type = _type; }
	ComponentType GetComponentID() const		{ return m_type; }
	



	// serialization function
	static void Register() { LOG_INFO("Registering Component - Generic. If you see this, you didn't override this in your component."); };
private:
	ComponentType m_type						{ __INVALID };
	

};