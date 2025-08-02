in vec3 frag_position;
in vec3 frag_normal;
in vec2 frag_uv;
uniform sampler2D u_albedo;
uniform float u_deltaTime;
out vec4 out_color;

/*
*/
layout (std140, binding=0) uniform LightBlock {
	int m_count;
	int var2;
	int _pad[2];
};


void main() {
	vec4 color = texture(u_albedo, frag_uv);

	float debug = float(m_count) * 0.0 + float(var2) * 0.0;
	// out_color = vec4(0, 0, var2, 1);
	out_color = color + vec4(m_count, var2, u_deltaTime, 1);

	
}


