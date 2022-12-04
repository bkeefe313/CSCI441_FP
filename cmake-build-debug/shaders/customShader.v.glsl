/*
 *   Vertex Shader
 *
 *   CSCI 441, Computer Graphics, Colorado School of Mines
 */

#version 410 core

// all uniform inputs
uniform mat4 mvpMtx;

uniform float time;

// all attribute inputs
in vec3 vPosition;
in vec2 vTexCoord;
in vec3 vNormal;

// all varying outputs
out vec2 texCoord;


void main() {
    //*****************************************
    //********* Vertex Calculations  **********
    //*****************************************


    gl_Position = mvpMtx * vec4(vPosition, 1);

    texCoord = vTexCoord;

}