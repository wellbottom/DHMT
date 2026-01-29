#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

// Project headers
#include "mesh.h"
#include "shader.h"
#include "camera.h"
#include "texture.h"
#include "config_notexture.h"
#include "config_hastexture.h"
#include "ObjectAnimator.h"

// New modular headers
#include "SceneConfig.h"
#include "RenderUtils.h"
#include "ClassroomObjects.h"


// ============================================================================
// GLOBAL STATE
// ============================================================================

Camera camera(glm::vec3(0.0f, 5.0f, 15.0f));
float lastX = WindowConfig::SCR_WIDTH / 2.0f;
float lastY = WindowConfig::SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
bool lightsOn = false;
bool projectorOn = false;
bool fanOn = false;
bool doorOpen = false;

ObjectAnimator* sunAnimator = nullptr;

// ============================================================================
// FUNCTION PROTOTYPES
// ============================================================================

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);
void setupLighting(Shader& lightingShader, const glm::vec3 ceilingLightPositions[4], glm::vec3 sunPosition);
void setupLightingWithTexture(Shader& shader, const Camera& camera);




//A few texture:
namespace TexturePaths {
    const char* FLOOR_TEXTURE_PATH = "C:\\OpenGL\\Project1\\Images\\floor.jpg";
    const char* WALL_TEXTURE_PATH = "C:\\OpenGL\\Project1\\Images\\road.jpg";
    const char* TET_PATH = "C:\\OpenGL\\Project1\\Images\\Tet.png";
    const char* TET1_PATH = "C:\\OpenGL\\Project1\\Images\\Tet1.jpg";
}

// ============================================================================
// MAIN FUNCTION
// ============================================================================

int main()
{
    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(
        WindowConfig::SCR_WIDTH,
        WindowConfig::SCR_HEIGHT,
        "Classroom Scene",
        NULL,
        NULL
    );

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    gladLoadGL();
    glViewport(0, 0, WindowConfig::SCR_WIDTH, WindowConfig::SCR_HEIGHT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Create shaders
    Shader lightingShader(vertexShaderSource, lightingFragmentShaderSource);
    Shader lightCubeShader(vertexShaderSource, lightCubeFragmentShaderSource);

	Shader lightingShader_withTexture(
		vertexShaderSource_withTexture,
		lightingFragmentShaderSource_withTexture
	);

    // Setup VAOs and VBOs
    GLuint cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    const auto& cubeVerts = Mesh::GetVertices(Mesh::CUBE);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, cubeVerts.size() * sizeof(float), cubeVerts.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    GLuint planeVAO, planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    const auto& planeVerts = Mesh::GetVertices(Mesh::PLANE);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, planeVerts.size() * sizeof(float), planeVerts.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    GLuint sphereVAO, sphereVBO;
    glGenVertexArrays(1, &sphereVAO);
    glGenBuffers(1, &sphereVBO);
    const auto& sphereVerts = Mesh::GetVertices(Mesh::SPHERE);
    glBindVertexArray(sphereVAO);
    glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
    glBufferData(GL_ARRAY_BUFFER, sphereVerts.size() * sizeof(float), sphereVerts.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    GLuint windowVAO, windowVBO;
    glGenVertexArrays(1, &windowVAO);
    glGenBuffers(1, &windowVBO);
    const auto& windowVerts = Mesh::GetVertices(Mesh::WINDOW);
    glBindVertexArray(windowVAO);
    glBindBuffer(GL_ARRAY_BUFFER, windowVBO);
    glBufferData(GL_ARRAY_BUFFER, windowVerts.size() * sizeof(float), windowVerts.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    GLuint cylinderVAO, cylinderVBO;
    glGenVertexArrays(1, &cylinderVAO);
    glGenBuffers(1, &cylinderVBO);
    const auto& cylinderVerts = Mesh::GetVertices(Mesh::CYLINDER);
    glBindVertexArray(cylinderVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cylinderVBO);
    glBufferData(GL_ARRAY_BUFFER, cylinderVerts.size() * sizeof(float), cylinderVerts.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Sun animator setup
    sunAnimator = new ObjectAnimator(glm::vec3(30.0f, 20.0f, -50.0f));
    sunAnimator->setAnimationType(CIRCULAR);
    sunAnimator->setRadius(15.0f);
    sunAnimator->setSpeed(0.1f);
    sunAnimator->setCenter(glm::vec3(0.0f, 20.0f, -50.0f));
    sunAnimator->setAxis(glm::vec3(0.0f, 0.0f, 1.0f));

    glm::vec3 ceilingLightPositions[4] = {
        glm::vec3(-10.0f, 14.5f, -10.0f),
        glm::vec3(10.0f, 14.5f, -10.0f),
        glm::vec3(-10.0f, 14.5f, 10.0f),
        glm::vec3(10.0f, 14.5f, 10.0f)
    };

    // Main render loop
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.5f, 0.7f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::vec3 sunPosition = sunAnimator->update(currentFrame);

        glm::mat4 projection = glm::perspective(
            glm::radians(camera.Zoom),
            (float)WindowConfig::SCR_WIDTH / (float)WindowConfig::SCR_HEIGHT,
            0.1f,
            500.0f
        );
        glm::mat4 view = camera.GetViewMatrix();

        // Setup lighting
        setupLighting(lightingShader, ceilingLightPositions, sunPosition);
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        //Setup lighting for texture shader

        setupLightingWithTexture(lightingShader_withTexture, camera);

        // Render main scene structure
        ClassroomObjects::renderClassroomStructure(
            cubeVAO, planeVAO, windowVAO,
            lightingShader, view, projection
        );

        float frontZ = ClassroomConfig::DEPTH / 2.0f;

        // Blackboard
        PanelStyle blackboard{
            25.0f, 8.0f, 0.15f, 0.2f,
            {0.3f, 0.2f, 0.1f}, {0.5f, 0.3f, 0.15f}, {0.2f, 0.15f, 0.1f},
            {0.05f, 0.1f, 0.05f}, {0.1f, 0.2f, 0.1f}, {0.05f, 0.05f, 0.05f},
            true
        };
        ClassroomObjects::renderFramedPanel(
            cubeVAO, lightingShader, view, projection,
            glm::vec3(10.0f, 8.0f, frontZ - 0.2f), blackboard
        );

        // Projection screen
        PanelStyle screen{
            15.0f, 9.0f, 0.05f, 0.15f,
            {0.05f, 0.05f, 0.05f}, {0.1f, 0.1f, 0.1f}, {0.2f, 0.2f, 0.2f},
            {0.8f, 0.8f, 0.8f}, {0.95f, 0.95f, 0.95f}, {0.3f, 0.3f, 0.3f},
            false
        };
        ClassroomObjects::renderFramedPanel(
            cubeVAO, lightingShader, view, projection,
            glm::vec3(-12.0f, 8.0f, frontZ - 0.2f), screen
        );

        // Projector
        ClassroomObjects::renderProjector(
            cubeVAO, cylinderVAO, lightingShader, view, projection,
            glm::vec3(-10.0f, 12.0f, frontZ - 20.0f)
        );

        // Door
        PanelStyle door{
            DoorConfig::WIDTH, DoorConfig::HEIGHT, 0.15f, 0.15f,
            {0.2f, 0.1f, 0.05f}, {0.4f, 0.2f, 0.1f}, {0.3f, 0.15f, 0.08f},
            {0.3f, 0.2f, 0.1f}, {0.6f, 0.4f, 0.2f}, {0.4f, 0.3f, 0.2f},
            false
        };
        float doorX = -ClassroomConfig::WIDTH / 2.0f + 0.15f;
        float doorY = 4.0f;
        ClassroomObjects::renderFramedPanel(
            cubeVAO, lightingShader, view, projection,
            glm::vec3(doorX, doorY, DoorConfig::Z_POSITION),
            door, 90.0f, true, doorOpen
        );

        // Ceiling lights
        ClassroomObjects::renderCeilingLights(
            cubeVAO, lightCubeShader, view, projection,
            ceilingLightPositions, lightsOn
        );

        // Ceiling fan
        ClassroomObjects::renderCeilingFan(
            cubeVAO, cylinderVAO, lightingShader,
            view, projection, currentFrame, fanOn
        );

        // Desks and benches
        float deskStride = DeskDimensions::MAIN_WIDTH + DeskLayout::PAIR_SPACING;
        float columnWidth = DeskLayout::NUM_DESKS_PER_GROUP * deskStride;

        for (int row = 0; row < DeskLayout::NUM_ROWS; row++)
        {
            for (int col = 0; col < DeskLayout::NUM_COLS; col++)
            {
                // Bench
                ClassroomObjects::renderBench(
                    cubeVAO, lightingShader, view, projection,
                    glm::vec3(
                        DeskLayout::START_X + col * (columnWidth + DeskLayout::COL_SPACING) +
                        (columnWidth - DeskLayout::PAIR_SPACING) / 2.0f - deskStride / 2.0f,
                        0.0f,
                        DeskLayout::START_Z + row * DeskLayout::ROW_SPACING - DeskLayout::ROW_SPACING / 2.0f
                    ),
                    columnWidth,
                    BenchDimensions::DEPTH,
                    BenchDimensions::HEIGHT,
                    BenchDimensions::LEG_WIDTH,
                    BenchDimensions::HEIGHT
                );

                // Desks
                for (int i = 0; i < DeskLayout::NUM_DESKS_PER_GROUP; i++)
                {
                    float x = DeskLayout::START_X + col * (columnWidth + DeskLayout::COL_SPACING) + i * deskStride;
                    float z = DeskLayout::START_Z + row * DeskLayout::ROW_SPACING;
                    ClassroomObjects::renderDesk(
                        cubeVAO, planeVAO, lightingShader, view, projection,
                        glm::vec3(x, 0.0f, z)
                    );
                }
            }
        }

        // Sun
        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);
        lightCubeShader.setVec3("lightColor", 1.0f, 1.0f, 0.8f);

        glBindVertexArray(sphereVAO);
        glm::mat4 sunModel = glm::mat4(1.0f);
        sunModel = glm::translate(sunModel, sunPosition);
        sunModel = glm::scale(sunModel, glm::vec3(5.0f));
        lightCubeShader.setMat4("model", sunModel);
        glDrawArrays(GL_TRIANGLES, 0, Mesh::GetVertexCount(Mesh::SPHERE));



        // Posters

		GLuint tetPosterTex = Texture::LoadTexture(TexturePaths::TET_PATH);
        if (tetPosterTex == 0) {
            std::cout << "Failed to load poster texture!" << std::endl;
        }


        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tetPosterTex);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, tetPosterTex);

        ClassroomObjects::renderPoster(
            planeVAO,
            lightingShader_withTexture,
            view,
            projection,
            glm::vec3(0.0f, 9.0f, -ClassroomConfig::DEPTH / 2.0f + 0.11f),
            glm::vec3(20.0f, 1.0f, 15.0f),
            tetPosterTex,              // textureID
            90.0f,                      // rotationDegrees
            glm::vec3(1.0f, 0.0f, 0.0f) // rotationAxis
        );



        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &planeVAO);
    glDeleteVertexArrays(1, &sphereVAO);
    glDeleteVertexArrays(1, &windowVAO);
    glDeleteVertexArrays(1, &cylinderVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &planeVBO);
    glDeleteBuffers(1, &sphereVBO);
    glDeleteBuffers(1, &windowVBO);
    glDeleteBuffers(1, &cylinderVBO);
    lightingShader.deleteProgram();
    lightCubeShader.deleteProgram();

    delete sunAnimator;

    glfwTerminate();
    return 0;
}

// ============================================================================
// LIGHTING SETUP
// ============================================================================

void setupLighting(Shader& lightingShader, const glm::vec3 ceilingLightPositions[4], glm::vec3 sunPosition)
{
    lightingShader.use();
    lightingShader.setVec3("viewPos", camera.Position);
    lightingShader.setFloat("material.shininess", 32.0f);

    // Directional light (sun)
    glm::vec3 sunDirection = glm::normalize(glm::vec3(0.0f, 0.0f, 0.0f) - sunPosition);
    lightingShader.setVec3("dirLight.direction", sunDirection);
    lightingShader.setVec3("dirLight.ambient", 0.3f, 0.3f, 0.3f);
    lightingShader.setVec3("dirLight.diffuse", 0.8f, 0.8f, 0.7f);
    lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

    // Point lights (ceiling lights)
    for (int i = 0; i < 4; i++)
    {
        std::string number = std::to_string(i);
        lightingShader.setVec3("pointLights[" + number + "].position", ceilingLightPositions[i]);

        if (lightsOn)
        {
            lightingShader.setVec3("pointLights[" + number + "].ambient", 0.2f, 0.2f, 0.2f);
            lightingShader.setVec3("pointLights[" + number + "].diffuse", 0.8f, 0.8f, 0.8f);
            lightingShader.setVec3("pointLights[" + number + "].specular", 1.0f, 1.0f, 1.0f);
        }
        else
        {
            lightingShader.setVec3("pointLights[" + number + "].ambient", 0.0f, 0.0f, 0.0f);
            lightingShader.setVec3("pointLights[" + number + "].diffuse", 0.0f, 0.0f, 0.0f);
            lightingShader.setVec3("pointLights[" + number + "].specular", 0.0f, 0.0f, 0.0f);
        }

        lightingShader.setFloat("pointLights[" + number + "].constant", 1.0f);
        lightingShader.setFloat("pointLights[" + number + "].linear", 0.045f);
        lightingShader.setFloat("pointLights[" + number + "].quadratic", 0.0075f);
    }

    // Spotlight (projector)
    lightingShader.setVec3("spotLight.position", glm::vec3(-10.0f, 12.0f, 5.0f));
    lightingShader.setVec3("spotLight.direction",
        glm::normalize(glm::vec3(-12.0f, 8.0f, 24.8f) - glm::vec3(-10.0f, 12.0f, 5.0f)));

    //first frame default values
    lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
    lightingShader.setVec3("spotLight.diffuse", 0.0f, 0.0f, 0.0f);
    lightingShader.setVec3("spotLight.specular", 0.0f, 0.0f, 0.0f);
    lightingShader.setFloat("spotLight.constant", 1.0f);
    lightingShader.setFloat("spotLight.linear", 0.09f);
    lightingShader.setFloat("spotLight.quadratic", 0.032f);
    lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

    if (projectorOn)
    {
        lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        lightingShader.setVec3("spotLight.diffuse", 1.5f, 1.8f, 4.0f);
        lightingShader.setVec3("spotLight.specular", 2.0f, 2.5f, 4.5f);
        lightingShader.setFloat("spotLight.constant", 1.0f);
        lightingShader.setFloat("spotLight.linear", 0.014f);
        lightingShader.setFloat("spotLight.quadratic", 0.0007f);
        lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(10.0f)));
        lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(13.0f)));
    }
    else
    {
        lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        lightingShader.setVec3("spotLight.diffuse", 0.0f, 0.0f, 0.0f);
        lightingShader.setVec3("spotLight.specular", 0.0f, 0.0f, 0.0f);
    }
}


void setupLightingWithTexture(Shader& shader, const Camera& camera)
{
    shader.use();

    // =========================
    // View position (REQUIRED)
    // =========================
    shader.setVec3("viewPos", camera.Position);

    // =========================
    // Directional light
    // =========================
    shader.setVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));

    shader.setVec3("dirLight.ambient", glm::vec3(1.0f));   // Low ambient, neutral color
    shader.setVec3("dirLight.diffuse", glm::vec3(1.0f));   // Full diffuse, neutral color
    shader.setVec3("dirLight.specular", glm::vec3(1.0f));  // Full specular, neutral color


    // =========================
    // Point lights (NR_POINT_LIGHTS = 4)
    // =========================
    glm::vec3 pointLightPositions[4] = {
        { -6.0f, 7.5f,  6.0f },
        {  6.0f, 7.5f,  6.0f },
        { -6.0f, 7.5f, -6.0f },
        {  6.0f, 7.5f, -6.0f }
    };

    for (int i = 0; i < 4; i++)
    {
        std::string base = "pointLights[" + std::to_string(i) + "]";

        shader.setVec3(base + ".position", pointLightPositions[i]);
        shader.setVec3(base + ".ambient", glm::vec3(0.1f));
        shader.setVec3(base + ".diffuse", glm::vec3(0.8f));
        shader.setVec3(base + ".specular", glm::vec3(1.0f));

        shader.setFloat(base + ".constant", 1.0f);
        shader.setFloat(base + ".linear", 0.09f);
        shader.setFloat(base + ".quadratic", 0.032f);
    }

    // =========================
    // Spotlight (camera-attached)
    // =========================
    shader.setVec3("spotLight.position", camera.Position);
    shader.setVec3("spotLight.direction", camera.Front);

    shader.setFloat("spotLight.cutOff",
        glm::cos(glm::radians(12.5f)));
    shader.setFloat("spotLight.outerCutOff",
        glm::cos(glm::radians(17.5f)));

    shader.setVec3("spotLight.ambient", glm::vec3(0.0f));
    shader.setVec3("spotLight.diffuse", glm::vec3(1.0f));
    shader.setVec3("spotLight.specular", glm::vec3(1.0f));

    shader.setFloat("spotLight.constant", 1.0f);
    shader.setFloat("spotLight.linear", 0.09f);
    shader.setFloat("spotLight.quadratic", 0.032f);

    // =========================
    // Material (TEXTURE-BASED)
    // =========================
    shader.setInt("material.diffuse", 0); // GL_TEXTURE0
    shader.setInt("material.specular", 0); // reuse diffuse
    shader.setFloat("material.shininess", 16.0f);
}


// ============================================================================
// INPUT HANDLING
// ============================================================================

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

    // C = toggle ceiling lights
    static bool cKeyPressed = false;
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && !cKeyPressed)
    {
        cKeyPressed = true;
        lightsOn = !lightsOn;
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE)
    {
        cKeyPressed = false;
    }

    // V = toggle fan
    static bool vKeyPressed = false;
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS && !vKeyPressed)
    {
        vKeyPressed = true;
        fanOn = !fanOn;
    }
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_RELEASE)
    {
        vKeyPressed = false;
    }

    // B = toggle projector
    static bool bKeyPressed = false;
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && !bKeyPressed)
    {
        bKeyPressed = true;
        projectorOn = !projectorOn;
    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE)
    {
        bKeyPressed = false;
    }

    // Z = toggle door
    static bool zKeyPressed = false;
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS && !zKeyPressed)
    {
        zKeyPressed = true;
        doorOpen = !doorOpen;
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_RELEASE)
    {
        zKeyPressed = false;
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

