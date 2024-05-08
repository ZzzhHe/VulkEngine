#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 uv;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 fragPosWorld;
layout(location = 2) out vec3 fragNormalWorld;

layout(set = 0, binding = 0) uniform GlobalUbo {
	mat4 projectionViewMatrix;
	vec4 ambientLightColor;
	vec3 lightPos;
	vec3 viewerPos;
	vec4 lightColor;
} ubo;

// only one can be use in one shader
layout(push_constant) uniform Push {
	mat4 modelMatrix;
	mat4 normalTransformMatrix;
} push;

const float AMBIENT = 0.02;


void main() {
	vec4 posWorld = push.modelMatrix * vec4(position, 1.0f);
	gl_Position = ubo.projectionViewMatrix * posWorld;
	
	fragColor = color;
	fragPosWorld = posWorld.xyz;
	fragNormalWorld = normalize(mat3(push.normalTransformMatrix) * normal);
}
