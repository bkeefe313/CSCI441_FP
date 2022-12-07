//
// Created by Ben Keefe on 12/5/22.
//

#ifndef FP_PERLINTERRAIN_H
#define FP_PERLINTERRAIN_H

#include <CSCI441/ShaderProgram.hpp>
#include <random>

class PerlinTerrain {
public:
    int _res;
    float _size;

    std::vector<glm::vec3> _vertices;
    std::vector<GLushort> _indices;

    GLint _numTriangles;

    GLuint _vao, _vbo, _ibo;
    GLuint _quadVAO, _quadVBO, _quadIBO;
    GLuint _fbo, _noiseTex;

    GLfloat _scalingFactor = 20.0f;

    CSCI441::ShaderProgram* _noiseShader;
    CSCI441::ShaderProgram* _terrainShader;

    PerlinTerrain(int res, float size) {
        _numTriangles = 0;
        _vao = -1, _vbo = -1, _ibo = -1;
        _res = res;
        _size = size;

        _noiseShader = new CSCI441::ShaderProgram("shaders/perlinGenerator.v.glsl",
                                                  "shaders/perlinGenerator.f.glsl");
        _terrainShader = new CSCI441::ShaderProgram("shaders/perlinTerrain.v.glsl",
                                                    "shaders/perlinTerrain.f.glsl");
    }

    void drawTerrain(glm::mat4 viewMtx, glm::mat4 projMtx, glm::vec3 flPos, glm::vec3 flDir){
        _terrainShader->useProgram();
        glm::mat4 modelMtx = glm::scale(glm::mat4(1), glm::vec3(1, _scalingFactor, 1));
        CSCI441::setVertexAttributeLocations(_terrainShader->getAttributeLocation("vPos"));

        glm::mat4 mvpMatrix = projMtx * viewMtx * modelMtx;
        _terrainShader->setProgramUniform("mvpMatrix", mvpMatrix);
        _terrainShader->setProgramUniform("perlinTex", _noiseTex);
        _terrainShader->setProgramUniform("flashlightPos", flPos);
        _terrainShader->setProgramUniform("flashlightDir", flDir);

        glBindTexture(GL_TEXTURE_2D, _noiseTex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        glBindVertexArray(_vao);
        glDrawElements(GL_TRIANGLES, _numTriangles*3, GL_UNSIGNED_SHORT, (void*)0);

    }

    void configTerrainShader(glm::vec3 sunPos, glm::vec3 sunColor, glm::vec3 flColor, GLfloat flAngle) {
        _terrainShader->setProgramUniform("pointLightPos", sunPos);
        _terrainShader->setProgramUniform("pointLightColor", sunColor);
        _terrainShader->setProgramUniform("flashlightColor", flColor);
        _terrainShader->setProgramUniform("flashlightCutoff", flAngle);
        _terrainShader->setProgramUniform("worldSize", _size);
    }

    void generateBuffers() {

        for(int i = 0; i < _res; i++) {
            for(int j = 0; j < _res; j++) {

                glm::vec3 v(((float)i / (float) _res) * _size, 0, ((float)j / (float) _res) * _size);
                _vertices.push_back(v);

            }
        }

        for(int i = 0; i < _res - 1; i++) {
            for(int j = 0; j < _res - 1; j++) {

                int v0 = (i * _res) + j;
                int v1 = ((i + 1) * _res) + j;
                int v2 = (i * _res) + j + 1;
                int v3 = ((i + 1) * _res) + j + 1;

                _indices.push_back(v0);
                _indices.push_back(v1);
                _indices.push_back(v2);
                _indices.push_back(v1);
                _indices.push_back(v2);
                _indices.push_back(v3);
                _numTriangles+=2;

            }
        }

        glGenVertexArrays(1, &_vao);

        glBindVertexArray( _vao );

        glGenBuffers(1, &_vbo);
        glBindBuffer( GL_ARRAY_BUFFER, _vbo );
        glBufferData( GL_ARRAY_BUFFER, _vertices.size() * sizeof(glm::vec3), &_vertices[0], GL_STATIC_DRAW );

        glEnableVertexAttribArray( _terrainShader->getAttributeLocation("vPos") );
        glVertexAttribPointer( _terrainShader->getAttributeLocation("vPos"), 3, GL_FLOAT, GL_FALSE, 0, (void*) 0 );

        glGenBuffers(1, &_ibo);
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, _ibo );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(GLushort), &_indices[0], GL_STATIC_DRAW );

        fprintf( stdout, "GENERATED TERRAIN TO HANDLE: %d\n", _vao );
    }

    void setupFBO() {
        glGenFramebuffers(1, &_fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

        glGenTextures(1, &_noiseTex);
        glBindTexture(GL_TEXTURE_2D, _noiseTex);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1000, 1000, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, _noiseTex);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _noiseTex, 0);

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void generateNoiseTexture() {
        setupFBO();

        // set up a quad in screen space
        float vertices[18] = {
                -1.0f, -1.0f, 0.0f, //0
                -1.0f, 1.0f, 0.0f, //1
                1.0f, -1.0f, 0.0f, //2
                -1.0f, 1.0f, 0.0f, //1
                1.0f, -1.0f, 0.0f, //2
                1.0f, 1.0f, 0.0f //3
        };
        GLushort indices[6] {
            0, 1, 2, 1, 2, 3
        };

        //generate vao for quad
        glGenVertexArrays(1, &_quadVAO);

        glBindVertexArray( _quadVAO );

        glGenBuffers(1, &_quadVBO);
        glBindBuffer( GL_ARRAY_BUFFER, _quadVBO );
        glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW );

        glEnableVertexAttribArray( _noiseShader->getAttributeLocation("vPos") );
        glVertexAttribPointer( _noiseShader->getAttributeLocation("vPos"), 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, (void*) 0 );

        glGenBuffers(1, &_quadIBO);
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, _quadIBO );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW );

        _noiseShader->setProgramUniform("blurOn", false);
        drawNoiseToFBO();
        _noiseShader->setProgramUniform("blurOn", true);
        _noiseShader->setProgramUniform("prevNoise", 0);
        glBindTexture(GL_TEXTURE_2D, _noiseTex);
        glActiveTexture(GL_TEXTURE0);
        drawNoiseToFBO();
    }

    void drawNoiseToFBO() {
        _noiseShader->setProgramUniform(_noiseShader->getUniformLocation("screenSize"), 1000.0f);
        CSCI441::setVertexAttributeLocations(_noiseShader->getAttributeLocation("vPos"));

        //draw the quad using noise generation shader
        _noiseShader->useProgram();

        //FBO will write end result to a texture!
        glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

        glBindVertexArray(_quadVAO);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        //unbind!
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void drawNoiseToScreen() {
        _noiseShader->setProgramUniform(_noiseShader->getUniformLocation("screenSize"), 1000.0f);
        CSCI441::setVertexAttributeLocations(_noiseShader->getAttributeLocation("vPos"));

        //draw the quad using noise generation shader
        _noiseShader->useProgram();

        glBindVertexArray(_quadVAO);

        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    void drawNoiseToScreenWithBlur() {
        _noiseShader->setProgramUniform("blurOn", false);
        drawNoiseToFBO();
        _noiseShader->setProgramUniform("blurOn", true);
        _noiseShader->setProgramUniform("prevNoise", 0);
        glBindTexture(GL_TEXTURE_2D, _noiseTex);
        glActiveTexture(GL_TEXTURE0);
        drawNoiseToScreen();
    }


};

#endif //FP_PERLINTERRAIN_H
