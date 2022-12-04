#include "A5Engine.hpp"

#include <CSCI441/objects.hpp>
#include <CSCI441/materials.hpp>

#include <fstream>
#include <string>
#include <iostream>
#include "stb_image.h"

//*************************************************************************************
//
// Public Interface

A5Engine::A5Engine()
         : CSCI441::OpenGLEngine(4, 1, 1280, 720, "A5") {
    _cam = new ArcballCam();

    for(auto& _key : _keys) _key = GL_FALSE;

    _mousePosition = glm::vec2(MOUSE_UNINITIALIZED, MOUSE_UNINITIALIZED );
    _leftMouseButtonState = GLFW_RELEASE;
    _leftControlState = GLFW_RELEASE;
    _numEnemies = 1;
    _numTrees = 0;
    _numCoins = 0;
}

A5Engine::~A5Engine() {
    delete _cam;
}

void A5Engine::handleKeyEvent(GLint key, GLint action) {
    if(key != GLFW_KEY_UNKNOWN)
        _keys[key] = ((action == GLFW_PRESS) || (action == GLFW_REPEAT));

    if(key == GLFW_KEY_LEFT_SHIFT) {
        _leftShiftState = action;
    }
    if(key == GLFW_KEY_LEFT_CONTROL) {
        _leftControlState = action;
        if(action == GLFW_RELEASE)
            _cam->zoom(0);
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
                _spawnEnemy(10);
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

void A5Engine::handleMouseButtonEvent(GLint button, GLint action) {
    // if the event is for the left mouse button
    if( button == GLFW_MOUSE_BUTTON_LEFT ) {
        // update the left mouse button's state
        _leftMouseButtonState = action;
    }
}

void A5Engine::handleCursorPositionEvent(glm::vec2 currMousePosition) {
    // if mouse hasn't moved in the window, prevent camera from flipping out
    if(_mousePosition.x == MOUSE_UNINITIALIZED) {
        _mousePosition = currMousePosition;
    }

    // if the left mouse button is being held down while the mouse is moving
    if(_leftMouseButtonState == GLFW_PRESS) {
        if(_leftControlState != GLFW_PRESS) {
            // rotate the camera by the distance the mouse moved
            _cam->rotate((currMousePosition.x - _mousePosition.x) * 0.01f,
                         (_mousePosition.y - currMousePosition.y) * 0.01f);
            _player->updateDirection(_cam->getPosition());
        } else {
            _cam->zoom((currMousePosition.y - _mousePosition.y) * 0.01f);
        }
    }

    // update the mouse position
    _mousePosition = currMousePosition;
}

void A5Engine::_createSkyboxBuffers() {
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

void A5Engine::_setupGLFW() {
    CSCI441::OpenGLEngine::_setupGLFW();

    // set our callbacks
    glfwSetKeyCallback(_window, lab04_engine_keyboard_callback);
    glfwSetMouseButtonCallback(_window, lab04_engine_mouse_button_callback);
    glfwSetCursorPosCallback(_window, lab04_engine_cursor_callback);
    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

void A5Engine::_setupOpenGL() {
    glEnable( GL_DEPTH_TEST );					                    // enable depth testing
    glDepthFunc( GL_LESS );							                // use less than depth test

    glEnable(GL_BLEND);									            // enable blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	            // use one minus blending equation

    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );	// clear the frame buffer to black

    glFrontFace(GL_CCW);                                            // the front faces are CCW
    glCullFace(GL_BACK);                                            // cull the back faces

    glPatchParameteri( GL_PATCH_VERTICES, BezierPatch::POINTS_PER_PATCH );
}

void A5Engine::_setupShaders() {

    _sun = new Light(glm::vec3(0, 400, 0), glm::vec3(1, 1, 1));

    _texShaderProgram = new CSCI441::ShaderProgram("./shaders/textureShader.v.glsl",
                                                      "./shaders/textureShader.f.glsl");
    _textureShaderUniformLocations.camPos                   = _texShaderProgram->getUniformLocation("camPos");
    _textureShaderUniformLocations.pointLightColor          = _texShaderProgram->getUniformLocation("pointLightColor");
    _textureShaderUniformLocations.pointLightPos            = _texShaderProgram->getUniformLocation("pointLightPos");
    _textureShaderUniformLocations.mvpMatrix                = _texShaderProgram->getUniformLocation("mvpMatrix");
    _textureShaderUniformLocations.modelMtx                 = _texShaderProgram->getUniformLocation("modelMtx");


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

    //point light
    glProgramUniform3fv(_texShaderProgram->getShaderProgramHandle(), _textureShaderUniformLocations.pointLightPos, 1, &(_sun->getPosition())[0]);
    glProgramUniform3fv(_texShaderProgram->getShaderProgramHandle(), _textureShaderUniformLocations.pointLightColor, 1, &(_sun->getColor())[0]);

    _bezierShaderProgram = new CSCI441::ShaderProgram( "shaders/bezierPatch.v.glsl",
                                                       "shaders/bezierPatch.tc.glsl",
                                                       "shaders/bezierPatch.te.glsl",
                                                       "shaders/bezierPatch.f.glsl");
    _bezierShaderProgramUniformLocations.camPos             = _bezierShaderProgram->getUniformLocation("camPos");
    _bezierShaderProgramUniformLocations.pointLightColor    = _bezierShaderProgram->getUniformLocation("pointLightColor");
    _bezierShaderProgramUniformLocations.pointLightPos      = _bezierShaderProgram->getUniformLocation("pointLightPos");
    _bezierShaderProgramUniformLocations.mvpMatrix          = _bezierShaderProgram->getUniformLocation("mvpMatrix");
    _bezierShaderProgramUniformLocations.modelMtx           = _bezierShaderProgram->getUniformLocation("modelMtx");
    _bezierShaderProgramAttributeLocations.vPos             = _bezierShaderProgram->getAttributeLocation("vPos");

    _bezierShaderProgram->setProgramUniform(_bezierShaderProgramUniformLocations.pointLightPos, _sun->getPosition());
    _bezierShaderProgram->setProgramUniform(_bezierShaderProgramUniformLocations.pointLightColor, _sun->getColor());
}

void A5Engine::_loadControlPointsFromFile(const char* FILENAME, GLuint* numBezierPoints, GLuint* numBezierSurfaces, glm::vec3* &bezierPoints, GLushort* &bezierIndices) {
    FILE *file = fopen(FILENAME, "r");

    if(!file) {
        fprintf( stderr, "[ERROR]: Could not open \"%s\"\n", FILENAME );
    } else {
        fscanf( file, "%u\n", numBezierSurfaces );

        fprintf( stdout, "[INFO]: Reading in %u surfaces\n", *numBezierSurfaces );

        bezierIndices = (GLushort*)malloc( sizeof( GLushort ) * *numBezierSurfaces * BezierPatch::POINTS_PER_PATCH );
        if(!bezierIndices) {
            fprintf( stderr, "[ERROR]: Could not allocate space for surface indices\n" );
        } else {
            for( int i = 0; i < *numBezierSurfaces; i++ ) {
                // read in the first 15 points that have a comma following
                for( int j = 0; j < BezierPatch::POINTS_PER_PATCH-1; j++) {
                    fscanf( file, "%hu,", &bezierIndices[i*BezierPatch::POINTS_PER_PATCH + j] );
                    bezierIndices[i *BezierPatch::POINTS_PER_PATCH + j]--;
                }
                // read in the 16th point that has a new line following
                fscanf( file, "%hu\n", &bezierIndices[i*BezierPatch::POINTS_PER_PATCH + BezierPatch::POINTS_PER_PATCH-1] );
                bezierIndices[i*BezierPatch::POINTS_PER_PATCH + BezierPatch::POINTS_PER_PATCH-1]--;
            }
        }

        fscanf( file, "%u\n", numBezierPoints );

        fprintf( stdout, "[INFO]: Reading in %u control points\n", *numBezierPoints );

        bezierPoints = (glm::vec3*)malloc( sizeof( glm::vec3 ) * *numBezierPoints );
        if(!bezierPoints) {
            fprintf( stderr, "[ERROR]: Could not allocate space for control points\n" );
        } else {
            for( int i = 0; i < *numBezierPoints; i++ ) {
                fscanf( file, "%f,%f,%f\n", &(bezierPoints[i].x), &(bezierPoints[i].y), &(bezierPoints[i].z));
            }
        }
    }
    for( int i = 0; i < *numBezierPoints; i++ ) {
        bezierPoints[i] = (bezierPoints[i] * WORLD_SIZE) / 5.0f;
    }
    fclose(file);
}

void A5Engine::_createBezierPatch() {
    glGenVertexArrays(1, &_vaos[VAOs::BEZIER]);

    glBindVertexArray( _vaos[VAOs::BEZIER] );

    glGenBuffers(1, &_vbos[VAOs::BEZIER]);
    glBindBuffer( GL_ARRAY_BUFFER, _vbos[VAOs::BEZIER] );
    glBufferData( GL_ARRAY_BUFFER, _bezierPatch.numControlPoints * sizeof(glm::vec3), _bezierPatch.controlPoints, GL_STATIC_DRAW );

    glEnableVertexAttribArray( _bezierShaderProgramAttributeLocations.vPos );
    glVertexAttribPointer( _bezierShaderProgramAttributeLocations.vPos, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0 );

    glGenBuffers(1, &_ibos[VAOs::BEZIER]);
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, _ibos[VAOs::BEZIER] );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, _bezierPatch.numSurfaces * BezierPatch::POINTS_PER_PATCH * sizeof(GLushort), _bezierPatch.patchIndices, GL_STATIC_DRAW );

    fprintf( stdout, "[INFO]: surface control points generated in with VAO %d\n", _vaos[VAOs::BEZIER] );
}

// _setupBuffers() /////////////////////////////////////////////////////////////
//
//      Create our VAOs & VBOs. Send vertex data to the GPU for future rendering
//
void A5Engine::_setupBuffers() {
    //create player object
    _player = new Player();
    _player->initModel(_texShaderProgram->getAttributeLocation("vPos"),
                       _texShaderProgram->getAttributeLocation("vNormal"),
                       _texShaderProgram->getAttributeLocation("vTexCoord"));

    _models[Models::ENEMY] = new CSCI441::ModelLoader("assets/suzanne.obj");
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

    _loadControlPointsFromFile("data/surface.txt", &_bezierPatch.numControlPoints, &_bezierPatch.numSurfaces,
                               _bezierPatch.controlPoints, _bezierPatch.patchIndices);
    _createBezierPatch();

}

void A5Engine::_setupTextures() {
    _textures[Textures::SKYBOX] = _loadAndRegisterTexture("assets/sky.png");
    _textures[Textures::JERMA] = _loadAndRegisterTexture("assets/jermasus.png");
    _textures[Textures::RED] = _loadAndRegisterTexture("assets/red.png");
}

void A5Engine::_setupScene() {
    _cam->setPosition(glm::vec3(0, 10, 0));
    _cam->setRadius(30);
    _cam->setTheta(0.1 );
    _cam->setPhi(0.1 );
    _cam->_target = _player;
    _cam->recomputeOrientation();

    _player->_position = glm::vec3(50, 0, 50);

    _enemies[0] = new Enemy(_models[Models::ENEMY], glm::vec3(0), glm::vec3(3));

    _populateScene(256, 20);

}

void A5Engine::_populateScene(int nTrees, int nCoins) {
    _numTrees = nTrees;
    _numCoins = nCoins;
    for(int i = 0; i < nTrees; i++) {
        glm::vec3 randPos = glm::vec3((rand() % (int)WORLD_SIZE*2) - (int)WORLD_SIZE, 0, (rand() % (int)WORLD_SIZE*2) - (int)WORLD_SIZE);
        randPos = glm::vec3(randPos.x, evalSurface(randPos.x, randPos.z).y, randPos.z);
        _trees[i] = new StaticObject(_models[Models::TREE], randPos);
    }
    for(int i = 0; i < nCoins; i++) {
        glm::vec3 randPos = glm::vec3((rand() % (int)WORLD_SIZE*2) - (int)WORLD_SIZE, 0, (rand() % (int)WORLD_SIZE*2) - (int)WORLD_SIZE);
        randPos = glm::vec3(randPos.x, evalSurface(randPos.x, randPos.z).y, randPos.z);
        _coins[i] = new StaticObject(_models[Models::COIN], randPos, glm::vec3(5));
    }
}

//*************************************************************************************
//
// Engine Cleanup

void A5Engine::_cleanupShaders() {
    // LOOK HERE #4: we're cleaning up our memory again!
    fprintf( stdout, "[INFO]: ...deleting Shaders.\n" );
    glDeleteShader(_texShaderProgram->getShaderProgramHandle());
    glDeleteShader(_gouraudShaderProgram->getShaderProgramHandle());
    glDeleteShader(_bezierShaderProgram->getShaderProgramHandle());
}

void A5Engine::_cleanupBuffers() {
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

void A5Engine::_renderScene(glm::mat4 viewMtx, glm::mat4 projMtx) {
    _texShaderProgram->useProgram();

    //// DRAW ENEMIES ////
    for (int i = 0; i < _numEnemies; i++) {
        _computeAndSendTransformationMatrices(_texShaderProgram, _enemies[i]->getModelMatrix(), viewMtx, projMtx,
                                              _textureShaderUniformLocations.mvpMatrix,
                                              _textureShaderUniformLocations.modelMtx,
                                              _textureShaderUniformLocations.normMatrix);
        _enemies[i]->draw(_texShaderProgram);
    }


    /// DRAW PLAYER ///
    _computeAndSendTransformationMatrices(_texShaderProgram, _player->getModelMatrix(), viewMtx, projMtx,
                                          _textureShaderUniformLocations.mvpMatrix,
                                          _textureShaderUniformLocations.modelMtx,
                                          _textureShaderUniformLocations.normMatrix);
    _player->draw(_texShaderProgram);


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
                                          _textureShaderUniformLocations.modelMtx,
                                          _textureShaderUniformLocations.normMatrix);

    glBindTexture(GL_TEXTURE_2D, _textures[Textures::SKYBOX]);
    glBindVertexArray(_vaos[VAOs::SKY]);
    glDrawElements(GL_TRIANGLE_STRIP, _numSkyboxPoints, GL_UNSIGNED_SHORT, (void *) 0);

    /// DRAW BEZIER ///
    _bezierShaderProgram->useProgram();
    CSCI441::setVertexAttributeLocations(_bezierShaderProgramAttributeLocations.vPos);
    _bezierShaderProgram->setProgramUniform(_bezierShaderProgramUniformLocations.camPos, _cam->getPosition());
    modelMtx = glm::mat4(1);
    _computeAndSendTransformationMatrices(_bezierShaderProgram,
                                          modelMtx, viewMtx, projMtx,
                                          _bezierShaderProgramUniformLocations.mvpMatrix,
                                          _bezierShaderProgramUniformLocations.modelMtx,
                                          -1);
    glBindVertexArray(_vaos[VAOs::BEZIER]);
    glDrawElements(GL_PATCHES, _bezierPatch.numSurfaces * BezierPatch::POINTS_PER_PATCH, GL_UNSIGNED_SHORT, (void *) 0);

    /// DRAW STATIC OBJECTS ///
    _texShaderProgram->useProgram();
    for (int i = 0; i < _numTrees; i++) {
        _computeAndSendTransformationMatrices(_texShaderProgram, _trees[i]->getModelMatrix(), viewMtx, projMtx,
                                              _textureShaderUniformLocations.mvpMatrix,
                                              _textureShaderUniformLocations.modelMtx,
                                              _textureShaderUniformLocations.normMatrix);
        _trees[i]->draw(_texShaderProgram);
    }
    for (int i = 0; i < _numCoins; i++) {
        _computeAndSendTransformationMatrices(_texShaderProgram, _coins[i]->getModelMatrix(), viewMtx, projMtx,
                                              _textureShaderUniformLocations.mvpMatrix,
                                              _textureShaderUniformLocations.modelMtx,
                                              _textureShaderUniformLocations.normMatrix);
        _coins[i]->draw(_texShaderProgram);
    }


    /// DRAW SUN ///
    glBindTexture(GL_TEXTURE_2D, _textures[Textures::RED]);
    modelMtx = glm::translate(glm::mat4(1), _sun->getPosition());
    _computeAndSendTransformationMatrices(_texShaderProgram, modelMtx, viewMtx, projMtx,
                                          _textureShaderUniformLocations.mvpMatrix,
                                          _textureShaderUniformLocations.modelMtx,
                                          _textureShaderUniformLocations.normMatrix);
    CSCI441::drawSolidSphere(8, 16, 16);



}

void A5Engine::_updateScene() {
    if(_leftShiftState == GLFW_PRESS && _player->_walkSpeed > 0)
        _player->_walkSpeed = 0.6f;

    _cam->recomputeOrientation();
    _cam->updateZoom();
    _player->updatePosition();

    if(glfwGetTime() - _lastSpawnTime > 5) {
        _lastSpawnTime = glfwGetTime();
        _spawnEnemy(25);
    }

    //make player fall of world edge
    if(_player->_position.x > WORLD_SIZE  || _player->_position.z > WORLD_SIZE ||
       _player->_position.x < -WORLD_SIZE || _player->_position.z < -WORLD_SIZE  ) {
        _player->fallOffEdge();
    }
    if(!_player->_falling)
        _player->_position = glm::vec3(_player->_position.x, evalSurface(_player->_position.x, _player->_position.z).y, _player->_position.z);

    for(int i = 0; i < _numEnemies; i++) {
        _enemies[i]->_speed += _lastSpawnTime / 1000000.0f;
        _enemies[i]->calculateTrajectory(_player->_position);
        _enemies[i]->move();
        _enemies[i]->_position = glm::vec3(_enemies[i]->_position.x, evalSurface(_enemies[i]->_position.x, _enemies[i]->_position.z).y, _enemies[i]->_position.z);
    }

    for(int i = 0; i < _numCoins; i++) {
        _coins[i]->updateRotation(0.01f);
    }


    _checkCollisions();


    if(_player->_position.y < -500) {
        _gameOver = true;
        _gameOverMessage = "YOU DIED";
    }
    if(_numCoins <= 0) {
        _gameOver = true;
        _gameOverMessage = "YOU WON!";
    }

}

void A5Engine::run() {
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
        glm::mat4 projectionMatrix = glm::perspective( 45.0f, (GLfloat) framebufferWidth / (GLfloat) framebufferHeight, 0.001f, 10000.0f );

        // set up our look at matrix to position our camera
        glm::mat4 viewMatrix = _cam->getViewMatrix();

        // draw everything to the window
        _renderScene(viewMatrix, projectionMatrix);

        _updateScene();

        glfwSwapBuffers(_window);                       // flush the OpenGL commands and make sure they get rendered!
        glfwPollEvents();				                // check for any events and signal to redraw screen
    }
}

void A5Engine::_checkCollisions() {
    for(int i = 0; i < _numEnemies; i++) {
        if(length(_enemies[i]->_position - _player->_position) < _enemies[i]->_scale.x/2.0f) {
            _gameOver = true;
        }
    }

    for(int i = 0; i < _numTrees; i++) {
        if(length(_trees[i]->_position - _player->_position) < 12) {
            glm::vec3 opposite = _player->_position - _trees[i]->_position;
            _player->_position += glm::normalize(glm::vec3(opposite.x, 0, opposite.z));
        }
    }

    for(int i = 0; i < _numCoins; i++) {
        if(length(_coins[i]->_position - _player->_position) < 5) {
            _numCoins--;
            if(i < _numCoins) {
                std::cout << "YOU FOUND A COIN! THERE ARE " << _numCoins << " COINS LEFT!" << std::endl;
                _coins[i] = _coins[_numCoins];
                _coins[_numCoins] = nullptr;
            }
        }
    }

    for(int i = 0; i < _numEnemies; i++) {
        for(int j = 0; j < _numEnemies; j++) {
            if (j == i) continue;
            if(length(_enemies[i]->_position - _enemies[j]->_position) < length(_enemies[i]->_scale + _enemies[j]->_scale)/2) {
                if(length(_enemies[i]->_scale) > length(_enemies[j]->_scale)) {
                    _enemies[i]->handleCollision(_enemies[j]);
                    _numEnemies--;
                    if(j < _numEnemies){
                        _enemies[j] = _enemies[_numEnemies];
                        _enemies[_numEnemies] = nullptr;
                    }
                } else {
                    _enemies[j]->handleCollision(_enemies[i]);
                    _numEnemies--;
                    if(i < _numEnemies){
                        _enemies[i] = _enemies[_numEnemies];
                        _enemies[_numEnemies] = nullptr;
                    }
                }

            }
        }
        for(int j = 0; j < _numTrees; j++) {
            if(abs(length(_enemies[i]->_position - _trees[j]->_position)) < 12) {
                _numEnemies--;
                if(i < _numEnemies){
                    _enemies[i] = _enemies[_numEnemies];
                    _enemies[_numEnemies] = nullptr;
                    break;
                }
            }
        }
    }
}

//*************************************************************************************
//
// Private Helper FUnctions

//// HELPERS

void A5Engine::_computeAndSendMatrixUniforms(CSCI441::ShaderProgram* shader, glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const {
    // precompute the Model-View-Projection matrix on the CPU
    glm::mat4 mvpMtx = projMtx * viewMtx * modelMtx;
    // then send it to the shader on the GPU to apply to every vertex
    shader->setProgramUniform("mvpMatrix", mvpMtx);
}

void A5Engine::_computeAndSendTransformationMatrices(CSCI441::ShaderProgram* shaderProgram,
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

GLuint A5Engine::_loadAndRegisterTexture(const char* FILENAME) {
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

void A5Engine::_spawnEnemy(int n) {
    for(int i = 0; i < n; i++) {
        _numEnemies++;
        if(_numEnemies > NUM_MAX_ENEMIES) {
            _numEnemies = NUM_MAX_ENEMIES;
            return;
        }
        glm::vec3 randPos((rand() % (int)WORLD_SIZE*2) - (int)WORLD_SIZE, 0, (rand() % (int)WORLD_SIZE*2) - (int)WORLD_SIZE);
        std::cout << "Spawned enemy at: ( " << randPos.x << ", " << randPos.z << " ). " << "There are now " << _numEnemies << " enemies." << std::endl;
        _enemies[_numEnemies - 1] = new Enemy(_models[Models::ENEMY], randPos, glm::vec3(3));
    }
}

void A5Engine::_deleteEnemy() {
    _numEnemies--;
    if(_numEnemies < 0) {
        _numEnemies = 0;
        return;
    }

    std::cout << "Deleted enemy. There are now " << _numEnemies << " enemies." << std::endl;

    delete(_enemies[_numEnemies]);
}

glm::vec3 A5Engine::evalBezierCurve(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, GLfloat t) {
    return ( (powf(1.0f-t,3)*p0) + (3.0f*powf(1.0f-t, 2)*t*p1) + (3.0f*(1.0f-t)*t*t*p2) + (t*t*t*p3) );
}

glm::vec3 A5Engine::evalSurface(GLfloat x, GLfloat y) {
    GLfloat u = (x + WORLD_SIZE)/(WORLD_SIZE*2);
    GLfloat v = (y + WORLD_SIZE)/(WORLD_SIZE*2);
    return evalBezierCurve(evalBezierCurve(_bezierPatch.controlPoints[0], _bezierPatch.controlPoints[1], _bezierPatch.controlPoints[2], _bezierPatch.controlPoints[3], u),
                           evalBezierCurve(_bezierPatch.controlPoints[4], _bezierPatch.controlPoints[5], _bezierPatch.controlPoints[6], _bezierPatch.controlPoints[7], u),
                           evalBezierCurve(_bezierPatch.controlPoints[8], _bezierPatch.controlPoints[9], _bezierPatch.controlPoints[10], _bezierPatch.controlPoints[11], u),
                           evalBezierCurve(_bezierPatch.controlPoints[12], _bezierPatch.controlPoints[13], _bezierPatch.controlPoints[14], _bezierPatch.controlPoints[15], u), v);
}


//*************************************************************************************
//
// Callbacks

void lab04_engine_keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods ) {
    auto engine = (A5Engine*) glfwGetWindowUserPointer(window);

    // pass the key and action through to the engine
    engine->handleKeyEvent(key, action);
}

void lab04_engine_cursor_callback(GLFWwindow *window, double x, double y ) {
    auto engine = (A5Engine*) glfwGetWindowUserPointer(window);

    // pass the cursor position through to the engine
    engine->handleCursorPositionEvent(glm::vec2(x, y));
}

void lab04_engine_mouse_button_callback(GLFWwindow *window, int button, int action, int mods ) {
    auto engine = (A5Engine*) glfwGetWindowUserPointer(window);

    // pass the mouse button and action through to the engine
    engine->handleMouseButtonEvent(button, action);
}