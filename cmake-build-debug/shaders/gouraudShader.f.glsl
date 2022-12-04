#version 410 core

// uniform inputs
uniform vec3 lightColor;                // light color
uniform vec3 materialAmbColor;          // the material ambient color

// varying inputs
layout(location = 0) in vec4 color;     // interpolated color for this fragment

// outputs
out vec4 fragColorOut;                  // color to apply to this fragment

void main() {
    // if we are looking at the front face of the fragment
    if(gl_FrontFacing) {
        // thne pass the interpolated color through as output
        fragColorOut = color;
    }
    // otherwise we are lookiing at the back face of the fragment
    else {
        // apply only ambient lighting
        fragColorOut = vec4( lightColor * materialAmbColor, 1.0f );
    }
}