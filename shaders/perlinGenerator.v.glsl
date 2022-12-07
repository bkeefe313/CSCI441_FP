#version 410 core

uniform float screenSize;
uniform bool blurOn;

in vec3 vPos;

out vec2 perlinCoord;
out vec2 samplingCoord;

void main() {
    gl_Position = vec4(vPos.x, vPos.y, vPos.z, 1.0);

    perlinCoord = vec2(vPos.x, vPos.y) * (screenSize / 91.0);

    if (blurOn)
    samplingCoord = vec2(vPos.x, vPos.y);
    else
    samplingCoord = vec2(0);
}
