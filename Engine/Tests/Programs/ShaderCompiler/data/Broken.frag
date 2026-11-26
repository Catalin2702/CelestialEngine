#version 450

// Test input of CE_ShaderCompiler: an undeclared variable, so it must fail with a diagnostic.
layout(location = 0) out vec4 outputColor;

void main() {
	outputColor = undeclaredColor;
}
