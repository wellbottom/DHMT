#ifndef CLASSROOM_OBJECTS_H
#define CLASSROOM_OBJECTS_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "shader.h"

// Panel Style Structure
struct PanelStyle {
    float width;
    float height;
    float thickness;
    float frameThickness;
    glm::vec3 frameAmbient;
    glm::vec3 frameDiffuse;
    glm::vec3 frameSpecular;
    glm::vec3 surfaceAmbient;
    glm::vec3 surfaceDiffuse;
    glm::vec3 surfaceSpecular;
    bool hasTray = false;
    float trayHeight = 0.1f;
    float trayDepth = 0.3f;
    float trayOffset = 0.15f;
};

// Classroom object rendering class
class ClassroomObjects {
public:
    // Render the main classroom structure (walls, floor, ceiling, windows)
    static void renderClassroomStructure(
        GLuint cubeVAO,
        GLuint planeVAO,
        GLuint windowVAO,
        Shader& shader,
        glm::mat4& view,
        glm::mat4& projection
    );

    // Render a single desk
    static void renderDesk(
        GLuint cubeVAO,
        GLuint planeVAO,
        Shader& shader,
        glm::mat4& view,
        glm::mat4& projection,
        glm::vec3 position
    );

    // Render a bench
    static void renderBench(
        GLuint cubeVAO,
        Shader& shader,
        glm::mat4& view,
        glm::mat4& projection,
        glm::vec3 position,
        float benchWidth,
        float benchDepth,
        float benchHeight,
        float legWidth,
        float legHeight
    );

    // Render ceiling fan
    static void renderCeilingFan(
        GLuint cubeVAO,
        GLuint cylinderVAO,
        Shader& shader,
        glm::mat4& view,
        glm::mat4& projection,
        float currentTime,
        bool fanOn
    );

    // Render ceiling lights
    static void renderCeilingLights(
        GLuint cubeVAO,
        Shader& lightCubeShader,
        glm::mat4& view,
        glm::mat4& projection,
        const glm::vec3 lightPositions[4],
        bool lightsOn
    );

    // Render framed panel (blackboard, screen, door)
    static void renderFramedPanel(
        GLuint cubeVAO,
        Shader& shader,
        const glm::mat4& view,
        const glm::mat4& projection,
        const glm::vec3& position,
        const PanelStyle& style,
        float rotationDegrees = 0.0f,
        bool canRotate = false,
        bool isOpen = false
    );

    // Render projector
    static void renderProjector(
        GLuint cubeVAO,
        GLuint cylinderVAO,
        Shader& shader,
        glm::mat4& view,
        glm::mat4& projection,
        glm::vec3 position
    );

    // Render poster (textured plane on wall)
    static void renderPoster(
        GLuint planeVAO,
        Shader& shader,
        const glm::mat4& view,
        const glm::mat4& projection,
        const glm::vec3& position,
        const glm::vec3& scale,
        GLuint textureID,
        float rotationDegrees = 0.0f,
        const glm::vec3& rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f)
    );
};

#endif