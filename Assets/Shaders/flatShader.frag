#version 330 core
in vec4 vtxPos;
in vec3 aColour;
in vec2 oUv;
out vec4 FragColor;

uniform vec4 color;

uniform bool useTex1;
uniform sampler2D tex1;
uniform bool useTex2;
uniform sampler2D tex2;


void main() {
	// FragColor = mix(texture(tex1, TexCoord), texture(tex2, TexCoord), 0.2);
	FragColor = color;
	if (useTex1){
		FragColor = mix(texture(tex2, oUv), texture(tex1, vec2(1 - oUv.x, oUv.y)), 0.2) * vec4(oUv, 1, 1);
	}

}
