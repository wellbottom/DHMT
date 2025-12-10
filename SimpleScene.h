#ifndef SIMPLE_SCENE_H
#define SIMPLE_SCENE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "camera.h"

class SimpleScene
{
public:
    virtual ~SimpleScene() {}

    // Initialize scene
    virtual void Init() = 0;

    // Update scene
    virtual void Update(float deltaTime) = 0;

    // Render scene
    virtual void Render(Camera& camera, int width, int height) = 0;

    // Process input
    virtual void ProcessInput(GLFWwindow* window, Camera& camera, float deltaTime) = 0;

    // Cleanup
    virtual void Cleanup() = 0;
};

#endif