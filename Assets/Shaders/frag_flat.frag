#version 460 core
in vec4 vtxPos;


in vec3 fragPos;
in vec2 oUv;
in vec3 oNml;

out vec4 outCol;

uniform vec4 colour;

uniform bool useTex1;
uniform sampler2D tex1;
uniform bool useTex2;
uniform sampler2D tex2;


void main() {
	outCol = colour;
	if (useTex1 == true && useTex2 == true) {
		outCol = mix(texture(tex2, oUv), texture(tex1, vec2(1 - oUv.x, oUv.y)), 0.2) * vec4(oUv, 1, 1);
	}
}
