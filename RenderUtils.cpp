#include "RenderUtils.h"
#include "mesh.h"
#include <glm/gtc/matrix_transform.hpp>

void RenderUtils::renderCube(
    GLuint cubeVAO,
    Shader& shader,
    const glm::vec3& position,
    const glm::vec3& scale,
    const glm::vec3& ambient,
    const glm::vec3& diffuse,
    const glm::vec3& specular,
    float alpha,
    float rotationDegrees,
    const glm::vec3& rotationAxis
) {
    shader.setVec3("material.ambient", ambient);
    shader.setVec3("material.diffuse", diffuse);
    shader.setVec3("material.specular", specular);
    shader.setFloat("material.alpha", alpha);

    glBindVertexArray(cubeVAO);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    if (rotationDegrees != 0.0f) {
        model = glm::rotate(model, glm::radians(rotationDegrees), rotationAxis);
    }
    model = glm::scale(model, scale);
    shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, Mesh::GetVertexCount(Mesh::CUBE));
}

void RenderUtils::renderCubeWithMatrix(
    GLuint cubeVAO,
    Shader& shader,
    const glm::mat4& transformMatrix,
    const glm::vec3& scale,
    const glm::vec3& ambient,
    const glm::vec3& diffuse,
    const glm::vec3& specular,
    float alpha
) {
    shader.setVec3("material.ambient", ambient);
    shader.setVec3("material.diffuse", diffuse);
    shader.setVec3("material.specular", specular);
    shader.setFloat("material.alpha", alpha);

    glBindVertexArray(cubeVAO);
    glm::mat4 model = transformMatrix;
    model = glm::scale(model, scale);
    shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, Mesh::GetVertexCount(Mesh::CUBE));
}

void RenderUtils::renderPlane(
    GLuint planeVAO,
    Shader& shader,
    const glm::vec3& position,
    const glm::vec3& scale,
    const glm::vec3& ambient,
    const glm::vec3& diffuse,
    const glm::vec3& specular,
    float alpha,
    float rotationDegrees,
    const glm::vec3& rotationAxis
) {
    shader.setVec3("material.ambient", ambient);
    shader.setVec3("material.diffuse", diffuse);
    shader.setVec3("material.specular", specular);
    shader.setFloat("material.alpha", alpha);

    glBindVertexArray(planeVAO);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    if (rotationDegrees != 0.0f) {
        model = glm::rotate(model, glm::radians(rotationDegrees), rotationAxis);
    }
    model = glm::scale(model, scale);
    shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, Mesh::GetVertexCount(Mesh::PLANE));
}

void RenderUtils::renderCylinder(
    GLuint cylinderVAO,
    Shader& shader,
    const glm::vec3& position,
    const glm::vec3& scale,
    const glm::vec3& ambient,
    const glm::vec3& diffuse,
    const glm::vec3& specular,
    float alpha,
    float rotationDegrees,
    const glm::vec3& rotationAxis
) {
    shader.setVec3("material.ambient", ambient);
    shader.setVec3("material.diffuse", diffuse);
    shader.setVec3("material.specular", specular);
    shader.setFloat("material.alpha", alpha);

    glBindVertexArray(cylinderVAO);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    if (rotationDegrees != 0.0f) {
        model = glm::rotate(model, glm::radians(rotationDegrees), rotationAxis);
    }
    model = glm::scale(model, scale);
    shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, Mesh::GetVertexCount(Mesh::CYLINDER));
}

void RenderUtils::renderCylinderWithMatrix(
    GLuint cylinderVAO,
    Shader& shader,
    const glm::mat4& transformMatrix,
    const glm::vec3& scale,
    const glm::vec3& ambient,
    const glm::vec3& diffuse,
    const glm::vec3& specular,
    float alpha
) {
    shader.setVec3("material.ambient", ambient);
    shader.setVec3("material.diffuse", diffuse);
    shader.setVec3("material.specular", specular);
    shader.setFloat("material.alpha", alpha);

    glBindVertexArray(cylinderVAO);
    glm::mat4 model = transformMatrix;
    model = glm::scale(model, scale);
    shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, Mesh::GetVertexCount(Mesh::CYLINDER));
}

void RenderUtils::renderWindow(
    GLuint windowVAO,
    Shader& shader,
    const glm::vec3& position,
    const glm::vec3& scale,
    const glm::vec3& ambient,
    const glm::vec3& diffuse,
    const glm::vec3& specular,
    float alpha,
    float rotationDegrees,
    const glm::vec3& rotationAxis
) {
    shader.setVec3("material.ambient", ambient);
    shader.setVec3("material.diffuse", diffuse);
    shader.setVec3("material.specular", specular);
    shader.setFloat("material.alpha", alpha);

    glBindVertexArray(windowVAO);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    if (rotationDegrees != 0.0f) {
        model = glm::rotate(model, glm::radians(rotationDegrees), rotationAxis);
    }
    model = glm::scale(model, scale);
    shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, Mesh::GetVertexCount(Mesh::WINDOW));
}

void RenderUtils::renderSphere(
    GLuint sphereVAO,
    Shader& shader,
    const glm::vec3& position,
    const glm::vec3& scale,
    const glm::vec3& ambient,
    const glm::vec3& diffuse,
    const glm::vec3& specular,
    float alpha
) {
    shader.setVec3("material.ambient", ambient);
    shader.setVec3("material.diffuse", diffuse);
    shader.setVec3("material.specular", specular);
    shader.setFloat("material.alpha", alpha);

    glBindVertexArray(sphereVAO);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, scale);
    shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, Mesh::GetVertexCount(Mesh::SPHERE));
}