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

#include "ObjectAnimator.h"  // Include the animator


enum SceneType
{
    SCENE_A = 0,
    SCENE_B = 1
};

float rotationAngle = 0.0f;
bool rPressedLastFrame = false;


SceneType currentScene = SCENE_A;

// Prevent key repeat toggle
bool enterPressedLastFrame = false;

// Function prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);

// Settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 1600;

// Camera   
Camera camera(glm::vec3(0.0f, 0.0f, 8.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    void renderSceneA(GLuint cubeVAO, Shader & lightingShader, glm::mat4 & view, glm::mat4 & projection, glm::vec3 cubePositions[]);
    void renderSceneB(GLuint cubeVAO, Shader & lightingShader, glm::mat4 & view, glm::mat4 & projection, glm::vec3 cubePositions[]);


    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Lighting with Animation", NULL, NULL);
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

    // Build shaders
    Shader lightingShader(vertexShaderSource, lightingFragmentShaderSource);
    Shader lightCubeShader(vertexShaderSource, lightCubeFragmentShaderSource);

    

    // Positions for 10 objets
    glm::vec3 objectPositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    // STEP 5: Create Animators for Boxes
    // Animate the first cube (index 0) with circular motion
    ObjectAnimator boxAnimator1(objectPositions[0]);
    boxAnimator1.setAnimationType(CIRCULAR);
    boxAnimator1.setRadius(2.0f);
    boxAnimator1.setSpeed(1.0f);
    boxAnimator1.setCenter(glm::vec3(0.0f, 0.0f, 0.0f));

    // Animate the second cube (index 1) with bounce
    ObjectAnimator boxAnimator2(objectPositions[1]);
    boxAnimator2.setAnimationType(BOUNCE);
    boxAnimator2.setSpeed(2.0f);
    boxAnimator2.setAmplitude(glm::vec3(0.0f, 3.0f, 0.0f));

    // Positions of the 4 point lights
    glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f,  0.2f,  2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3(0.0f,  0.0f, -3.0f)
    };

    //// STEP 6: Create Animators for Lights
    //// Animate first light in circular motion
    //ObjectAnimator lightAnimator1(pointLightPositions[0]);
    //lightAnimator1.setAnimationType(CIRCULAR);
    //lightAnimator1.setRadius(4.0f);
    //lightAnimator1.setSpeed(0.5f);
    //lightAnimator1.setCenter(glm::vec3(0.0f, 0.0f, 0.0f));

    //// Animate second light in figure-8 pattern
    //ObjectAnimator lightAnimator2(pointLightPositions[1]);
    //lightAnimator2.setAnimationType(FIGURE_EIGHT);
    //lightAnimator2.setRadius(3.0f);
    //lightAnimator2.setSpeed(0.8f);
    //lightAnimator2.setCenter(glm::vec3(0.0f, 0.0f, -5.0f));

    //// Third light orbits around Y-axis
    //ObjectAnimator lightAnimator3(pointLightPositions[2]);
    //lightAnimator3.setAnimationType(ORBIT);
    //lightAnimator3.setSpeed(0.3f);
    //lightAnimator3.setCenter(glm::vec3(-2.0f, 0.0f, -8.0f));
    //lightAnimator3.setAxis(glm::vec3(0.0f, 1.0f, 0.0f));

    //// STEP 7: Custom Animation for Fourth Light
    //lightAnimator3.setAnimationType(CUSTOM);
    //lightAnimator3.setCustomFunction([](glm::vec3 start, float time, float speed) {
    //    // Spiral motion
    //    float radius = 2.0f + sin(time * 0.5f) * 1.0f;
    //    float x = start.x + radius * cos(time);
    //    float y = start.y + sin(time * 2.0f) * 2.0f;
    //    float z = start.z + radius * sin(time);
    //    return glm::vec3(x, y, z);
    //    });

    // Set up cube VAO and VBO
    GLuint cubeVAO, VBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(cubeVAO);
    Mesh::Type currentMesh = Mesh::TETRAHEDRON;

    const auto& verts = Mesh::GetVertices(currentMesh);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
        verts.size() * sizeof(float),
        verts.data(),
        GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    GLuint lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    //load texture from image
    unsigned int diffuseMap = Texture::LoadTexture("C:\\Users\\Admin\\Downloads\\cheems.png");
    unsigned int specularMap = Texture::LoadTexture("C:\\Users\\Admin\\Downloads\\cheems.png");

    lightingShader.use();
    lightingShader.setInt("material.diffuse", 0);
    lightingShader.setInt("material.specular", 1);

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // STEP 8: Update Animations - Call update() each frame
        objectPositions[0] = boxAnimator1.update(currentFrame);
        objectPositions[1] = boxAnimator2.update(currentFrame);

        //pointLightPositions[0] = lightAnimator1.update(currentFrame);
        //pointLightPositions[1] = lightAnimator2.update(currentFrame);
        //pointLightPositions[2] = lightAnimator3.update(currentFrame);

        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);
        lightingShader.setFloat("material.shininess", 32.0f);




        // Point lights with updated positions
        for (int i = 0; i < 4; i++)
        {

            std::string number = std::to_string(i);

            glm::vec3 spotLightPos = objectPositions[i];   // light moves with object
            glm::vec3 targetPos = objectPositions[1];   // light points to another object
            glm::vec3 spotDir = glm::normalize(targetPos - spotLightPos);
            
            
            //set materials colors

            //direction light
            lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
            lightingShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);  // Orange-ish color
            lightingShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
            lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);

            //point light
            lightingShader.setVec3("pointLights[" + number + "].position", pointLightPositions[i]);
            lightingShader.setVec3("pointLights[" + number + "].ambient", 0.05f, 0.05f, 0.05f);
            lightingShader.setVec3("pointLights[" + number + "].diffuse", 0.8f, 0.8f, 0.8f);
            lightingShader.setVec3("pointLights[" + number + "].specular", 1.0f, 1.0f, 1.0f);
            lightingShader.setFloat("pointLights[" + number + "].constant", 1.0f);
            lightingShader.setFloat("pointLights[" + number + "].linear", 0.09f);
            lightingShader.setFloat("pointLights[" + number + "].quadratic", 0.032f);





        }

        //spot light
        lightingShader.setVec3("spotLight.position", glm::vec3(0.0f, 3.0f, 0.0f));     // light location
        lightingShader.setVec3("spotLight.direction", glm::vec3(0.0f, -1.0f, 0.0f));

        lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("spotLight.constant", 1.0f);
        lightingShader.setFloat("spotLight.linear", 0.09f);
        lightingShader.setFloat("spotLight.quadratic", 0.032f);
        lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);


        //////bind texture
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, diffuseMap);
        //glActiveTexture(GL_TEXTURE1);
        //glBindTexture(GL_TEXTURE_2D, specularMap);




        // Render based on active scene
        if (currentScene == SCENE_A)
        {
            renderSceneA(cubeVAO, lightingShader, view, projection, objectPositions);
        }
        else
        {
            renderSceneB(cubeVAO, lightingShader, view, projection, objectPositions);
        }


        // Draw light cubes with updated positions
        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);

        glBindVertexArray(lightCubeVAO);
        for (unsigned int i = 0; i < 4; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f));
            lightCubeShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);
    lightingShader.deleteProgram();
    lightCubeShader.deleteProgram();
    glfwTerminate();
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

    // ===== Scene toggle using Enter =====
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS && !enterPressedLastFrame)
    {
        currentScene = (currentScene == SCENE_A) ? SCENE_B : SCENE_A;
        enterPressedLastFrame = true;
    }

    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE)
    {
        enterPressedLastFrame = false;
    }

    // Rotate scene by 90 degrees when R is pressed
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && !rPressedLastFrame)
    {
        rotationAngle += 90.0f;
        if (rotationAngle >= 360.0f)
            rotationAngle = 0.0f;

        rPressedLastFrame = true;
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE)
    {
        rPressedLastFrame = false;
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

void renderSceneA(GLuint cubeVAO, Shader& lightingShader, glm::mat4& view, glm::mat4& projection, glm::vec3 cubePositions[])
{
    glBindVertexArray(cubeVAO);

    for (unsigned int i = 0; i < 6; i++)   // 0 → 5
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);

        lightingShader.setMat4("model", model);

        // Unique color per cube
        glm::vec3 color;
        color.r = (i % 3) / 2.0f;
        color.g = ((i + 1) % 3) / 2.0f;
        color.b = ((i + 2) % 3) / 2.0f;

        lightingShader.setVec3("material.ambient", color * 0.2f);
        lightingShader.setVec3("material.diffuse", color);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

void renderSceneB(GLuint cubeVAO, Shader& lightingShader, glm::mat4& view, glm::mat4& projection, glm::vec3 cubePositions[])
{
    glBindVertexArray(cubeVAO);

    for (unsigned int i = 6; i < 10; i++)  // 6 → 9
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        model = glm::scale(model, glm::vec3(0.5f));

        lightingShader.setMat4("model", model);

        // Unique color per cube
        glm::vec3 color;
        color.r = (i % 3) / 2.0f;
        color.g = ((i + 1) % 3) / 2.0f;
        color.b = ((i + 2) % 3) / 2.0f;

        lightingShader.setVec3("material.ambient", color * 0.2f);
        lightingShader.setVec3("material.diffuse", color);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

