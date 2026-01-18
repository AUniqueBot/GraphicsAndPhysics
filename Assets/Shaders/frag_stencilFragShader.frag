uniform usampler2D u_objectIdBuffer;
in vec2 vUV;
out vec4 out_color;


void main() {
	uint id = texture(uObjectIdBuffer, vUV).r;
}