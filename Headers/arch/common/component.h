#pragma once
#include "../components/componentList.h"

class Component {
	
public:


	virtual void Init() { std::cout << typeid(Component).name() << ": Initialised" << std::endl; };
	virtual void End()	{};

	// not intended to be set outside.
	void SetComponentID(ComponentType _type)	{ m_type = _type; }
	ComponentType GetComponentID() const		{ return m_type; }
	



	// serialization function
	static void Register() { std::cout << typeid(Component).name() << ":Register Generic" << std::endl; };
private:
	ComponentType m_type						{ __INVALID };
	

};