/*
 *  CSCI 444, Advanced Computer Graphics, Fall 2021
 *
 *  Project: lab12
 *  File: billboardQuadShader.g.glsl
 *
 *  Description:
 *      Geometry Shader that expands a point to a billboarded quad
 *      with texture coordinates
 *
 *  Author:
 *      Dr. Jeffrey Paone, Colorado School of Mines
 *
 *  Notes:
 *
 */

// we are using OpenGL 4.1 Core profile
#version 410 core

// ***** GEOMETRY SHADER PARAMETERS *****
layout ( points ) in;

layout( triangle_strip, max_vertices = 4 ) out;

// ***** GEOMETRY SHADER UNIFORMS *****
uniform mat4 projMatrix;
uniform mat4 mvMatrix;

// ***** GEOMETRY SHADER INPUT *****

// ***** GEOMETRY SHADER OUTPUT *****
out vec2 texCoord;


// ***** GEOMETRY SHADER HELPER FUNCTIONS *****

// ***** GEOMETRY SHADER MAIN FUNCTION *****
void main() {
    vec4 pos = mvMatrix * (gl_in[0].gl_Position + vec4(0, 5, 0, 0));

    gl_Position = projMatrix * (pos + vec4(-5, -5, 0, 5));
    texCoord = vec2(0, 0);
    EmitVertex();


    gl_Position = projMatrix * (pos + vec4(-5, 5, 0, 5));
    texCoord = vec2(0, 1);
    EmitVertex();


    gl_Position = projMatrix * (pos + vec4(5, -5, 0, 5));
    texCoord = vec2(1, 0);
    EmitVertex();


    gl_Position = projMatrix * (pos + vec4(5, 5, 0, 5));
    texCoord = vec2(1, 1);
    EmitVertex();


    EndPrimitive();

}
