#version 460 core


struct Material {
	sampler2D diffuse;
	sampler2D specular;

};

struct Light {

	vec3 position;

	vec3 color;
	vec3 specular;

};


in vec3 fragPos;
in vec2 oUv;
in vec3 oNml;

out vec4 outCol;

uniform Material surfaceMaterial;
uniform Light light;
uniform vec3 viewPos;


vec3 Diffuse(vec3, vec3, vec3, vec3);
vec3 Ambient(float, vec3);
vec3 Specular(vec3, vec3, float, vec3, vec3, vec3);


void main() {
	vec3 lpos = light.position;
	vec3 lcol = light.color;
	vec3 lspc = light.specular;


	vec3 ambient = Ambient(0.2, lcol) * texture(surfaceMaterial.diffuse, oUv).rgb;
	vec3 diffuse = Diffuse(oNml, fragPos, lpos, lcol) * texture(surfaceMaterial.diffuse, oUv).rgb;
	
	vec3 specular = Specular(oNml, fragPos, 0.5, viewPos, lpos, lspc) * texture(surfaceMaterial.specular, oUv).rgb;
	outCol = vec4(ambient + diffuse + specular, 1.0);
}



vec3 Diffuse(vec3 _normal, vec3 _fragPos, vec3 _lightPosition, vec3 _lightCol) {
	_normal = normalize(_normal);
	vec3 lightDir = normalize(_lightPosition - _fragPos);
	return _lightCol * max(0, dot(_normal, lightDir));
}

vec3 Ambient(float _factor, vec3 _lightCol) {
	return _lightCol * _factor;
}

vec3 Specular(vec3 _normal, vec3 _fragPos, float _strength, vec3 _viewPos, vec3 _lightPos, vec3 _lightCol){
	vec3 viewDir = normalize(_viewPos - _fragPos);
	vec3 reflectDir = reflect(-normalize(_lightPos - _fragPos), normalize(_normal));
	// specular strength based on how aligned the viewing angle is to the reflection dir
	return pow(max(dot(viewDir, reflectDir), 0), 32) * _strength * _lightCol;
}
