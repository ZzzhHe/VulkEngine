#version 450

layout(location = 0) in vec2 position;

layout(location = 0) out vec3 FragPos;

void main() {
	gl_Position = vec4(position, 0.0, 1.0);
	FragPos = gl_Position.xyz + 0.8f;
}
