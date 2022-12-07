#version 410 core

// uniform inputs
uniform mat4 mvpMatrix;// the precomputed Model-View-Projection Matrix
uniform mat4 vpMatrix;
uniform mat4 modelMtx;

//camera position uniform
uniform vec3 camPos;
uniform sampler2D perlinTex;
uniform float worldSize;
uniform float scalingFactor;
uniform vec3 wsPos;

in vec2 vTexCoord;
out vec2 texCoord;

// attribute inputs
layout(location = 0) in vec3 vPos;// the position of this specific vertex in object space

in vec3 vNormal;
out vec3 normal;
out vec3 pos;

// varying outputs

void main() {
    vec4 mPos = modelMtx * vec4(vPos, 1.0);
    vec4 vTexColor = texture(perlinTex, vec2(wsPos.x / worldSize, wsPos.z / worldSize));
    float height = ((vTexColor.x - 0.5) * 2.0) * scalingFactor;
    // transform & output the vertex in clip space
    gl_Position = vpMatrix * vec4(mPos.x, mPos.y + height, mPos.z, 1.0);
    pos = vPos;
    normal = vNormal;
    texCoord = vTexCoord;
}