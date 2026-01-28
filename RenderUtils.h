#ifndef RENDER_UTILS_H
#define RENDER_UTILS_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "shader.h"

// Rendering utility functions for basic shapes
class RenderUtils {
public:
    // Render cube with position, scale, and material properties
    static void renderCube(
        GLuint cubeVAO,
        Shader& shader,
        const glm::vec3& position,
        const glm::vec3& scale,
        const glm::vec3& ambient,
        const glm::vec3& diffuse,
        const glm::vec3& specular,
        float alpha = 1.0f,
        float rotationDegrees = 0.0f,
        const glm::vec3& rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f)
    );

    // Render cube with pre-built transformation matrix
    static void renderCubeWithMatrix(
        GLuint cubeVAO,
        Shader& shader,
        const glm::mat4& transformMatrix,
        const glm::vec3& scale,
        const glm::vec3& ambient,
        const glm::vec3& diffuse,
        const glm::vec3& specular,
        float alpha = 1.0f
    );

    // Render plane
    static void renderPlane(
        GLuint planeVAO,
        Shader& shader,
        const glm::vec3& position,
        const glm::vec3& scale,
        const glm::vec3& ambient,
        const glm::vec3& diffuse,
        const glm::vec3& specular,
        float alpha = 1.0f,
        float rotationDegrees = 0.0f,
        const glm::vec3& rotationAxis = glm::vec3(1.0f, 0.0f, 0.0f)
    );

    // Render cylinder
    static void renderCylinder(
        GLuint cylinderVAO,
        Shader& shader,
        const glm::vec3& position,
        const glm::vec3& scale,
        const glm::vec3& ambient,
        const glm::vec3& diffuse,
        const glm::vec3& specular,
        float alpha = 1.0f,
        float rotationDegrees = 0.0f,
        const glm::vec3& rotationAxis = glm::vec3(1.0f, 0.0f, 0.0f)
    );

    // Render cylinder with pre-built transformation matrix
    static void renderCylinderWithMatrix(
        GLuint cylinderVAO,
        Shader& shader,
        const glm::mat4& transformMatrix,
        const glm::vec3& scale,
        const glm::vec3& ambient,
        const glm::vec3& diffuse,
        const glm::vec3& specular,
        float alpha = 1.0f
    );

    // Render window
    static void renderWindow(
        GLuint windowVAO,
        Shader& shader,
        const glm::vec3& position,
        const glm::vec3& scale,
        const glm::vec3& ambient,
        const glm::vec3& diffuse,
        const glm::vec3& specular,
        float alpha = 0.3f,
        float rotationDegrees = 0.0f,
        const glm::vec3& rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f)
    );

    // Render sphere
    static void renderSphere(
        GLuint sphereVAO,
        Shader& shader,
        const glm::vec3& position,
        const glm::vec3& scale,
        const glm::vec3& ambient,
        const glm::vec3& diffuse,
        const glm::vec3& specular,
        float alpha = 1.0f
    );
};

#endif