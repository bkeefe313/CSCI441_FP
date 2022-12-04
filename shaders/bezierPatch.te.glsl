/*
 *  CSCI 444, Advanced Computer Graphics, Fall 2022
 *
 *  Project: lab09
 *  File: bezierPatch.te.glsl
 *
 *  Description:
 *      TES to interpolate a Bezier Patch at a given (u,v) coordinate
 *
 *  Author:
 *      Dr. Jeffrey Paone, Colorado School of Mines
 *
 *  Notes:
 *
 */

// we are using OpenGL 4.1 Core profile
#version 410 core

// TODO #D: specify primitive information
layout( quads, equal_spacing, ccw ) in;

// ***** TESSELLATION EVALUATION SHADER UNIFORMS *****
uniform mat4 mvpMatrix;
uniform mat4 modelMtx;

// point light uniforms
uniform vec3 pointLightPos;
uniform vec3 pointLightColor;

//camera position uniform
uniform vec3 camPos;

// ***** TESSELLATION EVALUATION SHADER INPUT *****

// ***** TESSELLATION EVALUATION SHADER OUTPUT *****
// TODO #J: create varying output
out vec3 tessColorOut;


// ***** TESSELLATION EVALUATION SHADER HELPER FUNCTIONS *****
vec4 evalBezierCurve(vec4 p0, vec4 p1, vec4 p2, vec4 p3, float t) {
    return ( (pow(1.0f-t,3)*p0) + (3.0f*pow(1.0f-t, 2)*t*p1) + (3.0f*(1.0f-t)*t*t*p2) + (t*t*t*p3) );
}

vec4 getBezierTangent(vec4 p0, vec4 p1, vec4 p2, vec4 p3, float t) {
    return normalize( (-3*p0*t*t) + (6*p0*t) + (-3*p0) + (27*p1*t*t) + (-36*p1*t) + (9*p1) + (-9*p2*t*t) + (6*p2*t) + (3*p3*t*t) );
}

// ***** TESSELLATION EVALUATION SHADER MAIN FUNCTION *****
void main() {
    // TODO #E: get tessellation coordinate
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    // TODO #F: get all 16 control points
    vec4 p00 = gl_in[0].gl_Position;
    vec4 p01 = gl_in[1].gl_Position;
    vec4 p02 = gl_in[2].gl_Position;
    vec4 p03 = gl_in[3].gl_Position;
    vec4 p04 = gl_in[4].gl_Position;
    vec4 p05 = gl_in[5].gl_Position;
    vec4 p06 = gl_in[6].gl_Position;
    vec4 p07 = gl_in[7].gl_Position;
    vec4 p08 = gl_in[8].gl_Position;
    vec4 p09 = gl_in[9].gl_Position;
    vec4 p10 = gl_in[10].gl_Position;
    vec4 p11 = gl_in[11].gl_Position;
    vec4 p12 = gl_in[12].gl_Position;
    vec4 p13 = gl_in[13].gl_Position;
    vec4 p14 = gl_in[14].gl_Position;
    vec4 p15 = gl_in[15].gl_Position;

    // TODO #H: evaluate our bezier surface at point (u, v)
    vec4 bezierPoint = evalBezierCurve(evalBezierCurve(p00, p01, p02, p03, u), evalBezierCurve(p04, p05, p06, p07, u),
                                       evalBezierCurve(p08, p09, p10, p11, u), evalBezierCurve(p12, p13, p14, p15, u), v);

    // TODO #I: output bezier point
    gl_Position = mvpMatrix * bezierPoint;

    vec3 matColor = vec3(0.25, 0.75, 0.1);

    vec4 preNorm = getBezierTangent(evalBezierCurve(p00, p01, p02, p03, u), evalBezierCurve(p04, p05, p06, p07, u),
                                  evalBezierCurve(p08, p09, p10, p11, u), evalBezierCurve(p12, p13, p14, p15, u), v);
    vec3 normal = vec3(-preNorm.x, preNorm.y, -preNorm.z);

    vec4 transformedPos = bezierPoint;
    vec3 pos = vec3(transformedPos.x, transformedPos.y, transformedPos.z);

    vec3 pointLightRefl = normalize(pointLightPos - pos);

    //compute attenutations
    float pointAttenuation = length(pos - pointLightPos) / 4.0;

    //compute view dir
    vec3 viewDir = normalize(camPos - pos);
    vec3 halfway = normalize(viewDir + pointLightRefl);

    vec3 i_d =  pointLightColor * matColor * max(dot(normal, pointLightRefl), 0.0);

    // perform specular calculations
    vec3 i_s = pointLightColor * (matColor/4) * pow(max(dot(normal, halfway), 0.0), 4.0);

    //perform ambient calculation
    vec3 i_a = vec3(0.4) * matColor;

    //add together components
    vec3 i_tot = i_d + i_s + i_a;

    // TODO #K: set the varying value
    tessColorOut = i_tot;
}
