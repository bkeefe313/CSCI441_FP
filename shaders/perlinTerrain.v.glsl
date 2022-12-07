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
out vec3 color;

// varying outputs
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

    color = vec3(0.1, 0.8, 0.2);
    pos = vec3(prePos.x, prePos.y, prePos.z);
    normal = vec3(0, 1, 0); //normalize(vec3(-gradientApprox.x, gradientApprox.y, -gradientApprox.z));
}