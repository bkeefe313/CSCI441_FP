#ifndef LAB04_LAB04_ENGINE_HPP
#define LAB04_LAB04_ENGINE_HPP

#include <CSCI441/FreeCam.hpp>
#include <CSCI441/ModelLoader.hpp>
#include <CSCI441/OpenGLEngine.hpp>
#include "Player.h"
#include "ArcballCam.h"
#include "types/Enemy.h"
#include "types/StaticObject.h"
#include "types/Light.hpp"
#include <CSCI441/ShaderProgram.hpp>

#define NUM_MAX_ENEMIES 1024
#define NUM_MAX_STATIC_OBJECTS 1024
#define NUM_COINS 32

class FPEngine : public CSCI441::OpenGLEngine {
public:
    FPEngine();
    ~FPEngine();

    void run() final;

    /// \desc handle any key events inside the engine
    /// \param key key as represented by GLFW_KEY_ macros
    /// \param action key event action as represented by GLFW_ macros
    void handleKeyEvent(GLint key, GLint action);

    /// \desc handle any mouse button events inside the engine
    /// \param button mouse button as represented by GLFW_MOUSE_BUTTON_ macros
    /// \param action mouse event as represented by GLFW_ macros
    void handleMouseButtonEvent(GLint button, GLint action);

    /// \desc handle any cursor movement events inside the engine
    /// \param currMousePosition the current cursor position
    void handleCursorPositionEvent(glm::vec2 currMousePosition);

    /// \desc value off-screen to represent mouse has not begun interacting with window yet
    static constexpr GLfloat MOUSE_UNINITIALIZED = -9999.0f;

private:
    void _setupGLFW() final;
    void _setupOpenGL() final;
    void _setupShaders() final;
    void _setupBuffers() final;
    void _setupScene() final;
    void _setupTextures() final;

    void _cleanupBuffers() final;
    void _cleanupShaders() final;

    void _computeAndSendMatrixUniforms(CSCI441::ShaderProgram*, glm::mat4, glm::mat4, glm::mat4) const;
    void _computeAndSendTransformationMatrices(CSCI441::ShaderProgram *shaderProgram, glm::mat4 modelMatrix,
                                               glm::mat4 viewMatrix, glm::mat4 projectionMatrix, GLint mvpMtxLocation,
                                               GLint modelMtxLocation, GLint normalMtxLocation);
    GLuint _loadAndRegisterTexture(const char* FILENAME);
    void _spawnEnemy(int);
    void _deleteEnemy();
    void _populateScene(int, int);

    /// \desc draws everything to the scene from a particular point of view
    /// \param viewMtx the current view matrix for our camera
    /// \param projMtx the current projection matrix for our camera
    void _renderScene(glm::mat4 viewMtx, glm::mat4 projMtx);
    /// \desc handles moving our FreeCam as determined by keyboard input
    void _updateScene();

    /// \desc tracks the number of different keys that can be present as determined by GLFW
    static const GLuint NUM_KEYS = GLFW_KEY_LAST;
    /// \desc boolean array tracking each key state.  if true, then the key is in a pressed or held
    /// down state.  if false, then the key is in a released state and not being interacted with
    GLboolean _keys[NUM_KEYS];

    GLfloat WORLD_SIZE = 1000.0f;
    GLfloat _lastSpawnTime = 0.0f;

    /// \desc last location of the mouse in window coordinates
    glm::vec2 _mousePosition;
    /// \desc current state of the left mouse button
    GLint _leftMouseButtonState;
    GLint _leftShiftState;
    GLint _leftControlState;

    bool _gameOver = false;
    std::string _gameOverMessage;

    /// \desc the static fixed camera in our world
    ArcballCam* _cam;

    GLuint _vaos[16];
    GLuint _vbos[16];
    GLuint _ibos[16];

    GLint _numSkyboxPoints;

    GLuint _textures[8];

    void _createSkyboxBuffers();

    /// \desc stores the player data, including model, position data, etc
    Player* _player;

    Enemy* _enemies[NUM_MAX_ENEMIES];
    GLint _numEnemies;

    StaticObject* _trees[NUM_MAX_STATIC_OBJECTS];
    StaticObject* _coins[NUM_MAX_STATIC_OBJECTS];
    GLint _numTrees;
    GLint _numCoins;

    CSCI441::ModelLoader* _models[4];

    /// lights
    Light* _sun;
    Light* _flashlight;

    CSCI441::ShaderProgram* _texShaderProgram = nullptr;
    CSCI441::ShaderProgram* _gouraudShaderProgram = nullptr;
    CSCI441::ShaderProgram* _bezierShaderProgram = nullptr;

    /// \desc Bezier Patch Information
    struct BezierPatch {
        /// \desc the number of control points per patch
        static const GLuint POINTS_PER_PATCH = 16;
        /// \desc control points array
        glm::vec3* controlPoints = nullptr;
        /// \desc index array
        GLushort* patchIndices = nullptr;
        /// \desc number of control points in the surface system.
        /// \desc corresponds to the size of controlPoints array
        GLuint numControlPoints;
        /// \desc number of curves in the system
        GLuint numSurfaces;
    } _bezierPatch;

    struct BezierShaderProgramUniformLocations {
        /// \desc precomputed MVP matrix location
        GLint mvpMatrix;
        GLint camPos;
        GLint pointLightColor;
        GLint pointLightPos;
        GLint modelMtx;
        GLint flashlightPos;
        GLint flashlightColor;
        GLint flashlightDir;
        GLint flashlightCutoff;
    } _bezierShaderProgramUniformLocations;

    /// \desc stores the locations of all of our shader attributes
    struct BezierShaderProgramAttributeLocations {
        /// \desc vertex position location
        GLint vPos;
    } _bezierShaderProgramAttributeLocations;

    struct TextureShaderUniformLocations {
        GLint mvpMatrix;
        GLint pointLightPos;
        GLint pointLightColor;
        GLint normMatrix;
        GLint camPos;
        GLint modelMtx;
        GLint flashlightPos;
        GLint flashlightColor;
        GLint flashlightDir;
        GLint flashlightCutoff;
    } _textureShaderUniformLocations;

    struct GouraudShaderProgramUniformLocations {
        /// \desc precomputed MVP matrix location
        GLint mvpMatrix;
        /// \desc model matrix location
        GLint modelMatrix;
        /// \desc normal matrix location
        GLint normalMatrix;
        /// \desc camera position location
        GLint eyePos;
        /// \desc light position location - used for point/spot
        GLint lightPos;
        /// \desc light direction location - used for directional/spot
        GLint lightDir;
        /// \desc light cone angle location - used for spot
        GLint lightCutoff;
        /// \desc color of the light location
        GLint lightColor;
        /// \desc type of the light location - 0 point 1 directional 2 spot
        GLint lightType;
        /// \desc material diffuse color location
        GLint materialDiffColor;
        /// \desc material specular color location
        GLint materialSpecColor;
        /// \desc material shininess factor location
        GLint materialShininess;
        /// \desc material ambient color location
        GLint materialAmbColor;
    } _gouraudShaderProgramUniformLocations;
    /// \desc stores the locations of all of our shader attributes
    struct GouraudShaderProgramAttributeLocations {
        /// \desc vertex position location
        GLint vPos;
        /// \desc vertex normal location
        GLint vNormal;
    } _gouraudShaderProgramAttributeLocations;

    /// \desc reads text from an external file and stores in a c-string
    /// \param [in] filename file to read contents from
    /// \param [out] output uninitialized c-string to write contents to - will allocate memory internally
    static void _readTextFromFile( const char* filename, char* &output );
    /// \desc checks GPU Shader logs and prints any messages to the screen
    /// \param handle Shader or Program handle
    static void _printLog( GLuint handle );
    /// \desc creates and compiles a given shader as written in an external file
    /// \param filename file to read shader from
    /// \param shaderType type of the shader contained in the file
    /// \returns shader handle
    static GLuint _compileShader( const char* filename, GLenum shaderType );

    void _loadControlPointsFromFile(const char* FILENAME, GLuint* numBezierPoints, GLuint* numBezierSurfaces, glm::vec3* &bezierPoints, GLushort* &bezierIndices);
    void _createBezierPatch();

    enum Textures {
        SKYBOX = 0,
        JERMA = 1,
        RED = 2
    };

    enum VAOs {
        GROUND = 0,
        SKY = 1,
        BEZIER = 2
    };

    enum Models {
        TREE = 0,
        ENEMY = 1,
        COIN = 2
    };

    glm::vec3 evalBezierCurve(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, GLfloat t);

    glm::vec3 evalSurface(GLfloat u, GLfloat v);

    void _checkCollisions();

};

void lab04_engine_keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods );
void lab04_engine_cursor_callback(GLFWwindow *window, double x, double y);
void lab04_engine_mouse_button_callback(GLFWwindow *window, int button, int action, int mods);

#endif // LAB04_LAB04_ENGINE_HPP
