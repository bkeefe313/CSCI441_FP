/*
 *  CSCI 444, Advanced Computer Graphics, Fall 2021
 *
 *  Project: lab12
 *  File: billboardQuadShader.f.glsl
 *
 *  Description:
 *      Fragment Shader that applies color as diffuse texture
 *
 *  Author:
 *      Dr. Jeffrey Paone, Colorado School of Mines
 *
 *  Notes:
 *
 */

// we are using OpenGL 4.1 Core profile
#version 410 corea

// ***** FRAGMENT SHADER UNIFORMS *****
uniform sampler2D image;

// ***** FRAGMENT SHADER INPUT *****
in vec2 texCoord;


// ***** FRAGMENT SHADER OUTPUT *****
out vec4 fragColorOut;


// ***** FRAGMENT SHADER HELPER FUNCTIONS *****


// ***** FRAGMENT SHADER MAIN FUNCTION *****
void main() {
    fragColorOut = texture(image, texCoord);
}
