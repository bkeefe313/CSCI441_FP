//
// Created by Ben Keefe on 12/5/22.
//

#ifndef FP_PERLINTERRAIN_H
#define FP_PERLINTERRAIN_H

#include <CSCI441/ShaderProgram.hpp>
#include <random>

#define SCREEN_SIZE 1000.0f

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

    GLfloat _scalingFactor = 40.0f;

    CSCI441::ShaderProgram* _noiseShader;
    CSCI441::ShaderProgram* _terrainShader;

    GLuint _textures[6];


    enum Textures {
        SNOW = 0,
        ROCKY = 1,
        ARID = 2,
        GRASS = 3,
        SAND = 4,
        WATER = 5
    };


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

    void drawTerrain(glm::mat4 viewMtx, glm::mat4 projMtx, glm::vec3 flPos, glm::vec3 flDir, glm::vec3 camPos){
        _terrainShader->useProgram();
        glm::mat4 modelMtx = glm::scale(glm::mat4(1), glm::vec3(1, _scalingFactor, 1));
        CSCI441::setVertexAttributeLocations(_terrainShader->getAttributeLocation("vPos"));

        glm::mat4 mvpMatrix = projMtx * viewMtx * modelMtx;
        _terrainShader->setProgramUniform("modelMtx", modelMtx);
        _terrainShader->setProgramUniform("mvpMatrix", mvpMatrix);
        _terrainShader->setProgramUniform("flashlightPos", flPos);
        _terrainShader->setProgramUniform("flashlightDir", flDir);
        _terrainShader->setProgramUniform("camPos", camPos);

        glBindVertexArray(_vao);

        _terrainShader->setProgramUniform( "perlinTex", 0 );
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_2D, _noiseTex );

        glActiveTexture( GL_TEXTURE1 + SNOW  );
        glBindTexture(   GL_TEXTURE_2D, _textures[SNOW]  );

        glActiveTexture( GL_TEXTURE1 + ROCKY );
        glBindTexture(   GL_TEXTURE_2D, _textures[ROCKY] );

        glActiveTexture( GL_TEXTURE1 + ARID  );
        glBindTexture(   GL_TEXTURE_2D, _textures[ARID]  );

        glActiveTexture( GL_TEXTURE1 + GRASS );
        glBindTexture(   GL_TEXTURE_2D, _textures[GRASS] );

        glActiveTexture( GL_TEXTURE1 + SAND  );
        glBindTexture(   GL_TEXTURE_2D, _textures[SAND]  );

        glActiveTexture( GL_TEXTURE1 + WATER );
        glBindTexture(   GL_TEXTURE_2D, _textures[WATER] );

        glActiveTexture( GL_TEXTURE0 );

        glDrawElements(GL_TRIANGLES, _numTriangles*3, GL_UNSIGNED_SHORT, (void*)0);

    }

    void setupTextures() {
        _textures[SNOW] = loadAndRegisterTexture("assets/snow.jpg");
        _textures[ROCKY] = loadAndRegisterTexture("assets/soil.jpg");
        _textures[ARID] = loadAndRegisterTexture("assets/arid.png");
        _textures[GRASS] = loadAndRegisterTexture("assets/grass.jpg");
        _textures[SAND] = loadAndRegisterTexture("assets/sand.jpg");
        _textures[WATER] = loadAndRegisterTexture("assets/water.png");


        _terrainShader->setProgramUniform( "snowTex", SNOW + 1   );
        glActiveTexture( GL_TEXTURE1 + SNOW  );
        glBindTexture(   GL_TEXTURE_2D, _textures[SNOW]  );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

        _terrainShader->setProgramUniform( "rockyTex", ROCKY + 1 );
        glActiveTexture( GL_TEXTURE1 + ROCKY );
        glBindTexture(   GL_TEXTURE_2D, _textures[ROCKY] );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

        _terrainShader->setProgramUniform( "aridTex", ARID + 1   );
        glActiveTexture( GL_TEXTURE1 + ARID  );
        glBindTexture(   GL_TEXTURE_2D, _textures[ARID]  );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

        _terrainShader->setProgramUniform( "grassTex", GRASS + 1 );
        glActiveTexture( GL_TEXTURE1 + GRASS );
        glBindTexture(   GL_TEXTURE_2D, _textures[GRASS] );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

        _terrainShader->setProgramUniform( "sandTex", SAND + 1   );
        glActiveTexture( GL_TEXTURE1 + SAND  );
        glBindTexture(   GL_TEXTURE_2D, _textures[SAND]  );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

        _terrainShader->setProgramUniform( "waterTex", WATER + 1 );
        glActiveTexture( GL_TEXTURE1 + WATER );
        glBindTexture(   GL_TEXTURE_2D, _textures[WATER] );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );


    }

    void configTerrainShader(glm::vec3 sunPos, glm::vec3 sunColor, glm::vec3 flColor, GLfloat flAngle) {
        _terrainShader->setProgramUniform("pointLightPos", sunPos);
        _terrainShader->setProgramUniform("pointLightColor", sunColor);
        _terrainShader->setProgramUniform("flashlightColor", flColor);
        _terrainShader->setProgramUniform("flashlightCutoff", flAngle);
        _terrainShader->setProgramUniform("worldSize", _size);

    }

    void generateBuffers() {

        setupTextures();

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
    }

    void drawNoiseToFBO() {
        _noiseShader->setProgramUniform(_noiseShader->getUniformLocation("screenSize"), SCREEN_SIZE);
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
        _noiseShader->setProgramUniform(_noiseShader->getUniformLocation("screenSize"), SCREEN_SIZE);
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

    glm::vec4 permute(glm::vec4 x){return mod(((x*34.0f)+1.0f)*x, 289.0f);}
    glm::vec2 fade(glm::vec2 t) {return t*t*t*(t*(t*6.0f-15.0f)+10.0f);}

    /// \desc Takes some vec2 in world space (x, z) and spits out the proper height on the terrain
    float cnoise(glm::vec2 P){
        P /= _size;

        std::cout << P.x << ", " << P.y << std::endl;

        P *= (SCREEN_SIZE / 91.0f);

        glm::vec4 Pi = glm::vec4(floor(P.x), floor(P.y), floor(P.x), floor(P.y)) + glm::vec4(0.0, 0.0, 1.0, 1.0);
        glm::vec4 Pf = glm::vec4(glm::fract(P.x), glm::fract(P.y), glm::fract(P.x), glm::fract(P.y)) - glm::vec4(0.0, 0.0, 1.0, 1.0);
        Pi = glm::mod(Pi, 289.0f); // To avoid truncation effects in permutation
        glm::vec4 ix(Pi.x, Pi.z, Pi.x, Pi.z);
        glm::vec4 iy(Pi.y, Pi.y, Pi.w, Pi.w);
        glm::vec4 fx(Pf.x, Pf.z, Pf.x, Pf.z);
        glm::vec4 fy(Pf.y, Pf.y, Pf.w, Pf.w);
        glm::vec4 i = permute(permute(ix) + iy);
        glm::vec4 gx = 2.0f * glm::fract(i * 0.0243902439f) - 1.0f; // 1/41 = 0.024...
        glm::vec4 gy = abs(gx) - 0.5f;
        glm::vec4 tx = floor(gx + 0.5f);
        gx = gx - tx;
        glm::vec2 g00 = glm::vec2(gx.x,gy.x);
        glm:: vec2 g10 = glm::vec2(gx.y,gy.y);
        glm::vec2 g01 = glm::vec2(gx.z,gy.z);
        glm::vec2 g11 = glm::vec2(gx.w,gy.w);
        glm::vec4 norm = 1.79284291400159f - 0.85373472095314f *
                glm::vec4(dot(g00, g00), dot(g01, g01), dot(g10, g10), dot(g11, g11));
        g00 *= norm.x;
        g01 *= norm.y;
        g10 *= norm.z;
        g11 *= norm.w;
        float n00 = dot(g00, glm::vec2(fx.x, fy.x));
        float n10 = dot(g10, glm::vec2(fx.y, fy.y));
        float n01 = dot(g01, glm::vec2(fx.z, fy.z));
        float n11 = dot(g11, glm::vec2(fx.w, fy.w));
        glm::vec2 fade_xy = fade(glm::vec2(Pf.x, Pf.y));
        glm::vec2 n_x = mix(glm::vec2(n00, n01), glm::vec2(n10, n11), fade_xy.x);
        float n_xy = glm::mix(n_x.x, n_x.y, fade_xy.y);
        return 2.3f * n_xy * _scalingFactor;
    }


    GLuint loadAndRegisterTexture(const char* FILENAME) {
        // our handle to the GPU
        GLuint textureHandle = 0;

        // enable setting to prevent image from being upside down
        stbi_set_flip_vertically_on_load(true);

        // will hold image parameters after load
        GLint imageWidth, imageHeight, imageChannels;
        // load image from file
        GLubyte* data = stbi_load( FILENAME, &imageWidth, &imageHeight, &imageChannels, 0);

        // if data was read from file
        if( data ) {
            const GLint STORAGE_TYPE = (imageChannels == 4 ? GL_RGBA : GL_RGB);

            glGenTextures(1, &textureHandle);

            glBindTexture(GL_TEXTURE_2D, textureHandle);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            glTexImage2D(GL_TEXTURE_2D, 0, STORAGE_TYPE, imageWidth, imageHeight, 0, STORAGE_TYPE, GL_UNSIGNED_BYTE, data);


            fprintf( stdout, "[INFO]: %s texture map read in with handle %d\n", FILENAME, textureHandle);

            // release image memory from CPU - it now lives on the GPU
            stbi_image_free(data);
        } else {
            // load failed
            fprintf( stderr, "[ERROR]: Could not load texture map \"%s\"\n", FILENAME );
        }

        // return generated texture handle
        return textureHandle;
    }

};

#endif //FP_PERLINTERRAIN_H
