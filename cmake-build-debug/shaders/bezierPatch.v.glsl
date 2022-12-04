/*
 *  CSCI 444, Advanced Computer Graphics, Fall 2022
 *
 *  Project: lab09
 *  File: bezierPatch.v.glsl
 *
 *  Description:
 *      Pass Through Vertex Shader to send control points to TCS
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

// ***** VERTEX SHADER INPUT *****
in vec3 vPos;     // vertex position

// ***** VERTEX SHADER OUTPUT *****


// ***** VERTEX SHADER HELPER FUNCTIONS *****


// ***** VERTEX SHADER MAIN FUNCTION *****
void main() {
    // pass our vertex position through
    gl_Position = vec4( vPos, 1.0 );
}
