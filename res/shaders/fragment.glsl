#version 460 core

in vec3 color;
in vec2 texCoords;

out vec4 fragColor;

uniform sampler2D tex;

void main() {
    fragColor = texture(tex, texCoords);
}