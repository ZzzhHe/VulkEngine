#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 uv;

layout(location = 0) out vec3 fragColor;

// only one can be use in one shader
layout(push_constant) uniform Push {
	mat4 transformMatrix; // projection * view * model
	mat4 normalTransformMatrix;
} push;

const vec3 DIRECTION_TO_LIGHT = normalize(vec3(1.0, -3.0, -1.0));
const float AMBIENT = 0.02;


void main() {
//	gl_Position = vec4(push.transform * position + push.offset, 0.0, 1.0);
	gl_Position = push.transformMatrix * vec4(position, 1.0f);
	
	vec3 normalWorldSpace = normalize(mat3(push.normalTransformMatrix) * normal);
	
	float lightIntensity = AMBIENT +  max(dot(normalWorldSpace, DIRECTION_TO_LIGHT), 0);
	
	fragColor = lightIntensity * color;
}
