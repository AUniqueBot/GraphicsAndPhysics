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
};






constexpr int C_SHADOWMAP_COUNT { 6 }; // you need 6 (!) for cube maps.
struct alignas(sizeof(glm::vec4)) ShadowData {

	glm::mat4 m_lightMatrix[C_SHADOWMAP_COUNT]			{};	//
	glm::vec4 m_atlasOffsetSize[C_SHADOWMAP_COUNT]		{}; // position and size of the tile.
	glm::vec4 m_lightTypeShadowId						{}; // id is layer of arrayid.


	// get light type and shadow id.

	void SetShadowID(int _id) {
		m_lightTypeShadowId.y = static_cast<float>(_id);
	}
	int GetShadowID() const {
		return static_cast<int>(m_lightTypeShadowId.y);
	}
	void SetLightType(LightType _type) {
		m_lightTypeShadowId.x = static_cast<float>(_type);
	}
	LightType GetLightType() const {
		return static_cast<LightType>(m_lightTypeShadowId.x);
	}

	void SetMatrix(glm::mat4 _matrix, int _idx) {
		assert(_idx < C_SHADOWMAP_COUNT && _idx > -1);
		m_lightMatrix[_idx] = _matrix;
	}
	glm::mat4& GetMatrix(int _idx) {
		assert(_idx < C_SHADOWMAP_COUNT && _idx > -1);
		return m_lightMatrix[_idx];
	}
	const glm::mat4& GetMatrix(int _idx) const {
		assert(_idx < C_SHADOWMAP_COUNT && _idx > -1);
		return m_lightMatrix[_idx];
	}

	void SetAtlasOffset(const glm::vec2& _offset, int _idx) {
		assert(_idx < C_SHADOWMAP_COUNT && _idx > -1);
		m_atlasOffsetSize[_idx].x = _offset.x;
		m_atlasOffsetSize[_idx].y = _offset.y;
	}

	glm::vec2 GetAtlasOffset(int _idx) {
		assert(_idx < C_SHADOWMAP_COUNT && _idx > -1);
		return glm::vec2 { 
			m_atlasOffsetSize[_idx].x,
			m_atlasOffsetSize[_idx].y
		};
	}
	void SetAtlasSize(const glm::vec2& _tileSize, int _idx) {
		assert(_idx < C_SHADOWMAP_COUNT && _idx > -1);
		m_atlasOffsetSize[_idx].z = _tileSize.x;
		m_atlasOffsetSize[_idx].w = _tileSize.y;
	}

	glm::vec2 GetAtlasSize(int _idx) {
		assert(_idx < C_SHADOWMAP_COUNT && _idx > -1);
		return glm::vec2{
			m_atlasOffsetSize[_idx].z,
			m_atlasOffsetSize[_idx].w
		};
	}
};


constexpr unsigned C_MAX_LIGHTS			{ 10 };
struct alignas(sizeof(glm::vec4)) LightUBOData {
	LightData m_lightData[C_MAX_LIGHTS];
	int m_count{}; 
};


constexpr int C_MAX_SHADOWS				{ 10 };
struct alignas(sizeof(glm::vec4)) ShadowMapUBOData {
	ShadowData m_shadowData[C_MAX_SHADOWS];

	glm::vec4 m_directionalAtlasResAndTexelSize;
	glm::vec4 m_spotAtlasResAndTexelSize;
	glm::vec4 m_pointAtlasResAndTexelSize;
	

	int m_directionalCount;
	int m_pointCount;
	int m_spotCount;
};




class Light : public Component {


public:


	 
public:
	void Init() override {}

	const LightType& Type() const;
	void Type(const LightType& _type);
	
	float Power() const;
	void Power(float _value);

	const glm::vec3& GetColor() const;
	void SetColor(const glm::vec3& _col);

	
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

	const ShadowData& GetShadowData() const;
	ShadowData& GetShadowData();

public:
	void SetShadowDataMatrix(int level, const glm::mat4& matrix) const;
	void SetShadowDataAtlasOffset(int level, const glm::vec2& offset) const;
	void SetShadowDataAtlasSize(int level, const glm::vec2& size) const;

private:

	void UpdateLightData() const;
	void UpdateShadowData() const;

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
	mutable bool m_castShadow						{ false };
	mutable bool m_castShadowDirty					{ false };
	mutable unsigned m_shadowMapID					{ std::numeric_limits<unsigned>::max() };


	// all this would provide a struct of data to be sent for rendering.

	// -  cached data to be mutable --------------------------
	mutable LightData m_lightData					{};
	mutable bool m_lightDataMismatch				{ true };


	mutable ShadowData m_shadowData					{};
	mutable bool m_shadowDataMismatch				{};

	// ---------------------------------------------------------------------------------
	// - prop declaration ------------------------------------
	INSPECTABLE_DECLAREPROPS(Light);
	
};





