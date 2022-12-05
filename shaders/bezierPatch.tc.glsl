/*
 *  CSCI 444, Advanced Computer Graphics, Fall 2022
 *
 *  Project: lab09
 *  File: bezierPatch.tc.glsl
 *
 *  Description:
 *      TCS to set up control points for tessellation
 *
 *  Author:
 *      Dr. Jeffrey Paone, Colorado School of Mines
 *
 *  Notes:
 *
 */

// we are using OpenGL 4.1 Core profile
#version 410 core

// TODO #A: specify the number of control points per patch
layout(vertices=16) out;


// ***** TESSELLATION CONTROL SHADER UNIFORMS *****

// ***** TESSELLATION CONTROL SHADER INPUT *****

// ***** TESSELLATION CONTROL SHADER OUTPUT *****

// ***** TESSELLATION CONTROL SHADER HELPER FUNCTIONS *****


// ***** TESSELLATION CONTROL SHADER MAIN FUNCTION *****
void main() {
    //pass along vertex pos unmodified
    gl_out[ gl_InvocationID ].gl_Position = gl_in[ gl_InvocationID ].gl_Position;

    //specify outer and inner tessellation levels
    gl_TessLevelOuter[0] = 30.0f;
    gl_TessLevelOuter[1] = 30.0f;
    gl_TessLevelOuter[2] = 30.0f;
    gl_TessLevelOuter[3] = 30.0f;
    gl_TessLevelInner[0] = 30.0f;
    gl_TessLevelInner[1] = 30.0f;
}
