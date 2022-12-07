#include "FPEngine.hpp"

#include <CSCI441/objects.hpp>
#include <CSCI441/materials.hpp>

#include <fstream>
#include <string>
#include <iostream>
#include "stb_image.h"

//*************************************************************************************
//
// Public Interface

FPEngine::FPEngine()
         : CSCI441::OpenGLEngine(4, 1, 1280, 720, "FP") {
    _cam = new CSCI441::ArcballCam();

    for(auto& _key : _keys) _key = GL_FALSE;

    _mousePosition = glm::vec2(MOUSE_UNINITIALIZED, MOUSE_UNINITIALIZED );
    _leftMouseButtonState = GLFW_RELEASE;
    _leftControlState = GLFW_RELEASE;
    _numEnemies = 0;
    _numTrees = 0;
    _numCoins = 0;
    _pingDir = glm::vec3(0);
}

FPEngine::~FPEngine() {
    delete _cam;
}

void FPEngine::handleKeyEvent(GLint key, GLint action) {
    if(key != GLFW_KEY_UNKNOWN)
        _keys[key] = ((action == GLFW_PRESS) || (action == GLFW_REPEAT));

    if(key == GLFW_KEY_LEFT_SHIFT) {
        _leftShiftState = action;
    }

    if(action == GLFW_PRESS) {
        switch( key ) {
            // quit!
            case GLFW_KEY_Q:
            case GLFW_KEY_ESCAPE:
                setWindowShouldClose();
                break;

            //movement
            case GLFW_KEY_W:
                _player->_walkSpeed = 0.2f;
                break;
            case GLFW_KEY_S:
                _player->_walkSpeed = -0.2f;
                break;
            case GLFW_KEY_A:
                _player->_strafeSpeed = -0.2f;
                break;
            case GLFW_KEY_D:
                _player->_strafeSpeed = 0.2f;
                break;

            case GLFW_KEY_UP:
                _spawnEnemy(1);
                break;
            case GLFW_KEY_DOWN:
                _deleteEnemy();
                break;
            case GLFW_KEY_RIGHT:
                _terrain->_scalingFactor += 5.0f;
                break;
            case GLFW_KEY_LEFT:
                _terrain->_scalingFactor -= 5.0f;
                break;

            case GLFW_KEY_P:
                _noiseOnlyMode = !_noiseOnlyMode;
                break;
            case GLFW_KEY_L:
                _daylightMode = !_daylightMode;
                break;
            case GLFW_KEY_M:
                while(_numEnemies > 0)
                    _deleteEnemy();
                _noDangerMode = !_noDangerMode;
                break;
            case GLFW_KEY_K:
                _staticObjectsOn = !_staticObjectsOn;
                break;
            case GLFW_KEY_O:
                _superFastMode = !_superFastMode;
                break;

            case GLFW_KEY_LEFT_BRACKET:
                _camOffset -= 5.0f;
                break;
            case GLFW_KEY_RIGHT_BRACKET:
                _camOffset += 5.0f;
                break;

            case GLFW_KEY_1:
                break;
            case GLFW_KEY_2:
                break;
            case GLFW_KEY_3:
                break;
            case GLFW_KEY_4:
            case GLFW_KEY_5:
            case GLFW_KEY_6:
            case GLFW_KEY_7:
                break;

            default: break; // suppress CLion warning
        }
    }

    if(action == GLFW_RELEASE) {
        switch(key) {
            case GLFW_KEY_W:
            case GLFW_KEY_S:
                _player->_walkSpeed = 0;
                break;
            case GLFW_KEY_A:
            case GLFW_KEY_D:
                _player->_strafeSpeed = 0;
                break;
        }
    }
}

void FPEngine::handleMouseButtonEvent(GLint button, GLint action) {
    // if the event is for the left mouse button
    if( button == GLFW_MOUSE_BUTTON_LEFT ) {
        // update the left mouse button's state
        _leftMouseButtonState = action;
    }
}

void FPEngine::handleCursorPositionEvent(glm::vec2 currMousePosition) {
    // if mouse hasn't moved in the window, prevent camera from flipping out
    if(fabs(_mousePosition.x - MOUSE_UNINITIALIZED) <= 0.000001f) {
        _mousePosition = currMousePosition;
    }
    // if shift is held down, update our camera radius

    // rotate the camera by the distance the mouse moved
    _cam->rotate((currMousePosition.x - _mousePosition.x) * 0.005f,
                            (currMousePosition.y - _mousePosition.y) * 0.005f);

    // update the mouse position
    _mousePosition = currMousePosition;

    int w = 0, h = 0;
    glfwGetWindowSize(_window, &w, &h);
    if(_mousePosition.x > w)
        glfwSetCursorPos(_window, 0, _mousePosition.y);
    if(_mousePosition.y > h)
        glfwSetCursorPos(_window, _mousePosition.x, 0);
    if(_mousePosition.x < 0)
        glfwSetCursorPos(_window, w, _mousePosition.y);
    if(_mousePosition.y < 0)
        glfwSetCursorPos(_window, _mousePosition.x, h);
}

void FPEngine::handleScrollEvent(glm::vec2 offset) {
    // update the camera radius in/out
    GLfloat totChgSq = offset.y;
    _cam->moveForward( totChgSq * 0.2f );
}

void FPEngine::_createSkyboxBuffers() {
    struct Vertex {
        GLfloat x, y, z, s, t;
    };

    Vertex skybox[8] = {
            {-1.0f, -1.0f, -1.0f, 0.25f, 0.33f},
            { 1.0f, -1.0f, -1.0f, 0.5f, 0.33f},
            {-1.0f, -1.0f,  1.0f, 1.0f, 0.33f},
            { 1.0f, -1.0f,  1.0f, 0.75f, 0.33f},
            {1.0f, 1.0f, 1.0f, 0.75f, 0.66f},
            { -1.0f, 1.0f, 1.0f, 1.0f, 0.66f},
            {1.0f, 1.0f,  -1.0f, 0.5f, 0.66f},
            { -1.0f, 1.0f,  -1.0f, 0.25f, 0.66f}
    };

    GLushort indices[12] = {0,1,6,3,4,2,5,0,7,6,5,4};

    _numSkyboxPoints = 12;

    glGenVertexArrays(1, &_vaos[VAOs::SKY]);
    glBindVertexArray(_vaos[VAOs::SKY]);

    GLuint vbods[2];       // 0 - VBO, 1 - IBO
    glGenBuffers(2, vbods);
    glBindBuffer(GL_ARRAY_BUFFER, vbods[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skybox), skybox, GL_STATIC_DRAW);

    glEnableVertexAttribArray(_texShaderProgram->getAttributeLocation("vPos"));
    glVertexAttribPointer(_texShaderProgram->getAttributeLocation("vPos"), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(_texShaderProgram->getAttributeLocation("vTexCoord"));
    glVertexAttribPointer(_texShaderProgram->getAttributeLocation("vTexCoord"), 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float)*3));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbods[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}


//*************************************************************************************
//
// Engine Setup

void FPEngine::_setupGLFW() {
    CSCI441::OpenGLEngine::_setupGLFW();

    glfwSetWindowAttrib(_window, GLFW_RESIZABLE, true);

    // set our callbacks
    glfwSetKeyCallback(_window, keyboard_callback);
    glfwSetMouseButtonCallback(_window, mouse_button_callback);
    glfwSetCursorPosCallback(_window, cursor_callback);
    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    glfwSetScrollCallback(_window, scroll_callback);

}

void FPEngine::_setupOpenGL() {
    glEnable( GL_DEPTH_TEST );					                    // enable depth testing
    glDepthFunc( GL_LESS );							                // use less than depth test

    glEnable(GL_BLEND);									            // enable blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	            // use one minus blending equation

    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );	// clear the frame buffer to black

    glFrontFace(GL_CCW);                                            // the front faces are CCW
    glCullFace(GL_BACK);                                            // cull the back faces
}

void FPEngine::_setupShaders() {

    _sun = new Light(glm::vec3(0, 400, 0), glm::vec3(0.0, 0.0, 0.05));
    _flashlight = new Light(glm::vec3(0,0,0), glm::normalize(glm::vec3(0,-0.5,1)), 0.5f, glm::vec3(0.5f, 0.5f, 0.5f));

    _texShaderProgram = new CSCI441::ShaderProgram("./shaders/textureShader.v.glsl",
                                                   "./shaders/textureShader.f.glsl");
    _textureShaderUniformLocations.camPos                   = _texShaderProgram->getUniformLocation("camPos");
    _textureShaderUniformLocations.pointLightColor          = _texShaderProgram->getUniformLocation("pointLightColor");
    _textureShaderUniformLocations.pointLightPos            = _texShaderProgram->getUniformLocation("pointLightPos");
    _textureShaderUniformLocations.flashlightColor          = _texShaderProgram->getUniformLocation("flashlightColor");
    _textureShaderUniformLocations.flashlightDir            = _texShaderProgram->getUniformLocation("flashlightDir");
    _textureShaderUniformLocations.flashlightCutoff         = _texShaderProgram->getUniformLocation("flashlightCutoff");
    _textureShaderUniformLocations.flashlightPos            = _texShaderProgram->getUniformLocation("flashlightPos");
    _textureShaderUniformLocations.mvpMatrix                = _texShaderProgram->getUniformLocation("mvpMatrix");
    _textureShaderUniformLocations.modelMtx                 = _texShaderProgram->getUniformLocation("modelMtx");

    _texShaderProgram->setProgramUniform(_textureShaderUniformLocations.pointLightPos, _sun->getPosition());
    _texShaderProgram->setProgramUniform(_textureShaderUniformLocations.pointLightColor, _sun->getColor());
    _texShaderProgram->setProgramUniform(_textureShaderUniformLocations.flashlightColor, _flashlight->getColor());
    _texShaderProgram->setProgramUniform(_textureShaderUniformLocations.flashlightCutoff, _flashlight->getAngle());

    _terrainAbidingTexShader = new CSCI441::ShaderProgram("./shaders/terrainAbidingTex.v.glsl",
                                                   "./shaders/textureShader.f.glsl");

    _terrainAbidingTexShader->setProgramUniform("pointLightPos", _sun->getPosition());
    _terrainAbidingTexShader->setProgramUniform("pointLightColor", _sun->getColor());
    _terrainAbidingTexShader->setProgramUniform("flashlightColor", _flashlight->getColor());
    _terrainAbidingTexShader->setProgramUniform("flashlightCutoff", _flashlight->getAngle());


    _gouraudShaderProgram = new CSCI441::ShaderProgram("./shaders/gouraudShader.v.glsl",
                                                       "./shaders/gouraudShader.f.glsl");
    _gouraudShaderProgramUniformLocations.modelMatrix       = _gouraudShaderProgram->getUniformLocation("modelMatrix");
    _gouraudShaderProgramUniformLocations.mvpMatrix         = _gouraudShaderProgram->getUniformLocation("mvpMatrix");
    _gouraudShaderProgramUniformLocations.lightColor        = _gouraudShaderProgram->getUniformLocation("lightColor");
    _gouraudShaderProgramUniformLocations.eyePos            = _gouraudShaderProgram->getUniformLocation("eyePos");
    _gouraudShaderProgramUniformLocations.lightCutoff       = _gouraudShaderProgram->getUniformLocation("lightCutoff");
    _gouraudShaderProgramUniformLocations.lightDir          = _gouraudShaderProgram->getUniformLocation("lightDir");
    _gouraudShaderProgramUniformLocations.lightPos          = _gouraudShaderProgram->getUniformLocation("lightPos");
    _gouraudShaderProgramUniformLocations.lightType         = _gouraudShaderProgram->getUniformLocation("lightType");
    _gouraudShaderProgramUniformLocations.materialAmbColor  = _gouraudShaderProgram->getUniformLocation("materialAmbColor");
    _gouraudShaderProgramUniformLocations.materialDiffColor = _gouraudShaderProgram->getUniformLocation("materialDiffColor");
    _gouraudShaderProgramUniformLocations.materialShininess = _gouraudShaderProgram->getUniformLocation("materialShininess");
    _gouraudShaderProgramUniformLocations.materialSpecColor = _gouraudShaderProgram->getUniformLocation("materialSpecColor");
    _gouraudShaderProgramUniformLocations.normalMatrix      = _gouraudShaderProgram->getUniformLocation("normalMatrix");

    _gouraudShaderProgram->setProgramUniform(_gouraudShaderProgramUniformLocations.lightPos, _sun->getPosition());
    _gouraudShaderProgram->setProgramUniform(_gouraudShaderProgramUniformLocations.lightType, 0);
    _gouraudShaderProgram->setProgramUniform(_gouraudShaderProgramUniformLocations.lightColor, _sun->getColor());
}

// _setupBuffers() /////////////////////////////////////////////////////////////
//
//      Create our VAOs & VBOs. Send vertex data to the GPU for future rendering
//
void FPEngine::_setupBuffers() {
    //create player object
    _player = new Player();
    _player->initModel(_texShaderProgram->getAttributeLocation("vPos"),
                       _texShaderProgram->getAttributeLocation("vNormal"),
                       _texShaderProgram->getAttributeLocation("vTexCoord"));

    _models[Models::ENEMY] = new CSCI441::ModelLoader("assets/WhenTheImposterIsSus.obj");
    _models[Models::ENEMY]->setAttributeLocations(_texShaderProgram->getAttributeLocation("vPos"),
                                                  _texShaderProgram->getAttributeLocation("vNormal"),
                                                  _texShaderProgram->getAttributeLocation("vTexCoord"));

    _models[Models::TREE] = new CSCI441::ModelLoader("assets/tree.obj");
    _models[Models::TREE]->setAttributeLocations(_texShaderProgram->getAttributeLocation("vPos"),
                                                 _texShaderProgram->getAttributeLocation("vNormal"),
                                                 _texShaderProgram->getAttributeLocation("vTexCoord"));

    _models[Models::COIN] = new CSCI441::ModelLoader("assets/coin.obj");
    _models[Models::COIN]->setAttributeLocations(_texShaderProgram->getAttributeLocation("vPos"),
                                                 _texShaderProgram->getAttributeLocation("vNormal"),
                                                 _texShaderProgram->getAttributeLocation("vTexCoord"));

    _createSkyboxBuffers();

    _terrain = new PerlinTerrain(100, WORLD_SIZE);
    _terrain->generateNoiseTexture();
    _terrain->generateBuffers();
    _terrain->configTerrainShader(_sun->getPosition(), _sun->getColor(), _flashlight->getColor(), _flashlight->getAngle());
}

void FPEngine::_setupTextures() {
    _textures[Textures::SKYBOX] = _loadAndRegisterTexture("assets/sky.png");
    _textures[Textures::JERMA] = _loadAndRegisterTexture("assets/jermasus.png");
    _textures[Textures::RED] = _loadAndRegisterTexture("assets/red.png");
    _textures[Textures::GRASS] = _loadAndRegisterTexture("assets/grass.jpg");
}

void FPEngine::_setupScene() {
    _cam->setLookAtPoint(_player->_position);
    _cam->setTheta(9.0f);
    _cam->setPhi(1.9f);
    _cam->setRadius(60.0f);
    _cam->recomputeOrientation();

    _player->_position = glm::vec3(WORLD_SIZE / 2, 0, WORLD_SIZE / 2);
    _flashlight->_position = _player->_position;

    _populateScene(300, 20);

    _spawnEnemy(1);

}

void FPEngine::_populateScene(int nTrees, int nCoins) {
    _numTrees = nTrees;
    _numCoins = nCoins;
    for(int i = 0; i < nTrees; i++) {
        glm::vec3 randPos = glm::vec3(rand() % (int)WORLD_SIZE, 0, rand() % (int)WORLD_SIZE);
        randPos = glm::vec3(randPos.x, 0, randPos.z);
        _trees[i] = new StaticObject(_models[Models::TREE], randPos);
    }
    for(int i = 0; i < nCoins; i++) {
        glm::vec3 randPos = glm::vec3(rand() % (int)WORLD_SIZE, 0, rand() % (int)WORLD_SIZE);
        randPos = glm::vec3(randPos.x, 0, randPos.z);
        _coins[i] = new StaticObject(_models[Models::COIN], randPos, glm::vec3(5));
    }
}

//*************************************************************************************
//
// Engine Cleanup

void FPEngine::_cleanupShaders() {
    // LOOK HERE #4: we're cleaning up our memory again!
    fprintf( stdout, "[INFO]: ...deleting Shaders.\n" );
    glDeleteShader(_texShaderProgram->getShaderProgramHandle());
    glDeleteShader(_gouraudShaderProgram->getShaderProgramHandle());
}

void FPEngine::_cleanupBuffers() {
    fprintf( stdout, "[INFO]: ...deleting VAOs....\n" );
    CSCI441::deleteObjectVAOs();

    fprintf( stdout, "[INFO]: ...deleting VBOs....\n" );
    CSCI441::deleteObjectVBOs();

    fprintf( stdout, "[INFO]: ...deleting models..\n" );
    delete _player;
}

//*************************************************************************************
//
// Rendering / Drawing Functions - this is where the magic happens!

void FPEngine::_renderScene(glm::mat4 viewMtx, glm::mat4 projMtx) {

    if(_noiseOnlyMode) {
        _terrain->drawNoiseToScreen();
        return;
    }
    if(_daylightMode) {
        _sun->_color = glm::vec3(0.75, 0.75, 0.75);
    } else {
        _sun->_color = glm::vec3(0, 0, 0);
    }
    _texShaderProgram->setProgramUniform(_textureShaderUniformLocations.pointLightColor, _sun->getColor());
    _terrainAbidingTexShader->setProgramUniform("pointLightColor", _sun->getColor());
    _terrain->_terrainShader->setProgramUniform("pointLightColor", _sun->getColor());


    glm::mat4 vpMatrix = projMtx * viewMtx;

    _terrainAbidingTexShader->useProgram();

    _terrainAbidingTexShader->setProgramUniform("vpMatrix", vpMatrix);
    _terrainAbidingTexShader->setProgramUniform("flashlightPos", _flashlight->_position);
    _terrainAbidingTexShader->setProgramUniform("flashlightDir", _flashlight->_direction);
    _terrainAbidingTexShader->setProgramUniform("camPos", _cam->getPosition());
    _terrainAbidingTexShader->setProgramUniform("tex", 0);
    _terrainAbidingTexShader->setProgramUniform("perlinTex", 1);
    _terrainAbidingTexShader->setProgramUniform("worldSize", WORLD_SIZE);
    _terrainAbidingTexShader->setProgramUniform("scalingFactor", _terrain->_scalingFactor);

    //// DRAW ENEMIES ////
    for (int i = 0; i < _numEnemies; i++) {
        _terrainAbidingTexShader->setProgramUniform("modelMtx", _enemies[i]->getModelMatrix());
        _terrainAbidingTexShader->setProgramUniform("wsPos", _enemies[i]->_position);
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, _terrain->_noiseTex);

        _enemies[i]->draw(_terrainAbidingTexShader);
    }


    /// DRAW PLAYER ///
    _terrainAbidingTexShader->setProgramUniform("modelMtx", _player->getModelMatrix());

    _terrainAbidingTexShader->setProgramUniform("wsPos", _player->_position);

    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, _terrain->_noiseTex);

    _player->draw(_terrainAbidingTexShader);


    /// DRAW SKYBOX ///
    _texShaderProgram->useProgram();
    _texShaderProgram->setProgramUniform("tex", Textures::SKYBOX);
    CSCI441::setVertexAttributeLocations(_texShaderProgram->getAttributeLocation("vPos"),
                                         _texShaderProgram->getAttributeLocation("vNormal"),
                                         _texShaderProgram->getAttributeLocation("vTexCoord"));

    glm::mat4 modelMtx = glm::translate(glm::mat4(1.0f), _player->_position);
    modelMtx = glm::scale(modelMtx, glm::vec3(WORLD_SIZE, WORLD_SIZE, WORLD_SIZE) * 2.0f);
    _computeAndSendTransformationMatrices(_texShaderProgram, modelMtx, viewMtx, projMtx,
                                          _textureShaderUniformLocations.mvpMatrix,
                                          _textureShaderUniformLocations.modelMtx, -1);

    glBindTexture(GL_TEXTURE_2D, _textures[Textures::SKYBOX]);
    glBindVertexArray(_vaos[VAOs::SKY]);
    glDrawElements(GL_TRIANGLE_STRIP, _numSkyboxPoints, GL_UNSIGNED_SHORT, (void *) 0);


    /// DRAW TERRAIN ///

    _terrain->drawTerrain(viewMtx, projMtx, _flashlight->_position, _flashlight->_direction, _cam->getPosition());


    if(_staticObjectsOn) {
        /// DRAW STATIC OBJECTS ///
        _terrainAbidingTexShader->useProgram();
        for (int i = 0; i < _numTrees; i++) {
            _terrainAbidingTexShader->setProgramUniform("wsPos", _trees[i]->_position);
            _terrainAbidingTexShader->setProgramUniform("modelMtx", _trees[i]->getModelMatrix());

            glActiveTexture(GL_TEXTURE0 + 1);
            glBindTexture(GL_TEXTURE_2D, _terrain->_noiseTex);

            _trees[i]->draw(_terrainAbidingTexShader);
        }
        for (int i = 0; i < _numCoins; i++) {
            _terrainAbidingTexShader->setProgramUniform("wsPos", _coins[i]->_position);
            _terrainAbidingTexShader->setProgramUniform("modelMtx", _coins[i]->getModelMatrix());

            _terrainAbidingTexShader->setProgramUniform("perlinTex", 1);
            glActiveTexture(GL_TEXTURE0 + 1);
            glBindTexture(GL_TEXTURE_2D, _terrain->_noiseTex);

            _coins[i]->draw(_terrainAbidingTexShader);
        }
    }


    glBindTexture(GL_TEXTURE_2D, _textures[Textures::RED]);
    glActiveTexture(GL_TEXTURE0);
    _terrainAbidingTexShader->setProgramUniform("perlinTex", 1);
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, _terrain->_noiseTex);
    modelMtx = glm::translate(glm::mat4(1), _player->_position + 8.0f*_pingDir + glm::vec3(0, 10, 0));
    _terrainAbidingTexShader->setProgramUniform("wsPos", _player->_position + 8.0f*_pingDir + glm::vec3(0, 10, 0));
    _terrainAbidingTexShader->setProgramUniform("modelMtx", modelMtx);
    CSCI441::drawSolidSphere(0.4, 16, 16);



}

void FPEngine::_updateScene() {
    if(_leftShiftState == GLFW_PRESS && _player->_walkSpeed > 0)
        _player->_walkSpeed = 0.8f;
    if(_superFastMode && _player->_walkSpeed > 0)
        _player->_walkSpeed = 2.0f;
    else if(_superFastMode && _player->_walkSpeed < 0)
        _player->_walkSpeed = -2.0f;

    _player->updatePosition();


    _cam->setLookAtPoint(_player->_position + glm::vec3(0, _camOffset, 0));



    _cam->recomputeOrientation();

    if(abs(_player->_walkSpeed) > 0 || abs(_player->_strafeSpeed) > 0)
        _player->updateDirection(_cam->getPosition());


    //make player fall of world edge
    if(_player->_position.x > WORLD_SIZE  || _player->_position.z > WORLD_SIZE ||
       _player->_position.x < 0 || _player->_position.z < 0  ) {
        _player->fallOffEdge();
    }
    if(!_player->_falling)
        _player->_position = glm::vec3(_player->_position.x, 0, _player->_position.z);

    for(int i = 0; i < _numEnemies; i++) {
        if(glm::length(_enemies[i]->_position - _player->_position) < 350.0f) {
            _enemies[i]->calculateTrajectory(_player->_position);
            _enemies[i]->move();
        }
    }

    for(int i = 0; i < _numCoins; i++) {
        _coins[i]->updateRotation(0.01f);
    }

    _flashlight->_direction = _player->_forward;
    _flashlight->_position = _player->_position + (1.5f*_player->_forward) + glm::vec3(0, 30, 0);

    _checkCollisions();


    if(_player->_position.y < -300) {
        _gameOver = true;
        _gameOverMessage = "YOU DIED";
    }
    if(_numCoins <= 0) {
        _gameOver = true;
        _gameOverMessage = "YOU WON!";
    }

}

void FPEngine::run() {
    //  This is our draw loop - all rendering is done here.  We use a loop to keep the window open
    //	until the user decides to close the window and quit the program.  Without a loop, the
    //	window will display once and then the program exits.
    while( !glfwWindowShouldClose(_window) ) {	        // check if the window was instructed to be closed

        if(_gameOver){
            std::cout << _gameOverMessage << std::endl;
            glfwSetWindowShouldClose(_window, true);
            break;
        }

        glDrawBuffer( GL_BACK );				        // work with our back frame buffer
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );	// clear the current color contents and depth buffer in the window

        // Get the size of our framebuffer.  Ideally this should be the same dimensions as our window, but
        // when using a Retina display the actual window can be larger than the requested window.  Therefore,
        // query what the actual size of the window we are rendering to is.
        GLint framebufferWidth, framebufferHeight;
        glfwGetFramebufferSize( _window, &framebufferWidth, &framebufferHeight );

        // update the viewport - tell OpenGL we want to render to the whole window
        glViewport( 0, 0, framebufferWidth, framebufferHeight );

        // set the projection matrix based on the window size
        // use a perspective projection that ranges
        // with a FOV of 45 degrees, for our current aspect ratio, and Z ranges from [0.001, 100].
        glm::mat4 projectionMatrix = glm::perspective( 45.0f, (GLfloat) framebufferWidth / (GLfloat) framebufferHeight, 0.001f, 8000.0f );

        // set up our look at matrix to position our camera
        glm::mat4 viewMatrix = _cam->getViewMatrix();

        // draw everything to the window
        _renderScene(viewMatrix, projectionMatrix);

        _updateScene();

        glfwSwapBuffers(_window);                       // flush the OpenGL commands and make sure they get rendered!
        glfwPollEvents();				                // check for any events and signal to redraw screen
    }
}

void FPEngine::_checkCollisions() {
    for(int i = 0; i < _numEnemies; i++) {
        if(length(_enemies[i]->_position - _player->_position) < _enemies[i]->_scale.x) {
            _gameOver = true;
            _gameOverMessage = "YOU DIED";
        }
        if(length(_enemies[i]->_position - _player->_position) < 300.0f &&
           abs(glm::dot(glm::normalize(_enemies[i]->_heading - _player->_forward), _player->_forward)) > 0.8f){
            _enemies[i]->_speed = 1.3f;
        } else {
            _enemies[i]->_speed = 0.5f;
        }
    }

    for(int i = 0; i < _numTrees; i++) {
        if(length(_trees[i]->_position - _player->_position) < 12) {
            glm::vec3 opposite = _player->_position - _trees[i]->_position;
            _player->_position += glm::normalize(glm::vec3(opposite.x, 0, opposite.z));
        }
    }

    float closest = 9999.9f;
    for(int i = 0; i < _numCoins; i++) {
        if(glm::length(_coins[i]->_position - _player->_position) < closest) {
            closest = glm::length(_coins[i]->_position - _player->_position);
            _pingDir = glm::normalize(glm::vec3(_coins[i]->_position - _player->_position));
        }

        if(glm::length(_coins[i]->_position - _player->_position) < 5) {
            _numCoins--;
            if(i < _numCoins) {
                std::cout << "YOU FOUND A COIN! THERE ARE " << _numCoins << " COINS LEFT!" << std::endl;
                _spawnEnemy(1);
                _coins[i] = _coins[_numCoins];
                _coins[_numCoins] = nullptr;
            }
        }
    }

    for(int i = 0; i < _numEnemies; i++) {
        for(int j = 0; j < _numTrees; j++) {
            if(abs(length(_enemies[i]->_position - _trees[j]->_position)) < 12) {
                glm::vec3 opposite = _enemies[i]->_position - _trees[i]->_position;
                _enemies[i]->_position += glm::normalize(glm::vec3(opposite.x, 0, opposite.z));
            }
        }
    }
}

//*************************************************************************************
//
// Private Helper FUnctions

//// HELPERS

void FPEngine::_computeAndSendTransformationMatrices(CSCI441::ShaderProgram* shaderProgram,
                                                     glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 projectionMatrix,
                                                     GLint mvpMtxLocation, GLint modelMtxLocation, GLint normalMtxLocation) {
    // ensure our shader program is not null
    if( shaderProgram ) {
        // precompute the MVP matrix CPU side
        glm::mat4 mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;
        // precompute the Normal matrix CPU side
        glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(modelMatrix)));

        // send the matrices to the shader
        shaderProgram->setProgramUniform(mvpMtxLocation, mvpMatrix);
        shaderProgram->setProgramUniform(modelMtxLocation, modelMatrix);
        shaderProgram->setProgramUniform(normalMtxLocation, normalMatrix);
    }
}

GLuint FPEngine::_loadAndRegisterTexture(const char* FILENAME) {
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

void FPEngine::_spawnEnemy(int n) {
    for(int i = 0; i < n; i++) {
        _numEnemies++;
        if(_numEnemies > NUM_MAX_ENEMIES) {
            _numEnemies = NUM_MAX_ENEMIES;
            return;
        }
        glm::vec3 randPos(rand() % (int)WORLD_SIZE, 0, rand() % (int)WORLD_SIZE);
        std::cout << "Spawned enemy at: ( " << randPos.x << ", " << randPos.z << " ). " << "There are now " << _numEnemies << " enemies." << std::endl;
        _enemies[_numEnemies - 1] = new Enemy(_models[Models::ENEMY], randPos, glm::vec3(10));
    }
}

void FPEngine::_deleteEnemy() {
    _numEnemies--;
    if(_numEnemies < 0) {
        _numEnemies = 0;
        return;
    }

    std::cout << "Deleted enemy. There are now " << _numEnemies << " enemies." << std::endl;

    delete(_enemies[_numEnemies]);
}


//*************************************************************************************
//
// Callbacks

void keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods ) {
    auto engine = (FPEngine*) glfwGetWindowUserPointer(window);

    // pass the key and action through to the engine
    engine->handleKeyEvent(key, action);
}

void cursor_callback(GLFWwindow *window, double x, double y ) {
    auto engine = (FPEngine*) glfwGetWindowUserPointer(window);


    // pass the cursor position through to the engine
    engine->handleCursorPositionEvent(glm::vec2(x, y));
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods ) {
    auto engine = (FPEngine*) glfwGetWindowUserPointer(window);

    // pass the mouse button and action through to the engine
    engine->handleMouseButtonEvent(button, action);
}

void scroll_callback(GLFWwindow *window, double xOffset, double yOffset) {
    auto engine = (FPEngine*) glfwGetWindowUserPointer(window);

    // ensure our engine is not null
    if(engine) {
        // pass the scroll offset through to the engine
        engine->handleScrollEvent(glm::vec2(xOffset, yOffset));
    }
}
