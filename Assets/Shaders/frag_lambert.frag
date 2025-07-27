
#include "./UBO/LightUBO.glsl"

in vec3 frag_position;
in vec3 frag_normal;
in vec2 frag_uv;
uniform sampler2D u_albedo; // to be updated
out vec4 out_color;


vec4 lambertReflectance(vec3 _fragNormal, vec3 _lightDir, vec4 _lightCol){
	float dotProduct = max(dot(_fragNormal,_lightDir), 0);
	return dotProduct * _lightCol;
}


void main() {
	vec4 color = texture(u_albedo, frag_uv);
	// out_color = color * lambertReflectance();
	out_color = color;
}


