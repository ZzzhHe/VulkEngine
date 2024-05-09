#version 450

layout (location = 0) in vec3 fragColor;
layout (location = 1) in vec3 fragPosWorld;
layout (location = 2) in vec3 fragNormalWorld;

layout (location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform GlobalUbo {
	mat4 projection;
	mat4 view;
	vec4 ambientLightColor;
	vec3 lightPos;
	vec3 viewerPos;
	vec4 lightColor;
} ubo;

layout(push_constant) uniform Push {
	mat4 modelMatrix;
	mat4 normalTransformMatrix;
} push;
void main() {
	
	vec3 directionToLight = ubo.lightPos - fragPosWorld;
	vec3 directionToViewer = ubo.viewerPos - fragPosWorld;
	
	vec3 normal = normalize(fragNormalWorld);
	
	float attenuation = 1.0 / dot(directionToLight, directionToLight);
	
	vec3 lightColor = ubo.lightColor.xyz * ubo.lightColor.w * attenuation;
	
	vec3 ambientLight = ubo.ambientLightColor.xyz * ubo.ambientLightColor.w;
	
	float diff = max(dot(normal, normalize(directionToLight)), 0);
	vec3 diffuseLight = lightColor * diff;
	
	vec3 halfwayDir = normalize(directionToLight + directionToViewer);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0f);
	vec3 specularLight = lightColor * spec;
	
	vec3 resultColor = (ambientLight + diffuseLight + specularLight) * fragColor;
	
	outColor = vec4(resultColor, 1.0);
}
