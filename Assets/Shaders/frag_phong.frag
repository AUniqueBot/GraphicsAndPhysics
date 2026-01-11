in vec3 frag_position;
in vec3 frag_normal;
in vec2 frag_uv;


uniform vec3 u_viewPosition;
uniform sampler2D u_albedo;
uniform sampler2D u_gloss;
uniform float u_deltaTime;
out vec4 out_color;

struct LightData {
	vec4 m_position_type;	// 1, 2, 3, 4
	vec4 m_direction_roll;
	vec4 m_color_power;
	vec4 m_attenuation;
};

layout (std140, binding=2) uniform LightBlock {
	int m_count;
	// int _pad[3];                // pad to 16 bytes so next comes at offset 16
	LightData m_lightData[50];
};


uniform float time;

vec3 Diffuse(vec3, vec3, vec3, vec3);
vec3 Ambient(float, vec3);
vec3 Specular(vec3, vec3, float, vec3, vec3, vec3);

void main() {
	// ambient color function -> ambient = k * 
	vec3 ambient = Ambient(0.2, lightCol);
	vec3 diffuse = Diffuse(oNml, fragPos, lightPos, lightCol);
	vec3 specular = Specular(oNml, fragPos, (sin(time) + 1) / 2, viewPos, lightPos, lightCol);
	// specular = vec3(0, 0, 0);
	// result = ambient + diffuse * objectColor + some specular;
	vec4 surfaceCol = vec4(0.9, 0.3, 0.7, 1.0);
	
	vec3 result = (diffuse + ambient + specular) * vec3(surfaceCol);
	out_color = vec4(result, surfaceCol.z);
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