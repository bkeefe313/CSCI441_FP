#version 410 core

// uniform inputs
uniform mat4 mvpMatrix;                 // the precomputed Model-View-Projection Matrix
uniform mat4 modelMtx;

// point light uniforms
uniform vec3 pointLightPos;
uniform vec3 pointLightColor;

//camera position uniform
uniform vec3 camPos;

in vec2 vTexCoord;
out vec2 texCoord;

// attribute inputs
layout(location = 0) in vec3 vPos;      // the position of this specific vertex in object space

in vec3 vNormal;

// varying outputs
layout(location = 0) out vec3 color;    // color to apply to this vertex

void main() {
    vec4 transformedPos = modelMtx * vec4(vPos, 1.0);
    vec3 pos = vec3(transformedPos.x, transformedPos.y, transformedPos.z);

    // transform & output the vertex in clip space
    gl_Position = mvpMatrix * vec4(vPos, 1.0);

    vec3 pointLightRefl = normalize(pointLightPos - pos);

    //compute view dir
    vec3 viewDir = normalize(camPos - pos);
    vec3 halfway = normalize(viewDir + pointLightRefl);

    vec3 i_d =  pointLightColor * max(dot(vNormal, pointLightRefl), 0.0);

    // perform specular calculations
    vec3 i_s = pointLightColor * max(dot(vNormal, halfway), 0.0);

    //perform ambient calculation
    vec3 i_a = vec3(0.5);

    //add together components
    vec3 i_tot = i_d + i_s + i_a;

    color = i_tot;
    texCoord = vTexCoord;
}