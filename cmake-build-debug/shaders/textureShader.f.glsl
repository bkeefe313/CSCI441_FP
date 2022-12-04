
#version 410 core

// all uniform inputs
uniform sampler2D tex;

// all vertex inputs
in vec2 texCoord;
in vec3 color;

// all fragment outputs
out vec4 fragColorOut;

void main() {
    // read the texture
    vec4 texColor = texture(tex, texCoord);

    // write the color to the output
    fragColorOut = vec4(color, 1)*texColor;
}