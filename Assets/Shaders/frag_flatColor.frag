


in vec3 frag_position;
in vec2 frag_uv;
uniform sampler2D u_albedo; // to be updated
out vec4 out_color;


void main() {
	vec4 color = texture(u_albedo, frag_uv);
	out_color = color;
}