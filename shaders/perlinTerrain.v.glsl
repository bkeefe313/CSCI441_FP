#version 410 core

// uniform inputs
uniform mat4 mvpMatrix;                 // the precomputed Model-View-Projection Matrix
uniform mat4 modelMtx;

//camera position uniform
uniform vec3 camPos;

uniform sampler2D perlinTex;
uniform float worldSize;

in vec2 vTexCoord;

// attribute inputs
layout(location = 0) in vec3 vPos;      // the position of this specific vertex in object space

out vec3 normal;
out vec3 pos;
out vec3 color;

// varying outputs

void main() {
    vec4 vTexColor = texture(perlinTex, vec2(vPos.x / worldSize, vPos.z / worldSize));
    float height = (vTexColor.x - 0.5) * 2.0;
    // transform & output the vertex in clip space
    gl_Position = mvpMatrix * vec4(vPos.x, height, vPos.z, 1.0);

    color = vec3(vTexColor.x, vPos.x / worldSize, vPos.z / worldSize);
    pos = vPos;
    normal = vec3(0, 1 ,0);
}