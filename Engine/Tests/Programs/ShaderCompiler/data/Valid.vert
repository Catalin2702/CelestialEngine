#version 450

// Test input of CE_ShaderCompiler: compiles to every output.
layout(location = 0) in vec3 inputPosition;

layout(set = 0, binding = 1, std140) uniform CameraConstants {
	mat4 viewProjection;
} camera;

void main() {
	gl_Position = camera.viewProjection * vec4(inputPosition, 1.0);
}
