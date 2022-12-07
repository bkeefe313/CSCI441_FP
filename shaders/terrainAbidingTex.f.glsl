/*
 *  CSCI 444, Advanced Computer Graphics, Fall 2022
 *
 *  Project: lab09
 *  File: bezierPatch.f.glsl
 *
 *  Description:
 *      Fragment Shader that applies color as height of patch fragment
 *
 *  Author:
 *      Dr. Jeffrey Paone, Colorado School of Mines
 *
 *  Notes:
 *
 */

// we are using OpenGL 4.1 Core profile
#version 410 core

// ***** FRAGMENT SHADER UNIFORMS *****
uniform vec3 pointLightPos;
uniform vec3 pointLightColor;
uniform vec3 flashlightPos;
uniform vec3 flashlightColor;
uniform float flashlightCutoff;
uniform vec3 flashlightDir;
uniform mat4 modelMtx;
uniform vec3 camPos;
uniform sampler2D tex;

// ***** FRAGMENT SHADER INPUT *****
in vec3 normal;
in vec3 pos;
in vec2 texCoord;

// ***** FRAGMENT SHADER OUTPUT *****
out vec4 fragColorOut;

// ***** FRAGMENT SHADER HELPER FUNCTIONS *****
vec3 calcLight(vec3 lightColor, vec3 lightPos, vec3 vertPos, vec3 vertNorm, vec3 lightDir, vec3 color) {
    vec4 transformedPos = modelMtx * vec4(vertPos, 1.0);
    vec3 pos = vec3(transformedPos.x, transformedPos.y, transformedPos.z);

    vec3 lightVector = normalize(lightPos - pos);

    //compute view dir
    vec3 viewDir = normalize(camPos - pos);
    vec3 halfway = normalize(viewDir + lightVector);

    vec3 i_d =  lightColor * color * max(dot(vertNorm, lightVector), 0.0);

    // perform specular calculations
    vec3 i_s = lightColor * color * pow(max(dot(vertNorm, halfway), 0.0), 4.0);

    if(length(lightDir) != 0) {
        float theta = dot(normalize(lightDir), -lightVector);
        if (theta <= flashlightCutoff) {
            i_d = vec3(0.0, 0.0, 0.0);
            i_s = vec3(0.0, 0.0, 0.0);
        }
    }

    //perform ambient calculation
    vec3 i_a = color * vec3(0.1);

    //add together components
    if(length(lightDir) != 0)
    return (i_d + i_s + i_a) / (length(lightPos - pos)/10.0);
    else
    return (i_d + i_s + i_a);
}

// ***** FRAGMENT SHADER MAIN FUNCTION *****
void main() {
    vec4 texColor = texture(tex, texCoord);
    vec3 texColor3 = vec3(texColor.x, texColor.y, texColor.z);

    fragColorOut = vec4(calcLight(pointLightColor, pointLightPos, pos, normal, vec3(0,0,0), texColor3) +
    calcLight(flashlightColor, flashlightPos, pos, normal, flashlightDir, texColor3), texColor.w);
}
