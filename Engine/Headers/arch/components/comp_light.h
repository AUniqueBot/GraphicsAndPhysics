#pragma once
#include <pch.h>
#include <arch/common/component.h>
#include <arch/components/comp_meshrenderer.h>
#include <arch/systems/sys_render_modules/sys_render_renderTarget.h>

enum LightType {
	POINT = 0,
	SPOT = 1,
	DIRECTIONAL = 2,
	AMBIENT = 3,
};

struct alignas(sizeof(glm::vec4)) LightData {
	// packed data to be sent to UBO for extensive testing
	glm::vec4 m_position_type		{};	// x,y,z - position,  w - type
	glm::vec4 m_direction			{};	// x,y,z - direction, w - roll
	glm::vec4 m_color_power			{};	// x,y,z - color,	  w - power
	glm::vec4 m_attenuation			{};	// x,y - attenuation, z,w - padding
	mutable glm::mat4 m_matrix		{};
	glm::vec4 m_shadowId			{}; // needs + 3 more

	void SetPosition(const glm::vec3& pos) { 
		m_position_type = glm::vec4(pos, m_position_type.w); 
	}
	void SetType(float type) { m_position_type.w = type; }

	void SetDirection(const glm::vec3& dir) { 
		m_direction = glm::vec4(dir, m_direction.w); 
	}
	void SetRoll(float roll) { m_direction.w = roll; }

	void SetColor(const glm::vec3& color) { m_color_power = glm::vec4(color, m_color_power.w); }
	void SetPower(float power) { m_color_power.w = power; }

	void SetAttenuation(const glm::vec2& att) { m_attenuation.x = att.x; m_attenuation.y = att.y; }

	void SetID(unsigned id) { 
		m_shadowId.x = id == std::numeric_limits<unsigned>::max() ? -1.f :  static_cast<float>(id); 
	}

	void SetMatrix(glm::mat4 mat) { m_matrix = mat; }
};


constexpr unsigned C_MAX_LIGHTS{ 10 };
struct alignas(sizeof(glm::vec4)) LightUBOData {
	//glm::vec3 _pad;
	LightData m_lightData[C_MAX_LIGHTS];
	int m_count{}; 
};

struct alignas(16) ShadowMapUBOData {
	glm::ivec2 m_framebufferSize;
	glm::ivec2 m_baseTileSize;
	int m_lodCount; // each lod causes dim >> 2.
	glm::ivec3 _pad;
};




class Light : public Component {


public:


	 
public:
	void Init() override {}

	LightType Type() const;
	void Type(LightType _type);
	
	float Power() const;
	void Power(float _value);

	const glm::vec3& Color() const;
	void Color(glm::vec3 _col);

	
	const LightData& GetLightData() const;
	LightData& GetLightData();

	void RenderShadow( 
		const MeshRenderer& _mr, 
		const glm::mat4& _objectMatrix
	) const;

	// -- shadows ----------------------------------------  
	bool GetCastShadow() const;
	void SetCastShadow(bool _cast);
	
	void InvalidateShadowMapID() const;
	void SetShadowMapID(unsigned _id) const;
	unsigned GetShadowMapID() const;
	

	bool CastShadowDirty() const;
	void CleanCastShadow() const;

private:

	void UpdateLightData() const;

private:
	// generic values
	LightType m_lightType							{ POINT };				//
	float m_power									{ 1.0f };				// kW
	glm::vec3 m_color								{ 1.0f, 1.0f, 1.0f };	// 

	// non sun/directional
	// saving strategy - contain only data for the light type.
	// if you're saving sun data, only save the sun data.
	glm::vec2 m_range								{ 10.f, 100.f };		// x - minimum attenuation, y - max attenuation
	

	// area light
	glm::vec2 m_dimensions							{ 1.0f, 1.0f };
	bool m_isWindow									{ false };


	// spotlight
	float m_angle									{ 25.0f };				// in degrees.

	// shadows
	mutable bool m_castShadow								{ false };
	mutable bool m_castShadowDirty							{ false };
	mutable unsigned m_shadowMapID							{ std::numeric_limits<unsigned>::max() };


	// all this would provide a struct of data to be sent for rendering.

	// -  cached data to be mutable --------------------------
	mutable LightData m_lightData					{};
	mutable bool m_lightDataMismatch				{ true };


};




// ---------------------------------------------------------------------------------

