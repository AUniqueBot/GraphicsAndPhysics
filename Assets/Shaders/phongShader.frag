#version 460 core
in vec4 vtxPos;


in vec3 fragPos;
in vec2 oUv;
in vec3 oNml;


out vec4 outCol;


uniform vec4 color;
uniform bool useTex1;
uniform sampler2D tex1;
uniform bool useTex2;
uniform sampler2D tex2;

uniform vec3 lightCol;
uniform vec3 lightPos;


uniform vec3 viewPos;

vec3 Diffuse(vec3, vec3, vec3, vec3);
vec3 Ambient(float, vec3);
vec3 Specular(vec3, vec3, float, vec3, vec3, vec3);

void main() {
	
	// ambient color function -> ambient = k * 
	vec3 ambient = Ambient(0.2, lightCol);
	vec3 diffuse = Diffuse(oNml, fragPos, lightPos, lightCol);
	vec3 specular = Specular(oNml, fragPos, 1, viewPos, lightPos, lightCol);
	// specular = vec3(0, 0, 0);
	// result = ambient + diffuse * objectColor + some specular;
	vec4 surfaceCol = vec4(0.9, 0.3, 0.7, 1.0);

	vec3 result = (diffuse + ambient + specular) * vec3(surfaceCol);
	outCol = vec4(result, surfaceCol.z);
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

	/*
		    vec3 normal = normalize(_normal);
    vec3 lightDir = normalize(_lightPos - _fragPos);
    vec3 viewDir = normalize(_viewPos - _fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    return _strength * spec * _lightCol;


	*/
}