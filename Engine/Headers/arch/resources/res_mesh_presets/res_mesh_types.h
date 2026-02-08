#pragma once
#include <arch/resources/res_mesh_presets/res_mesh_cube.h>
#include <arch/resources/res_mesh_presets/res_mesh_sphere.h>
#include <arch/resources/res_mesh_presets/res_mesh_plane.h>


enum Primitive {
	CUBE,
	SPHERE,
	PLANE,
	ICOSPHERE,
	__COUNT
};