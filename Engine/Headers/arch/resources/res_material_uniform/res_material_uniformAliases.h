#pragma once



// - light -----------------------------------------------------
constexpr const char* U_OBJECT_MATRIX			{ "u_objectMtx" };
constexpr const char* U_LIGHT_MATRIX			{ "u_lightMtx" };
constexpr const char* U_SHADOWMAP				{ "u_shadowMap"};
constexpr const char* U_DIRECTIONALSHADOWMAP	{ "u_directionalShadowMap" };
constexpr const char* U_SPOTSHADOWMAP			{ "u_spotLightShadowMap" };
constexpr const char* U_POINTSHADOWMAP			{ "u_pointLightShadowMap" };


// - texture maps ----------------------------------------------
constexpr const char* U_ALBEDO					{ "u_albedo" };
constexpr const char* U_NORMAL					{ "u_normal" };

constexpr const char* U_SPECULAR				{ "u_specular" };
constexpr const char* U_GLOSS					{ "u_gloss" };
constexpr const char* U_METALNESS				{ "u_metalness" };
constexpr const char* U_ROUGHNESS				{ "u_roughness" };

constexpr const char* U_EMISIVE					{ "u_emissive" };



// - other ------------------------------------------------------
constexpr const char* U_DELTATIME				{ "u_deltaTime" };
constexpr const char* U_OBJECTID				{ "u_objectId" };