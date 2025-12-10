#ifndef SCENE1_LIGHTING_H
#define SCENE1_LIGHTING_H

#include "SimpleScene.h"
#include "scene.h"
#include "shader.h"
#include "texture.h"
#include "config_notexture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <string>

class Scene1_Lighting : public SimpleScene
{
private:
    std::unique_ptr<Shader> lightingShader;
    std::unique_ptr<Shader> lightCubeShader;

    GLuint cubeVAO, lightCubeVAO, VBO;
    unsigned int diffuseMap, specularMap;

    std::vector<glm::vec3> cubePositions;
    std::vector<glm::vec3> pointLightPositions;

public:
    void Init() override
    {
        // Build shaders
        lightingShader = std::make_unique<Shader>(vertexShaderSource, lightingFragmentShaderSource);
        lightCubeShader = std::make_unique<Shader>(vertexShaderSource, lightCubeFragmentShaderSource);

        // Get positions from shared resources
        cubePositions = SceneResources::GetDefaultCubePositions();
        pointLightPositions = SceneResources::GetDefaultPointLightPositions();

        // Set up cube VAO using shared vertices
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(cubeVAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(SceneResources::cubeVertices),
            SceneResources::cubeVertices, GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // Normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // Texture coordinate attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        // Set up light cube VAO (reuses same VBO)
        glGenVertexArrays(1, &lightCubeVAO);
        glBindVertexArray(lightCubeVAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        // Load textures
        diffuseMap = Texture::LoadTexture("C:\\Users\\Admin\\Downloads\\cheems.png");
        specularMap = Texture::LoadTexture("C:\\Users\\Admin\\Downloads\\cheems.png");

        lightingShader->use();
        lightingShader->setInt("material.diffuse", 0);
        lightingShader->setInt("material.specular", 1);
    }

    void Update(float deltaTime) override
    {
        // Add any animation or update logic here
    }

    void Render(Camera& camera, int width, int height) override
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lightingShader->use();
        lightingShader->setVec3("viewPos", camera.Position);
        lightingShader->setFloat("material.shininess", 32.0f);

        // Directional light
        lightingShader->setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        lightingShader->setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        lightingShader->setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        lightingShader->setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

        // Point lights
        for (int i = 0; i < 4; i++)
        {
            std::string number = std::to_string(i);
            lightingShader->setVec3("pointLights[" + number + "].position", pointLightPositions[i]);
            lightingShader->setVec3("pointLights[" + number + "].ambient", 0.05f, 0.05f, 0.05f);
            lightingShader->setVec3("pointLights[" + number + "].diffuse", 0.8f, 0.8f, 0.8f);
            lightingShader->setVec3("pointLights[" + number + "].specular", 1.0f, 1.0f, 1.0f);
            lightingShader->setFloat("pointLights[" + number + "].constant", 1.0f);
            lightingShader->setFloat("pointLights[" + number + "].linear", 0.09f);
            lightingShader->setFloat("pointLights[" + number + "].quadratic", 0.032f);
        }

        // Spot light
        lightingShader->setVec3("spotLight.position", camera.Position);
        lightingShader->setVec3("spotLight.direction", camera.Front);
        lightingShader->setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        lightingShader->setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        lightingShader->setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        lightingShader->setFloat("spotLight.constant", 1.0f);
        lightingShader->setFloat("spotLight.linear", 0.09f);
        lightingShader->setFloat("spotLight.quadratic", 0.032f);
        lightingShader->setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        lightingShader->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
            (float)width / (float)height, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader->setMat4("projection", projection);
        lightingShader->setMat4("view", view);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        glBindVertexArray(cubeVAO);
        for (unsigned int i = 0; i < cubePositions.size(); i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            lightingShader->setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        lightCubeShader->use();
        lightCubeShader->setMat4("projection", projection);
        lightCubeShader->setMat4("view", view);

        glBindVertexArray(lightCubeVAO);
        for (unsigned int i = 0; i < pointLightPositions.size(); i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f));
            lightCubeShader->setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }

    void ProcessInput(GLFWwindow* window, Camera& camera, float deltaTime) override
    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.ProcessKeyboard(FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.ProcessKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    void Cleanup() override
    {
        glDeleteVertexArrays(1, &cubeVAO);
        glDeleteVertexArrays(1, &lightCubeVAO);
        glDeleteBuffers(1, &VBO);
        if (lightingShader) lightingShader->deleteProgram();
        if (lightCubeShader) lightCubeShader->deleteProgram();
    }
};

#endif