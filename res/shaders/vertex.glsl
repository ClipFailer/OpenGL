#version 460 core

layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec3 vertColor;
layout(location = 2) in vec2 texture_coords;

out vec3 color;
out vec2 texCoords;

uniform mat4 modelMat;
uniform mat4 projectionMat;

void main() {
    color = vertColor;
    texCoords = texture_coords;
    gl_Position = projectionMat * modelMat * vec4(vertPos, 1.0f);
}