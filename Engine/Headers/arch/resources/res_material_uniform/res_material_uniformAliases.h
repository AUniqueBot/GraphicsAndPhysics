#pragma once


// - camera ----------------------------------------------------
constexpr const char* U_OBJECT_MATRIX		{ "u_objectMtx" };
constexpr const char* U_CAMERA_MATRIX		{ "u_cameraMtx" };
constexpr const char* U_PROJECTION_MATRIX	{ "u_projectionMtx" };

// - light -----------------------------------------------------
constexpr const char* U_LIGHT_MATRIX		{ "u_lightMtx" };


// - texture maps ----------------------------------------------
constexpr const char* U_ALBEDO				{ "u_albedo" };
constexpr const char* U_NORMAL				{ "u_normal" };

constexpr const char* U_SPECULAR			{ "u_specular" };
constexpr const char* U_GLOSSINESS			{ "u_glossiness" };
constexpr const char* U_METALNESS			{ "u_metalness" };
constexpr const char* U_ROUGHNESS			{ "u_roughness" };

constexpr const char* U_EMISIVE				{ "u_emissive" };

// - other ------------------------------------------------------
constexpr const char* U_DELTATIME			{ "u_deltaTime" };
constexpr const char* U_OBJECTID			{ "u_objectId" };