#version 410 core

// uniform inputs
uniform mat4 mvpMatrix;                 // the precomputed Model-View-Projection Matrix
uniform mat4 modelMtx;

//camera position uniform
uniform vec3 camPos;

in vec2 vTexCoord;
out vec2 texCoord;

// attribute inputs
layout(location = 0) in vec3 vPos;      // the position of this specific vertex in object space

in vec3 vNormal;
out vec3 normal;
out vec3 pos;

// varying outputs

void main() {
    // transform & output the vertex in clip space
    gl_Position = mvpMatrix * vec4(vPos, 1.0);
    pos = vPos;
    normal = vNormal;
    texCoord = vTexCoord;
}