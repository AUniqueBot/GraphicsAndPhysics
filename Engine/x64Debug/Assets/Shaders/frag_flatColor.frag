
in vec3 frag_position;
in vec2 frag_uv;
uniform sampler2D u_albedo; // to be updated
out vec4 out_color;

/*
layout (std140, binding=0) uniform LightBlock {
	int m_count;
	int var2;
	int _pad[2];
};
*/


void main() {
	vec4 color = texture(u_albedo, frag_uv);
	out_color = color;
	// out_color = vec4(0, 0, var2, 1);
}