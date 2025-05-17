#pragma once
#include <pch.h>
#include <arch/common/component.h>


class Light : public Component {


public:
	enum LightType {
		POINT,
		AREA,
		SPOT,
		DIRECTIONAL,
		SUN
	};
public:
	Light() { Component::SetComponentID(LIGHT); }
	void Init() override {}

	LightType Type() const		{ return m_lightType; }
	
	float Power()	const		{ return m_power; }
	void Power(float _value)	{ m_power = _value; }

	glm::vec3 Colour() const	{ return m_color; }
	void Colour(glm::vec3 _col) { m_color = _col; }


	
private:
	// generic values
	LightType m_lightType	{ POINT };
	float m_power			{ 1.0f };				// kW
	glm::vec3 m_color		{ 1.0f, 1.0f, 1.0f };	//

	// non sun/directional
	// saving strategy - contain only data for the light type.
	// if you're saving sun data, only save the sun data.
	glm::vec2 m_range		{ 10.f, 100.f }; // x - minimum attenuation, y - max attenuation
	

	// area light
	glm::vec2 m_dimensions	{ 1.0f, 1.0f };
	bool m_isWindow			{};


	// spotlight
	float m_angle			{};

	// all this would provide a struct of data to be sent for batch rendering.



};




// ---------------------------------------------------------------------------------

