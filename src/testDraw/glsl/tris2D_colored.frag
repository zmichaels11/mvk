#version 450 core
layout (location = 0)   in vec4 fColor;

layout (location = 0)   out vec4 result;

void main() {
    if (fColor.a == 0.0) {
        discard;
    }

    result = fColor;
}
