#pragma once
#include <pch.h>
#include <arch/common/component.h>

enum LightType {
	POINT,
	AREA,
	SPOT,
	DIRECTIONAL,
	AMBIENT,
	SUN
};

struct LightData {
	// packed data to be sent to UBO for extensive testing
	glm::vec4 m_position_type;	// x,y,z - position,  w - type
	glm::vec4 m_direction_roll;	// x,y,z - direction, w - roll
	glm::vec4 m_color_power;	// x,y,z - color,	  w - power
	glm::vec4 m_attenuation;	// x,y - attenuation, z,w - padding


	void SetPosition(const glm::vec3& pos) { m_position_type = glm::vec4(pos, m_position_type.w); }
	void SetType(float type) { m_position_type.w = type; }

	void SetDirection(const glm::vec3& dir) { m_direction_roll = glm::vec4(dir, m_direction_roll.w); }
	void SetRoll(float roll) { m_direction_roll.w = roll; }

	void SetColor(const glm::vec3& color) { m_color_power = glm::vec4(color, m_color_power.w); }
	void SetPower(float power) { m_color_power.w = power; }

	void SetAttenuation(const glm::vec2& att) { m_attenuation.x = att.x; m_attenuation.y = att.y; }
	// Padding z,w can be left alone or zeroed
};



class Light : public Component {


public:



public:
	Light() { Component::SetComponentID(LIGHT); }
	void Init() override {}

	LightType Type() const;
	void Type(LightType _type);
	
	float Power() const;
	void Power(float _value);

	glm::vec3 Color() const;
	void Color(glm::vec3 _col);

	LightData GetLightData() const;


private:

	void UpdateLightData() const;

private:
	// generic values
	LightType m_lightType		{ POINT };				//
	float m_power				{ 1.0f };				// kW
	glm::vec3 m_color			{ 1.0f, 1.0f, 1.0f };	// 

	// non sun/directional
	// saving strategy - contain only data for the light type.
	// if you're saving sun data, only save the sun data.
	glm::vec2 m_range			{ 10.f, 100.f };		// x - minimum attenuation, y - max attenuation
	

	// area light
	glm::vec2 m_dimensions		{ 1.0f, 1.0f };
	bool m_isWindow				{ false };


	// spotlight
	float m_angle				{ 25.0f };				// in degrees.


	// all this would provide a struct of data to be sent for rendering.

	// -  cached data to be mutable --------------------------
	mutable LightData m_lightData		{};
	mutable bool m_lightDataMismatch	{ true };

};




// ---------------------------------------------------------------------------------

