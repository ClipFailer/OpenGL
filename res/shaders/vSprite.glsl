#version 460 core

layout(location = 0) in vec2 vertPos;
layout(location = 1) in vec2 texture_coords;

out vec2 texCoords;

uniform mat4 modelMat;
uniform mat4 projectionMat;

void main() {
    texCoords = texture_coords;
    gl_Position = projectionMat * modelMat * vec4(vertPos, 0.f, 1.f);
}