#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include "mesh.h"
#include "shader.h"
#include "camera.h"
#include "texture.h"
#include "config_notexture.h"
//#include "config_hastexture.h"

#include "ObjectAnimator.h"



// Function prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);
void renderScene(GLuint cubeVAO, GLuint planeVAO, GLuint sphereVAO, GLuint windowVAO, Shader& lightingShader, glm::mat4& view, glm::mat4& projection);

// Settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 960;

// Camera   
Camera camera(glm::vec3(0.0f, 5.0f, 15.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
bool lightsOn = false;
bool cKeyPressed = false;

// Sun animator
ObjectAnimator* sunAnimator = nullptr;

void renderScene(GLuint cubeVAO, GLuint planeVAO, GLuint sphereVAO, GLuint windowVAO, Shader& lightingShader, glm::mat4& view, glm::mat4& projection)
{
    lightingShader.use();

    // Wall material - beige/cream color
    glm::vec3 wallAmbient(0.4f, 0.35f, 0.25f);
    glm::vec3 wallDiffuse(0.7f, 0.6f, 0.45f);
    glm::vec3 wallSpecular(0.3f, 0.3f, 0.3f);

    // Classroom dimensions
    float classroomWidth = 50.0f;
    float classroomHeight = 15.0f;
    float classroomDepth = 50.0f;
    float wallThickness = 0.2f;

    // Window dimensions
    float windowWidth = 2.5f;
    float windowHeight = 3.0f;
    float windowY = 5.0f; // Center height of windows

    // 1. Render the floor (large plane)
    glBindVertexArray(planeVAO);
    glm::mat4 floorModel = glm::mat4(1.0f);
    floorModel = glm::translate(floorModel, glm::vec3(0.0f, 0.0f, 0.0f));
    floorModel = glm::scale(floorModel, glm::vec3(50.0f, 1.0f, 50.0f));
    lightingShader.setMat4("model", floorModel);

    lightingShader.setVec3("material.ambient", 0.1f, 0.3f, 0.1f);
    lightingShader.setVec3("material.diffuse", 0.2f, 0.6f, 0.2f);
    lightingShader.setVec3("material.specular", 0.1f, 0.1f, 0.1f);
    lightingShader.setFloat("material.alpha", 1.0f);

    glDrawArrays(GL_TRIANGLES, 0, Mesh::GetVertexCount(Mesh::PLANE));

    // 2. Render ceiling
    glBindVertexArray(planeVAO);
    glm::mat4 ceilingModel = glm::mat4(1.0f);
    ceilingModel = glm::translate(ceilingModel, glm::vec3(0.0f, classroomHeight, 0.0f));
    ceilingModel = glm::rotate(ceilingModel, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    ceilingModel = glm::scale(ceilingModel, glm::vec3(classroomWidth, 1.0f, classroomDepth));
    lightingShader.setMat4("model", ceilingModel);

    lightingShader.setVec3("material.ambient", 0.05f, 0.05f, 0.05f);
    lightingShader.setVec3("material.diffuse", 0.1f, 0.1f, 0.1f);
    lightingShader.setVec3("material.specular", 0.05f, 0.05f, 0.05f);
    lightingShader.setFloat("material.alpha", 1.0f);

    glDrawArrays(GL_TRIANGLES, 0, Mesh::GetVertexCount(Mesh::PLANE));

    // Set wall material for all wall segments
    lightingShader.setVec3("material.ambient", wallAmbient);
    lightingShader.setVec3("material.diffuse", wallDiffuse);
    lightingShader.setVec3("material.specular", wallSpecular);
    lightingShader.setFloat("material.alpha", 1.0f);

    glBindVertexArray(cubeVAO);

    // ============================================
    // FRONT WALL (Z = +classroomDepth/2) - SOLID WALL (no windows)
    // ============================================
    float frontZ = classroomDepth / 2.0f;

    // Single solid wall segment (no windows)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, classroomHeight / 2.0f, frontZ));
        model = glm::scale(model, glm::vec3(classroomWidth, classroomHeight, wallThickness));
        lightingShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, Mesh::GetVertexCount(Mesh::CUBE));
    }

    // ============================================
    // BACK WALL (Z = -classroomDepth/2) - 2 windows
    // ============================================
    float backZ = -classroomDepth / 2.0f;

    // Window positions for back wall
    float leftWindowX = -2.0f;
    float rightWindowX = 2.0f;

    // Calculate wall section boundaries
    float leftWindowLeft = leftWindowX - windowWidth / 2.0f;
    float leftWindowRight = leftWindowX + windowWidth / 2.0f;
    float rightWindowLeft = rightWindowX - windowWidth / 2.0f;
    float rightWindowRight = rightWindowX + windowWidth / 2.0f;

    // Far left wall section
    {
        float wallWidth = leftWindowLeft - (-classroomWidth / 2.0f);
        float wallCenterX = -classroomWidth / 2.0f + wallWidth / 2.0f;

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(wallCenterX, classroomHeight / 2.0f, backZ));
        model = glm::scale(model, glm::vec3(wallWidth, classroomHeight, wallThickness));
        lightingShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, Mesh::GetVertexCount(Mesh::CUBE));
    }

    // Left horizontal frames
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(leftWindowX, (windowY - windowHeight / 2.0f) / 2.0f, backZ));
        model = glm::scale(model, glm::vec3(windowWidth, windowY - windowHeight / 2.0f, wallThickness));
        lightingShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, Mesh::GetVertexCount(Mesh::CUBE));
    }
    {
        glm::mat4 model = glm::mat4(1.0f);
        float topY = windowY + windowHeight / 2.0f;
        model = glm::translate(model, glm::vec3(leftWindowX, topY + (classroomHeight - topY) / 2.0f, backZ));
        model = glm::scale(model, glm::vec3(windowWidth, classroomHeight - topY, wallThickness));
        lightingShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, Mesh::GetVertexCount(Mesh::CUBE));
    }

    // Middle wall section
    {
        float wallWidth = rightWindowLeft - leftWindowRight;
        float wallCenterX = leftWindowRight + wallWidth / 2.0f;

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(wallCenterX, classroomHeight / 2.0f, backZ));
        model = glm::scale(model, glm::vec3(wallWidth, classroomHeight, wallThickness));
        lightingShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, Mesh::GetVertexCount(Mesh::CUBE));
    }

    // Right horizontal frames
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(rightWindowX, (windowY - windowHeight / 2.0f) / 2.0f, backZ));
        model = glm::scale(model, glm::vec3(windowWidth, windowY - windowHeight / 2.0f, wallThickness));
        lightingShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, Mesh::GetVertexCount(Mesh::CUBE));
    }
    {
        glm::mat4 model = glm::mat4(1.0f);
        float topY = windowY + windowHeight / 2.0f;
        model = glm::translate(model, glm::vec3(rightWindowX, topY + (classroomHeight - topY) / 2.0f, backZ));
        model = glm::scale(model, glm::vec3(windowWidth, classroomHeight - topY, wallThickness));
        lightingShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, Mesh::GetVertexCount(Mesh::CUBE));
    }

    // Far right wall section
    {
        float wallWidth = (classroomWidth / 2.0f) - rightWindowRight;
        float wallCenterX = rightWindowRight + wallWidth / 2.0f;

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(wallCenterX, classroomHeight / 2.0f, backZ));
        model = glm::scale(model, glm::vec3(wallWidth, classroomHeight, wallThickness));
        lightingShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, Mesh::GetVertexCount(Mesh::CUBE));
    }

    // ============================================
    // LEFT WALL (X = -classroomWidth/2) - 1 window
    // ============================================
    float leftX = -classroomWidth / 2.0f;
    float centerWindowZ = 0.0f;

    // Calculate wall section boundaries along Z axis
    float windowFront = centerWindowZ + windowWidth / 2.0f;
    float windowBack = centerWindowZ - windowWidth / 2.0f;

    // Front wall section (from front edge to window)
    {
        float wallDepth = (classroomDepth / 2.0f) - windowFront;
        float wallCenterZ = windowFront + wallDepth / 2.0f;

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(leftX, classroomHeight / 2.0f, wallCenterZ));
        model = glm::scale(model, glm::vec3(wallThickness, classroomHeight, wallDepth));
        lightingShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, Mesh::GetVertexCount(Mesh::CUBE));
    }

    // Horizontal frame below window
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(leftX, (windowY - windowHeight / 2.0f) / 2.0f, centerWindowZ));
        model = glm::scale(model, glm::vec3(wallThickness, windowY - windowHeight / 2.0f, windowWidth));
        lightingShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, Mesh::GetVertexCount(Mesh::CUBE));
    }

    // Horizontal frame above window
    {
        glm::mat4 model = glm::mat4(1.0f);
        float topY = windowY + windowHeight / 2.0f;
        model = glm::translate(model, glm::vec3(leftX, topY + (classroomHeight - topY) / 2.0f, centerWindowZ));
        model = glm::scale(model, glm::vec3(wallThickness, classroomHeight - topY, windowWidth));
        lightingShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, Mesh::GetVertexCount(Mesh::CUBE));
    }

    // Back wall section (from window to back edge)
    {
        float wallDepth = windowBack - (-classroomDepth / 2.0f);
        float wallCenterZ = -classroomDepth / 2.0f + wallDepth / 2.0f;

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(leftX, classroomHeight / 2.0f, wallCenterZ));
        model = glm::scale(model, glm::vec3(wallThickness, classroomHeight, wallDepth));
        lightingShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, Mesh::GetVertexCount(Mesh::CUBE));
    }

    // ============================================
    // RIGHT WALL (X = +classroomWidth/2) - 1 window
    // ============================================
    float rightX = classroomWidth / 2.0f;

    // Front wall section
    {
        float wallDepth = (classroomDepth / 2.0f) - windowFront;
        float wallCenterZ = windowFront + wallDepth / 2.0f;

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(rightX, classroomHeight / 2.0f, wallCenterZ));
        model = glm::scale(model, glm::vec3(wallThickness, classroomHeight, wallDepth));
        lightingShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, Mesh::GetVertexCount(Mesh::CUBE));
    }

    // Horizontal frame below window
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(rightX, (windowY - windowHeight / 2.0f) / 2.0f, centerWindowZ));
        model = glm::scale(model, glm::vec3(wallThickness, windowY - windowHeight / 2.0f, windowWidth));
        lightingShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, Mesh::GetVertexCount(Mesh::CUBE));
    }

    // Horizontal frame above window
    {
        glm::mat4 model = glm::mat4(1.0f);
        float topY = windowY + windowHeight / 2.0f;
        model = glm::translate(model, glm::vec3(rightX, topY + (classroomHeight - topY) / 2.0f, centerWindowZ));
        model = glm::scale(model, glm::vec3(wallThickness, classroomHeight - topY, windowWidth));
        lightingShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, Mesh::GetVertexCount(Mesh::CUBE));
    }

    // Back wall section
    {
        float wallDepth = windowBack - (-classroomDepth / 2.0f);
        float wallCenterZ = -classroomDepth / 2.0f + wallDepth / 2.0f;

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(rightX, classroomHeight / 2.0f, wallCenterZ));
        model = glm::scale(model, glm::vec3(wallThickness, classroomHeight, wallDepth));
        lightingShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, Mesh::GetVertexCount(Mesh::CUBE));
    }

    // ============================================
    // RENDER WINDOWS (with transparency)
    // ============================================
    glDepthMask(GL_FALSE);
    glBindVertexArray(windowVAO);

    lightingShader.setVec3("material.ambient", 0.1f, 0.1f, 0.1f);
    lightingShader.setVec3("material.diffuse", 0.2f, 0.2f, 0.2f);
    lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
    lightingShader.setFloat("material.alpha", 0.3f);

    // Back wall windows
    for (int i = 0; i < 2; i++)
    {
        glm::mat4 windowModel = glm::mat4(1.0f);
        windowModel = glm::translate(windowModel, glm::vec3(-2.0f + i * 4.0f, windowY, backZ - 0.01f));
        windowModel = glm::rotate(windowModel, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        windowModel = glm::scale(windowModel, glm::vec3(windowWidth, windowHeight, 1.0f));
        lightingShader.setMat4("model", windowModel);
        glDrawArrays(GL_TRIANGLES, 0, Mesh::GetVertexCount(Mesh::WINDOW));
    }

    // Left wall window
    {
        glm::mat4 windowModel = glm::mat4(1.0f);
        windowModel = glm::translate(windowModel, glm::vec3(leftX - 0.01f, windowY, 0.0f));
        windowModel = glm::rotate(windowModel, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        windowModel = glm::scale(windowModel, glm::vec3(windowWidth, windowHeight, 1.0f));
        lightingShader.setMat4("model", windowModel);
        glDrawArrays(GL_TRIANGLES, 0, Mesh::GetVertexCount(Mesh::WINDOW));
    }

    // Right wall window
    {
        glm::mat4 windowModel = glm::mat4(1.0f);
        windowModel = glm::translate(windowModel, glm::vec3(rightX + 0.01f, windowY, 0.0f));
        windowModel = glm::rotate(windowModel, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        windowModel = glm::scale(windowModel, glm::vec3(windowWidth, windowHeight, 1.0f));
        lightingShader.setMat4("model", windowModel);
        glDrawArrays(GL_TRIANGLES, 0, Mesh::GetVertexCount(Mesh::WINDOW));
    }

    glDepthMask(GL_TRUE);
}


void renderCeilingLights(GLuint cubeVAO, Shader& lightCubeShader, glm::mat4& view, glm::mat4& projection, const glm::vec3 lightPositions[4], bool lightsOn)
{
    lightCubeShader.use();
    lightCubeShader.setMat4("projection", projection);
    lightCubeShader.setMat4("view", view);

    // Set light color based on whether lights are on or off
    if (lightsOn)
    {
        // Bright white when lights are on
        lightCubeShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
    }
    else
    {
        // Dim grey when lights are off
        lightCubeShader.setVec3("lightColor", 0.15f, 0.15f, 0.15f);
    }

    glBindVertexArray(cubeVAO);

    // Render 4 elongated light fixtures
    for (int i = 0; i < 4; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, lightPositions[i]);
        // Make them long and thin (fluorescent tube style)
        model = glm::scale(model, glm::vec3(6.0f, 0.2f, 0.5f));
        lightCubeShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, Mesh::GetVertexCount(Mesh::CUBE));
    }
}

void renderDesk(GLuint cubeVAO, GLuint planeVAO, Shader& shader, glm::mat4& view, glm::mat4& projection, glm::vec3 position)
{
    shader.use();
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);

    // Desk dimensions
    float mainSurfaceWidth = 5.0f;
    float mainSurfaceDepth = 1.75f;
    float mainSurfaceThickness = 0.2f;

    float subSurfaceWidth = 5.0f;
    float subSurfaceDepth = 1.5f;
    float subSurfaceThickness = 0.1f;

    float frontPanelThickness = 0.1f;
    float frontPanelHeight = 1.5f;

    float tableHeight = 3.0f;
    float legWidth = 0.1f;

    float gapBetweenSurfaces = 0.8f;

    // Wood color (brown)
    glm::vec3 woodAmbient(0.3f, 0.2f, 0.1f);
    glm::vec3 woodDiffuse(0.6f, 0.4f, 0.2f);
    glm::vec3 woodSpecular(0.3f, 0.2f, 0.1f);

    shader.setVec3("material.ambient", woodAmbient);
    shader.setVec3("material.diffuse", woodDiffuse);
    shader.setVec3("material.specular", woodSpecular);
    shader.setFloat("material.shininess", 32.0f);
    shader.setFloat("material.alpha", 1.0f);

    // 1. MAIN SURFACE
    glBindVertexArray(cubeVAO);
    glm::mat4 mainSurfaceModel = glm::mat4(1.0f);
    mainSurfaceModel = glm::translate(mainSurfaceModel, position + glm::vec3(0.0f, tableHeight - mainSurfaceThickness / 2.0f, 0.0f));
    mainSurfaceModel = glm::scale(mainSurfaceModel, glm::vec3(mainSurfaceWidth, mainSurfaceThickness, mainSurfaceDepth));
    shader.setMat4("model", mainSurfaceModel);
    glDrawArrays(GL_TRIANGLES, 0, Mesh::GetVertexCount(Mesh::CUBE));

    // 2. SUB-SURFACE
    float subSurfaceY = tableHeight - mainSurfaceThickness - gapBetweenSurfaces - subSurfaceThickness / 2.0f;
    glm::mat4 subSurfaceModel = glm::mat4(1.0f);
    float subSurfaceZOffset = -(mainSurfaceDepth - subSurfaceDepth) / 2.0f;
    subSurfaceModel = glm::translate(subSurfaceModel, position + glm::vec3(0.0f, subSurfaceY, -subSurfaceZOffset));
    subSurfaceModel = glm::scale(subSurfaceModel, glm::vec3(subSurfaceWidth, subSurfaceThickness, subSurfaceDepth));
    shader.setMat4("model", subSurfaceModel);
    glDrawArrays(GL_TRIANGLES, 0, Mesh::GetVertexCount(Mesh::CUBE));

    // 3. PANELS
    float frontPanelY = (tableHeight - mainSurfaceThickness + subSurfaceY - subSurfaceThickness / 2.0f) / 2.0f;
    float frontPanelHeight_calc = (tableHeight - mainSurfaceThickness) - (subSurfaceY - subSurfaceThickness / 2.0f);

    glm::mat4 frontPanelModel = glm::mat4(1.0f);
    frontPanelModel = glm::translate(frontPanelModel, position + glm::vec3(0.0f, frontPanelY, mainSurfaceDepth / 2.0f - frontPanelThickness / 2.0f));
    frontPanelModel = glm::scale(frontPanelModel, glm::vec3(mainSurfaceWidth, frontPanelHeight_calc, frontPanelThickness));
    shader.setMat4("model", frontPanelModel);
    glDrawArrays(GL_TRIANGLES, 0, Mesh::GetVertexCount(Mesh::CUBE));

    glm::mat4 leftPanelModel = glm::mat4(1.0f);
    leftPanelModel = glm::translate(leftPanelModel, position + glm::vec3(-mainSurfaceWidth / 2.0f + frontPanelThickness / 2.0f, frontPanelY, 0.0f));
    leftPanelModel = glm::scale(leftPanelModel, glm::vec3(frontPanelThickness, frontPanelHeight_calc, mainSurfaceDepth));
    shader.setMat4("model", leftPanelModel);
    glDrawArrays(GL_TRIANGLES, 0, Mesh::GetVertexCount(Mesh::CUBE));

    glm::mat4 rightPanelModel = glm::mat4(1.0f);
    rightPanelModel = glm::translate(rightPanelModel, position + glm::vec3(mainSurfaceWidth / 2.0f - frontPanelThickness / 2.0f, frontPanelY, 0.0f));
    rightPanelModel = glm::scale(rightPanelModel, glm::vec3(frontPanelThickness, frontPanelHeight_calc, mainSurfaceDepth));
    shader.setMat4("model", rightPanelModel);
    glDrawArrays(GL_TRIANGLES, 0, Mesh::GetVertexCount(Mesh::CUBE));

    // 4. FOUR LEGS
    float legOffsetX = mainSurfaceWidth / 2.0f - legWidth / 2.0f;
    float frontlegOffsetZ = mainSurfaceDepth / 2.0f - legWidth / 2.0f;
    float backlegOffsetZ = subSurfaceDepth / 2.0f - legWidth * 4.0f;
    float legHeight = subSurfaceY - subSurfaceThickness / 2.0f;

    glm::mat4 leg1Model = glm::mat4(1.0f);
    leg1Model = glm::translate(leg1Model, position + glm::vec3(-legOffsetX, legHeight / 2.0f, frontlegOffsetZ));
    leg1Model = glm::scale(leg1Model, glm::vec3(legWidth, legHeight, legWidth));
    shader.setMat4("model", leg1Model);
    glDrawArrays(GL_TRIANGLES, 0, Mesh::GetVertexCount(Mesh::CUBE));

    glm::mat4 leg2Model = glm::mat4(1.0f);
    leg2Model = glm::translate(leg2Model, position + glm::vec3(legOffsetX, legHeight / 2.0f, frontlegOffsetZ));
    leg2Model = glm::scale(leg2Model, glm::vec3(legWidth, legHeight, legWidth));
    shader.setMat4("model", leg2Model);
    glDrawArrays(GL_TRIANGLES, 0, Mesh::GetVertexCount(Mesh::CUBE));

    glm::mat4 leg3Model = glm::mat4(1.0f);
    leg3Model = glm::translate(leg3Model, position + glm::vec3(-legOffsetX, legHeight / 2.0f, -backlegOffsetZ));
    leg3Model = glm::scale(leg3Model, glm::vec3(legWidth, legHeight, legWidth));
    shader.setMat4("model", leg3Model);
    glDrawArrays(GL_TRIANGLES, 0, Mesh::GetVertexCount(Mesh::CUBE));

    glm::mat4 leg4Model = glm::mat4(1.0f);
    leg4Model = glm::translate(leg4Model, position + glm::vec3(legOffsetX, legHeight / 2.0f, -backlegOffsetZ));
    leg4Model = glm::scale(leg4Model, glm::vec3(legWidth, legHeight, legWidth));
    shader.setMat4("model", leg4Model);
    glDrawArrays(GL_TRIANGLES, 0, Mesh::GetVertexCount(Mesh::CUBE));
}

void renderCeilingFan(GLuint cubeVAO, GLuint cylinderVAO, Shader& shader, glm::mat4& view, glm::mat4& projection, float currentTime)
{
    shader.use();
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);

    glm::vec3 fanPosition(0.0f, 8.5f, 0.0f);

    float fanSpeed = 2.0f;
    float rotation = currentTime * fanSpeed * 360.0f;

    // CEILING ROD
    shader.setVec3("material.ambient", 0.15f, 0.15f, 0.15f);
    shader.setVec3("material.diffuse", 0.25f, 0.25f, 0.25f);
    shader.setVec3("material.specular", 0.4f, 0.4f, 0.4f);
    shader.setFloat("material.alpha", 1.0f);

    glBindVertexArray(cylinderVAO);
    glm::mat4 ceilingRodModel = glm::mat4(1.0f);
    ceilingRodModel = glm::translate(ceilingRodModel, glm::vec3(0.0f, (10.0f + fanPosition.y) / 2.0f, 0.0f));
    ceilingRodModel = glm::scale(ceilingRodModel, glm::vec3(0.15f, 10.0f - fanPosition.y, 0.15f));
    shader.setMat4("model", ceilingRodModel);
    glDrawArrays(GL_TRIANGLES, 0, Mesh::GetVertexCount(Mesh::CYLINDER));

    // CENTRAL DISK
    shader.setVec3("material.ambient", 0.7f, 0.7f, 0.7f);
    shader.setVec3("material.diffuse", 0.85f, 0.85f, 0.85f);
    shader.setVec3("material.specular", 0.9f, 0.9f, 0.9f);
    shader.setFloat("material.alpha", 1.0f);

    glBindVertexArray(cylinderVAO);
    glm::mat4 diskModel = glm::mat4(1.0f);
    diskModel = glm::translate(diskModel, fanPosition);
    diskModel = glm::rotate(diskModel, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
    diskModel = glm::scale(diskModel, glm::vec3(1.5f, 0.3f, 1.5f));
    shader.setMat4("model", diskModel);
    glDrawArrays(GL_TRIANGLES, 0, Mesh::GetVertexCount(Mesh::CYLINDER));

    // CONNECTOR STICKS AND BLADES
    for (int i = 0; i < 3; i++)
    {
        float angle = rotation + (i * 120.0f);

        // CONNECTOR STICK
        shader.setVec3("material.ambient", 0.2f, 0.2f, 0.2f);
        shader.setVec3("material.diffuse", 0.35f, 0.35f, 0.35f);
        shader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
        shader.setFloat("material.alpha", 1.0f);

        glBindVertexArray(cylinderVAO);
        glm::mat4 connectorModel = glm::mat4(1.0f);
        connectorModel = glm::translate(connectorModel, fanPosition);
        connectorModel = glm::rotate(connectorModel, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
        connectorModel = glm::translate(connectorModel, glm::vec3(1.2f, 0.0f, 0.0f));
        connectorModel = glm::rotate(connectorModel, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        connectorModel = glm::scale(connectorModel, glm::vec3(0.1f, 0.5f, 0.1f));
        shader.setMat4("model", connectorModel);
        glDrawArrays(GL_TRIANGLES, 0, Mesh::GetVertexCount(Mesh::CYLINDER));

        // BLADE
        shader.setVec3("material.ambient", 0.7f, 0.7f, 0.7f);
        shader.setVec3("material.diffuse", 0.85f, 0.85f, 0.85f);
        shader.setVec3("material.specular", 0.9f, 0.9f, 0.9f);
        shader.setFloat("material.alpha", 1.0f);

        glBindVertexArray(cubeVAO);
        glm::mat4 bladeModel = glm::mat4(1.0f);
        bladeModel = glm::translate(bladeModel, fanPosition);
        bladeModel = glm::rotate(bladeModel, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
        bladeModel = glm::translate(bladeModel, glm::vec3(2.5f, 0.0f, 0.0f));
        bladeModel = glm::scale(bladeModel, glm::vec3(2.5f, 0.08f, 0.6f));

        shader.setMat4("model", bladeModel);
        glDrawArrays(GL_TRIANGLES, 0, Mesh::GetVertexCount(Mesh::CUBE));
    }
}

void renderBench(
    GLuint cubeVAO,
    Shader& shader,
    glm::mat4& view,
    glm::mat4& projection,
    glm::vec3 position,
    float benchWidth = 11.0f,
    float benchDepth = 1.5f,
    float benchHeight = 1.5f,
    float legWidth = 0.1f,
    float legHeight = 1.5f
) {
    shader.use();
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);

    glm::vec3 benchAmbient(0.2f, 0.13f, 0.07f);
    glm::vec3 benchDiffuse(0.4f, 0.25f, 0.13f);
    glm::vec3 benchSpecular(0.2f, 0.13f, 0.07f);

    shader.setVec3("material.ambient", benchAmbient);
    shader.setVec3("material.diffuse", benchDiffuse);
    shader.setVec3("material.specular", benchSpecular);
    shader.setFloat("material.shininess", 16.0f);
    shader.setFloat("material.alpha", 1.0f);

    glBindVertexArray(cubeVAO);
    glm::mat4 seatModel = glm::mat4(1.0f);
    seatModel = glm::translate(seatModel, position + glm::vec3(0.0f, benchHeight, 0.0f));
    seatModel = glm::scale(seatModel, glm::vec3(benchWidth, 0.1f, benchDepth));
    shader.setMat4("model", seatModel);
    glDrawArrays(GL_TRIANGLES, 0, Mesh::GetVertexCount(Mesh::CUBE));

    float xOffset = benchWidth / 2.0f - legWidth / 2.0f;
    float zOffset = benchDepth / 2.0f - legWidth / 2.0f;
    float yOffset = legHeight / 2.0f;

    for (int sx = -1; sx <= 1; sx += 2) {
        for (int sz = -1; sz <= 1; sz += 2) {
            glm::mat4 legModel = glm::mat4(1.0f);
            legModel = glm::translate(legModel, position + glm::vec3(sx * xOffset, yOffset, sz * zOffset));
            legModel = glm::scale(legModel, glm::vec3(legWidth, legHeight, legWidth));
            shader.setMat4("model", legModel);
            glDrawArrays(GL_TRIANGLES, 0, Mesh::GetVertexCount(Mesh::CUBE));
        }
    }
}

void renderBlackBoard(GLuint cubeVAO, Shader& shader, glm::mat4& view, glm::mat4& projection, glm::vec3 position)
{
    shader.use();
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);

    // Blackboard dimensions
    float boardWidth = 25.0f;
    float boardHeight = 8.0f;
    float boardThickness = 0.15f;
    float frameThickness = 0.2f;

    // Dark green/black color for the board surface
    glm::vec3 boardAmbient(0.05f, 0.1f, 0.05f);
    glm::vec3 boardDiffuse(0.1f, 0.2f, 0.1f);
    glm::vec3 boardSpecular(0.05f, 0.05f, 0.05f);

    // Brown color for the wooden frame
    glm::vec3 frameAmbient(0.3f, 0.2f, 0.1f);
    glm::vec3 frameDiffuse(0.5f, 0.3f, 0.15f);
    glm::vec3 frameSpecular(0.2f, 0.15f, 0.1f);

    glBindVertexArray(cubeVAO);

    // Main blackboard surface
    shader.setVec3("material.ambient", boardAmbient);
    shader.setVec3("material.diffuse", boardDiffuse);
    shader.setVec3("material.specular", boardSpecular);
    shader.setFloat("material.alpha", 1.0f);

    glm::mat4 boardModel = glm::mat4(1.0f);
    boardModel = glm::translate(boardModel, position);
    boardModel = glm::scale(boardModel, glm::vec3(boardWidth, boardHeight, boardThickness));
    shader.setMat4("model", boardModel);
    glDrawArrays(GL_TRIANGLES, 0, Mesh::GetVertexCount(Mesh::CUBE));

    // Wooden frame around the blackboard
    shader.setVec3("material.ambient", frameAmbient);
    shader.setVec3("material.diffuse", frameDiffuse);
    shader.setVec3("material.specular", frameSpecular);
    shader.setFloat("material.alpha", 1.0f);

    // Top frame
    glm::mat4 topFrame = glm::mat4(1.0f);
    topFrame = glm::translate(topFrame, position + glm::vec3(0.0f, boardHeight / 2.0f + frameThickness / 2.0f, 0.0f));
    topFrame = glm::scale(topFrame, glm::vec3(boardWidth + 2.0f * frameThickness, frameThickness, boardThickness + 0.1f));
    shader.setMat4("model", topFrame);
    glDrawArrays(GL_TRIANGLES, 0, Mesh::GetVertexCount(Mesh::CUBE));

    // Bottom frame
    glm::mat4 bottomFrame = glm::mat4(1.0f);
    bottomFrame = glm::translate(bottomFrame, position + glm::vec3(0.0f, -boardHeight / 2.0f - frameThickness / 2.0f, 0.0f));
    bottomFrame = glm::scale(bottomFrame, glm::vec3(boardWidth + 2.0f * frameThickness, frameThickness, boardThickness + 0.1f));
    shader.setMat4("model", bottomFrame);
    glDrawArrays(GL_TRIANGLES, 0, Mesh::GetVertexCount(Mesh::CUBE));

    // Left frame
    glm::mat4 leftFrame = glm::mat4(1.0f);
    leftFrame = glm::translate(leftFrame, position + glm::vec3(-boardWidth / 2.0f - frameThickness / 2.0f, 0.0f, 0.0f));
    leftFrame = glm::scale(leftFrame, glm::vec3(frameThickness, boardHeight, boardThickness + 0.1f));
    shader.setMat4("model", leftFrame);
    glDrawArrays(GL_TRIANGLES, 0, Mesh::GetVertexCount(Mesh::CUBE));

    // Right frame
    glm::mat4 rightFrame = glm::mat4(1.0f);
    rightFrame = glm::translate(rightFrame, position + glm::vec3(boardWidth / 2.0f + frameThickness / 2.0f, 0.0f, 0.0f));
    rightFrame = glm::scale(rightFrame, glm::vec3(frameThickness, boardHeight, boardThickness + 0.1f));
    shader.setMat4("model", rightFrame);
    glDrawArrays(GL_TRIANGLES, 0, Mesh::GetVertexCount(Mesh::CUBE));

    // Chalk tray at the bottom
    glm::mat4 chalkTray = glm::mat4(1.0f);
    chalkTray = glm::translate(chalkTray, position + glm::vec3(0.0f, -boardHeight / 2.0f - frameThickness - 0.15f, 0.1f));
    chalkTray = glm::scale(chalkTray, glm::vec3(boardWidth, 0.1f, 0.3f));
    shader.setMat4("model", chalkTray);
    glDrawArrays(GL_TRIANGLES, 0, Mesh::GetVertexCount(Mesh::CUBE));
}


int main()
{
    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Classroom Scene", NULL, NULL);
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
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Shader lightingShader(vertexShaderSource, lightingFragmentShaderSource);
    Shader lightCubeShader(vertexShaderSource, lightCubeFragmentShaderSource);

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

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

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

    sunAnimator = new ObjectAnimator(glm::vec3(30.0f, 20.0f, -50.0f));
    sunAnimator->setAnimationType(CIRCULAR);
    sunAnimator->setRadius(15.0f);
    sunAnimator->setSpeed(0.1f);
    sunAnimator->setCenter(glm::vec3(0.0f, 20.0f, -50.0f));
    sunAnimator->setAxis(glm::vec3(0.0f, 0.0f, 1.0f));

    glm::vec3 ceilingLightPositions[4] = {
        glm::vec3(-10.0f, 9.5f, -10.0f),
        glm::vec3(10.0f, 9.5f, -10.0f),
        glm::vec3(-10.0f, 9.5f, 10.0f),
        glm::vec3(10.0f, 9.5f, 10.0f)
    };

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.5f, 0.7f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::vec3 sunPosition = sunAnimator->update(currentFrame);

        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);
        lightingShader.setFloat("material.shininess", 32.0f);

        glm::vec3 sunDirection = glm::normalize(glm::vec3(0.0f, 0.0f, 0.0f) - sunPosition);
        lightingShader.setVec3("dirLight.direction", sunDirection);
        lightingShader.setVec3("dirLight.ambient", 0.3f, 0.3f, 0.3f);
        lightingShader.setVec3("dirLight.diffuse", 0.8f, 0.8f, 0.7f);
        lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

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

        lightingShader.setVec3("spotLight.position", glm::vec3(0.0f));
        lightingShader.setVec3("spotLight.direction", glm::vec3(0.0f, -1.0f, 0.0f));
        lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        lightingShader.setVec3("spotLight.diffuse", 0.0f, 0.0f, 0.0f);
        lightingShader.setVec3("spotLight.specular", 0.0f, 0.0f, 0.0f);
        lightingShader.setFloat("spotLight.constant", 1.0f);
        lightingShader.setFloat("spotLight.linear", 0.09f);
        lightingShader.setFloat("spotLight.quadratic", 0.032f);
        lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 500.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        renderScene(cubeVAO, planeVAO, sphereVAO, windowVAO, lightingShader, view, projection);

        // Render the blackboard on the front wall, left side with 10.0f offset
        float classroomDepth = 50.0f;
        float frontZ = classroomDepth / 2.0f;
        renderBlackBoard(
            cubeVAO,
            lightingShader,
            view,
            projection,
            glm::vec3(10.0f, 8.0f, frontZ - 0.2f)
        );

        renderCeilingLights(cubeVAO, lightCubeShader, view, projection, ceilingLightPositions, lightsOn);

        renderCeilingFan(cubeVAO, cylinderVAO, lightingShader, view, projection, currentFrame);

        const int numRows = 6;
        const int numCols = 2;
        const int numDesksInACell = 3;
        const float rowSpacing = 6.0f;
        const float colSpacing = 10.0f;
        const float pairSpacing = 0.5f;
        const float startX = -20.0f;
        const float startZ = -15.0f;
        const float tableSizeX = 5.0f;

        float deskStride = tableSizeX + pairSpacing;
        float columnWidth = numDesksInACell * deskStride;

        for (int row = 0; row < numRows; row++)
        {
            for (int col = 0; col < numCols; col++)
            {
                renderBench(
                    cubeVAO,
                    lightingShader,
                    view,
                    projection,
                    glm::vec3(
                        startX
                        + col * (columnWidth + colSpacing)
                        + (columnWidth - pairSpacing) / 2.0f
                        - deskStride / 2.0f,
                        0.0f,
                        startZ
                        + row * rowSpacing
                        - rowSpacing / 2.0f
                    ),
                    columnWidth
                );
                for (int i = 0; i < numDesksInACell; i++)
                {
                    float x = startX
                        + col * (columnWidth + colSpacing)
                        + i * deskStride;

                    float z = startZ
                        + row * rowSpacing;

                    renderDesk(
                        cubeVAO,
                        planeVAO,
                        lightingShader,
                        view,
                        projection,
                        glm::vec3(x, 0.0f, z)
                    );
                }
            }
        }

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

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &planeVAO);
    glDeleteVertexArrays(1, &sphereVAO);
    glDeleteVertexArrays(1, &windowVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &planeVBO);
    glDeleteBuffers(1, &sphereVBO);
    glDeleteBuffers(1, &windowVBO);
    lightingShader.deleteProgram();
    lightCubeShader.deleteProgram();

    delete sunAnimator;

    glfwTerminate();

    glDeleteVertexArrays(1, &cylinderVAO);
    glDeleteBuffers(1, &cylinderVBO);
    return 0;
}

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