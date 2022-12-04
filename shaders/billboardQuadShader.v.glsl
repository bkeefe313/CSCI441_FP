/*
 *  CSCI 444, Advanced Computer Graphics, Fall 2021
 *
 *  Project: lab12
 *  File: billboardQuadShader.v.glsl
 *
 *  Description:
 *      Vertex Shader that transforms to eye space
 *
 *  Author:
 *      Dr. Jeffrey Paone, Colorado School of Mines
 *
 *  Notes:
 *
 */

// we are using OpenGL 4.1 Core profile
#version 410 core

// ***** VERTEX SHADER UNIFORMS *****
uniform mat4 mvMatrix;

// ***** VERTEX SHADER INPUT *****
in vec3 vPos;

// ***** VERTEX SHADER OUTPUT *****


// ***** VERTEX SHADER HELPER FUNCTIONS *****


// ***** VERTEX SHADER MAIN FUNCTION *****
void main() {
    // transform vertex in to eye space
    gl_Position = vec4(vPos, 1.0);
}
