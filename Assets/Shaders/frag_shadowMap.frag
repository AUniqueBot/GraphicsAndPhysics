in vec2 v_uv;

uniform sampler2DArray u_texture;


out vec4 out_color;

void main() {
    float depth = texture(
        u_shadowMap,
        vec3(v_uv, float(0))
    ).r;

    out_color = vec4(vec3(depth), 1.0);
    // out_color = vec4(0.03, 0.77, 0.24, 1.0);
}