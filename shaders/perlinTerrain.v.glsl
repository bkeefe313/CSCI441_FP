#version 410 core

// uniform inputs
uniform mat4 mvpMatrix;                 // the precomputed Model-View-Projection Matrix
uniform mat4 modelMtx;

//camera position uniform
uniform vec3 camPos;

uniform sampler2D perlinTex;
uniform float worldSize;

// attribute inputs
layout(location = 0) in vec3 vPos;      // the position of this specific vertex in object space

out vec3 normal;
out vec3 pos;
out float elevation;
out vec2 texCoord;

float perlin(vec2 pos) {
    vec4 vTexColor = texture(perlinTex, vec2(vPos.x / worldSize, vPos.z / worldSize));
    return (vTexColor.x - 0.5) * 2.0;
}

void main() {
    float height = perlin(vec2(vPos.x, vPos.z));
    // transform & output the vertex in clip space
    gl_Position = mvpMatrix * vec4(vPos.x, height, vPos.z, 1.0);

    vec4 prePos = modelMtx*vec4(vPos.x, height, vPos.z, 1.0);

    float dx = 0.1, dz = 0.1;
    vec3 gradientApprox = vec3(dx, perlin(vec2(vPos.x + dx, vPos.z + dz)) - height, dz);


    vec3 highestColor = vec3(0.8, 0.8, 0.9);
    vec3 highColor = vec3(0.25, 0.25, 0.3);
    vec3 highMidColor = vec3(0.18, 0.07, 0.0);
    vec3 midColor = vec3(0.1, 0.5, 0.1);
    vec3 lowColor = vec3(0.25, 0.1, 0.0);
    vec3 lowestColor = vec3(0.1, 0.05, 0.4);


    elevation = height;
    texCoord = vec2((vPos.x / worldSize) * 64, (vPos.z / worldSize) * 64);
    pos = vec3(prePos.x, height, prePos.z);
    normal = normalize(vec3(-gradientApprox.x, gradientApprox.y, -gradientApprox.z));
}