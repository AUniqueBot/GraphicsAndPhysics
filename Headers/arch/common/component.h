#pragma once
#include "../components/componentList.h"

class Component {
	
public:


	virtual void Init() {};
	virtual void End()	{};

	// not intended to be set outside.
	void SetComponentID(ComponentType _type)	{ m_type = _type; }
	ComponentType GetComponentID() const		{ return m_type; }

private:
	ComponentType m_type						{ __INVALID };


};