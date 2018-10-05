#version 450 core

layout (location = 0)           in vec2 vPos;
layout (location = 1)           in vec4 vColor;

layout (location = 0)           out vec4 fColor;

layout (push_constant)    uniform Matrices {
    mat4 uProjection;
} uMatrices;

void main() {
    gl_Position = uMatrices.uProjection * vec4(vPos, 0.0, 1.0);

    fColor = vColor;
}