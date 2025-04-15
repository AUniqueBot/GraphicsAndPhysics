#version 330 core

in vec2 vUv;

uniform vec4 uColor;
uniform bool uUseTexture;
uniform sampler2D uTexture;


out vec4 outColor;



void main() {
	outColor = uColor;
	// override texture color if uUseTexture is true
	// will be handled in cpp code
	if (uUseTexture) {
		outColor = texture(uTexture, vUv);
	}
}