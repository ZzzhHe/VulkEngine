#version 450

layout (location = 0) out vec4 outColor;

layout(location = 0) in vec3 FragPos;

void main() {
	outColor = vec4(FragPos, 1.0);
}
