#ifndef LAB04_LAB04_ENGINE_HPP
#define LAB04_LAB04_ENGINE_HPP

#include <CSCI441/FreeCam.hpp>
#include <CSCI441/ModelLoader.hpp>
#include <CSCI441/OpenGLEngine.hpp>
#include "types/Player.h"
#include "include/CSCI441/ArcballCam.hpp"
#include "types/Enemy.h"
#include "types/StaticObject.h"
#include "types/Light.hpp"
#include "types/PerlinTerrain.h"
#include <CSCI441/ShaderProgram.hpp>

#define NUM_MAX_ENEMIES 1024
#define NUM_MAX_STATIC_OBJECTS 1024

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

    /// \desc handle any scroll events inside the engine
    /// \param offset the current scroll offset
    void handleScrollEvent(glm::vec2 offset);


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

    void _computeAndSendTransformationMatrices(CSCI441::ShaderProgram *shaderProgram, glm::mat4 modelMatrix,
                                               glm::mat4 viewMatrix, glm::mat4 projectionMatrix, GLint mvpMtxLocation,
                                               GLint modelMtxLocation, GLint normalMtxLocation);

    GLuint _loadAndRegisterTexture(const char *FILENAME);

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

    GLfloat WORLD_SIZE = 2000.0f;

    /// \desc last location of the mouse in window coordinates
    glm::vec2 _mousePosition;
    /// \desc current state of the left mouse button
    GLint _leftMouseButtonState;
    GLint _leftShiftState;
    GLint _leftControlState;

    bool _gameOver = false;
    bool _noiseOnlyMode = false;
    bool _daylightMode = false;
    bool _noDangerMode = false;
    bool _staticObjectsOn = true;
    bool _superFastMode = false;
    bool _animateNoiseMode = false;
    std::string _gameOverMessage;

    /// \desc the static fixed camera in our world
    CSCI441::ArcballCam *_cam;
    float _camOffset = 20.0f;

    static constexpr GLuint NUM_VAOS = 16;
    GLuint _vaos[NUM_VAOS];

    GLint _numSkyboxPoints;

    GLuint _textures[1];

    void _createSkyboxBuffers();

    /// \desc stores the player data, including model, position data, etc
    Player *_player;

    Enemy *_enemies[NUM_MAX_ENEMIES];
    GLint _numEnemies;

    StaticObject *_trees[NUM_MAX_STATIC_OBJECTS];
    StaticObject *_coins[NUM_MAX_STATIC_OBJECTS];
    GLint _numTrees;
    GLint _numCoins;

    glm::vec3 _pingDir;

    CSCI441::ModelLoader *_models[4];

    /// lights
    Light *_sun;
    Light *_flashlight;

    CSCI441::ShaderProgram *_texShaderProgram = nullptr;
    CSCI441::ShaderProgram *_gouraudShaderProgram = nullptr;
    CSCI441::ShaderProgram *_terrainAbidingTexShader = nullptr;

    PerlinTerrain *_terrain;

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


    enum Textures {
        SKYBOX = 0,
        SECRET = 1
    };

    enum VAOs {
        SKY = 0,
    };

    enum Models {
        TREE = 0,
        ENEMY = 1,
        COIN = 2
    };

    void _checkCollisions();

};

void keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

void cursor_callback(GLFWwindow *window, double x, double y);

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);

void scroll_callback(GLFWwindow *window, double x, double y);

#endif // LAB04_LAB04_ENGINE_HPP