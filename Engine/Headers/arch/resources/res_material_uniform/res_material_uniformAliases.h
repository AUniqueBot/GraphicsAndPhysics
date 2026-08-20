#pragma once


namespace ShaderUniformConstants {

	// - light -----------------------------------------------------
	inline constexpr const char* U_OBJECT_MATRIX			{ "u_objectMtx" };
	inline constexpr const char* U_LIGHT_MATRIX				{ "u_lightMtx" };
	inline constexpr const char* U_SHADOWMAP				{ "u_shadowMap"};
	inline constexpr const char* U_DIRECTIONALSHADOWMAP		{ "u_directionalShadowMap" };
	inline constexpr const char* U_SPOTSHADOWMAP			{ "u_spotLightShadowMap" };
	inline constexpr const char* U_POINTSHADOWMAP			{ "u_pointLightShadowMap" };


	// - texture maps ----------------------------------------------
	inline constexpr const char* U_ALBEDO					{ "u_albedo" };
	inline constexpr const char* U_NORMAL					{ "u_normal" };

	inline constexpr const char* U_SPECULAR					{ "u_specular" };
	inline constexpr const char* U_GLOSS					{ "u_gloss" };
	inline constexpr const char* U_METALNESS				{ "u_metalness" };
	inline constexpr const char* U_ROUGHNESS				{ "u_roughness" };

	inline constexpr const char* U_EMISIVE					{ "u_emissive" };

	// - phong -----------------------------------------------------
	inline constexpr const char* U_EXPONENT					{ "u_exponent" };

	// - other ------------------------------------------------------
	inline constexpr const char* U_DELTATIME				{ "u_deltaTime" };
	inline constexpr const char* U_OBJECTID					{ "u_objectId" };
}

namespace MaterialLookupConstants {
	inline constexpr const char* C_MAT_U_ALBEDO_COL			{ "albedo_col" };
	inline constexpr const char* C_MAT_U_ALBEDO_TEX			{ "albedo_tex" };
	
	inline constexpr const char* C_MAT_U_SPECULARCOL		{ "specular_col" };
	inline constexpr const char* C_MAT_U_SPECULARTEX		{ "specular_tex" };
	
	inline constexpr const char* C_MAT_U_EXPONENT			{ "exponent_val" };
};