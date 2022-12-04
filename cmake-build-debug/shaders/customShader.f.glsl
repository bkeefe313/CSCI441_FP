/*
 *   Fragment Shader
 *
 *   CSCI 441, Computer Graphics, Colorado School of Mines
 */

#version 410 core

// all uniform inputs
uniform sampler2D tex;

// all varying inputs
in vec2 texCoord;

// all fragment outputs
out vec4 fragColorOut;


void main() {
    //*****************************************
    //******* Final Color Calculations ********
    //*****************************************
    vec4 texColor = texture(tex, texCoord);
    fragColorOut = texColor;

}
