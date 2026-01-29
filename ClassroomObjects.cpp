#include "ClassroomObjects.h"
#include "RenderUtils.h"
#include "SceneConfig.h"
#include "mesh.h"
#include <glm/gtc/matrix_transform.hpp>

void ClassroomObjects::renderClassroomStructure(
    GLuint cubeVAO,
    GLuint planeVAO,
    GLuint windowVAO,
    Shader& shader,
    glm::mat4& view,
    glm::mat4& projection
) {
    using namespace ClassroomConfig;
    using namespace WindowDimensions;
    using namespace DoorConfig;

    shader.use();

    // 1. Render the floor
    RenderUtils::renderPlane(
        planeVAO, shader,
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(ClassroomConfig::WIDTH, 1.0f, DEPTH),
        Colors::FLOOR_AMBIENT, Colors::FLOOR_DIFFUSE, Colors::FLOOR_SPECULAR
    );

    //1.1 Render additional floor outside classroom
    // 
    // Additional grey floor next to the green floor
    const glm::vec3 GREY_FLOOR_AMBIENT(0.3f, 0.3f, 0.3f);
    const glm::vec3 GREY_FLOOR_DIFFUSE(0.5f, 0.5f, 0.5f);
    const glm::vec3 GREY_FLOOR_SPECULAR(0.2f, 0.2f, 0.2f);

    RenderUtils::renderPlane(
        planeVAO, shader,
        glm::vec3(ClassroomConfig::WIDTH, 0.0f, 0.0f),  // Position it adjacent to the current floor
        glm::vec3(ClassroomConfig::WIDTH, 1.0f, DEPTH),
        GREY_FLOOR_AMBIENT, GREY_FLOOR_DIFFUSE, GREY_FLOOR_SPECULAR
    );

    //Additional grey floor in front of the classroom
    RenderUtils::renderPlane(
        planeVAO, shader,
        glm::vec3(0.0f, 0.0f, -DEPTH),  // Position it in front of the current floor
        glm::vec3(ClassroomConfig::WIDTH * 3, 1.0f, DEPTH),
        GREY_FLOOR_AMBIENT, GREY_FLOOR_DIFFUSE, GREY_FLOOR_SPECULAR
    );

    //Additional grey floor behind the classroom
    RenderUtils::renderPlane(
        planeVAO, shader,
        glm::vec3(0.0f, 0.0f, DEPTH),  // Position it behind the current floor
        glm::vec3(ClassroomConfig::WIDTH * 3, 1.0f, DEPTH),
        GREY_FLOOR_AMBIENT, GREY_FLOOR_DIFFUSE, GREY_FLOOR_SPECULAR
    );


    //Additional grey floor to the left of the classroom
    RenderUtils::renderPlane(
        planeVAO, shader,
        glm::vec3(-ClassroomConfig::WIDTH, 0.0f, 0.0f),  // Position it to the left of the current floor
        glm::vec3(ClassroomConfig::WIDTH, 1.0f, DEPTH),
        GREY_FLOOR_AMBIENT, GREY_FLOOR_DIFFUSE, GREY_FLOOR_SPECULAR
    );

    // 2. Render ceiling
    RenderUtils::renderPlane(
        planeVAO, shader,
        glm::vec3(0.0f, ClassroomConfig::HEIGHT, 0.0f),
        glm::vec3(ClassroomConfig::WIDTH, 1.0f, DEPTH),
        Colors::CEILING_AMBIENT, Colors::CEILING_DIFFUSE, Colors::CEILING_SPECULAR,
        1.0f, 180.0f
    );

    // Wall rendering
    float frontZ = DEPTH / 2.0f;
    float leftX = -ClassroomConfig::WIDTH / 2.0f;
    float rightX = ClassroomConfig::WIDTH / 2.0f;

    // Front wall (solid)
    RenderUtils::renderCube(
        cubeVAO, shader,
        glm::vec3(0.0f, ClassroomConfig::HEIGHT / 2.0f, frontZ),
        glm::vec3(ClassroomConfig::WIDTH, ClassroomConfig::HEIGHT, WALL_THICKNESS),
        Colors::WALL_AMBIENT, Colors::WALL_DIFFUSE, Colors::WALL_SPECULAR
    );

    // Back wall (solid)
    RenderUtils::renderCube(
        cubeVAO, shader,
        glm::vec3(0.0f, ClassroomConfig::HEIGHT / 2.0f, -frontZ),
        glm::vec3(ClassroomConfig::WIDTH, ClassroomConfig::HEIGHT, WALL_THICKNESS),
        Colors::WALL_AMBIENT, Colors::WALL_DIFFUSE, Colors::WALL_SPECULAR
    );

    // Left wall sections (with window and door cutouts)
    float centerWindowZ = 0.0f;
    float windowFront = centerWindowZ + WindowDimensions::WIDTH / 2.0f;
    float windowBack = centerWindowZ - WindowDimensions::WIDTH / 2.0f;
    float doorFront = DoorConfig::Z_POSITION + DoorConfig::WIDTH / 2.0f;
    float doorBack = DoorConfig::Z_POSITION - DoorConfig::WIDTH / 2.0f;

    // Left wall - front section
    {
        float wallDepth = (DEPTH / 2.0f) - windowFront;
        float wallCenterZ = windowFront + wallDepth / 2.0f;
        RenderUtils::renderCube(
            cubeVAO, shader,
            glm::vec3(leftX, ClassroomConfig::HEIGHT / 2.0f, wallCenterZ),
            glm::vec3(WALL_THICKNESS, ClassroomConfig::HEIGHT, wallDepth),
            Colors::WALL_AMBIENT, Colors::WALL_DIFFUSE, Colors::WALL_SPECULAR
        );
    }

    // Left wall - below window
    RenderUtils::renderCube(
        cubeVAO, shader,
        glm::vec3(leftX, (WindowDimensions::Y_POSITION - WindowDimensions::HEIGHT / 2.0f) / 2.0f, centerWindowZ),
        glm::vec3(WALL_THICKNESS, WindowDimensions::Y_POSITION - WindowDimensions::HEIGHT / 2.0f, WindowDimensions::WIDTH),
        Colors::WALL_AMBIENT, Colors::WALL_DIFFUSE, Colors::WALL_SPECULAR
    );

    // Left wall - above window
    {
        float topY = WindowDimensions::Y_POSITION + WindowDimensions::HEIGHT / 2.0f;
        RenderUtils::renderCube(
            cubeVAO, shader,
            glm::vec3(leftX, topY + (ClassroomConfig::HEIGHT - topY) / 2.0f, centerWindowZ),
            glm::vec3(WALL_THICKNESS, ClassroomConfig::HEIGHT - topY, WindowDimensions::WIDTH),
            Colors::WALL_AMBIENT, Colors::WALL_DIFFUSE, Colors::WALL_SPECULAR
        );
    }

    // Left wall - middle section (between window and door)
    {
        float wallDepth = doorFront - windowBack;
        float wallCenterZ = windowBack + wallDepth / 2.0f;
        RenderUtils::renderCube(
            cubeVAO, shader,
            glm::vec3(leftX, ClassroomConfig::HEIGHT / 2.0f, wallCenterZ),
            glm::vec3(WALL_THICKNESS, ClassroomConfig::HEIGHT, wallDepth),
            Colors::WALL_AMBIENT, Colors::WALL_DIFFUSE, Colors::WALL_SPECULAR
        );
    }

    // Left wall - above door
    {
        float topY = DoorConfig::HEIGHT;
        RenderUtils::renderCube(
            cubeVAO, shader,
            glm::vec3(leftX, topY + (ClassroomConfig::HEIGHT - topY) / 2.0f, DoorConfig::Z_POSITION),
            glm::vec3(WALL_THICKNESS, ClassroomConfig::HEIGHT - topY, DoorConfig::WIDTH),
            Colors::WALL_AMBIENT, Colors::WALL_DIFFUSE, Colors::WALL_SPECULAR
        );
    }

    // Left wall - back section
    {
        float wallDepth = doorBack - (-DEPTH / 2.0f);
        float wallCenterZ = -DEPTH / 2.0f + wallDepth / 2.0f;
        RenderUtils::renderCube(
            cubeVAO, shader,
            glm::vec3(leftX, ClassroomConfig::HEIGHT / 2.0f, wallCenterZ),
            glm::vec3(WALL_THICKNESS, ClassroomConfig::HEIGHT, wallDepth),
            Colors::WALL_AMBIENT, Colors::WALL_DIFFUSE, Colors::WALL_SPECULAR
        );
    }

    // Right wall sections (with window cutout)
    // Right wall - front section
    {
        float wallDepth = (DEPTH / 2.0f) - windowFront;
        float wallCenterZ = windowFront + wallDepth / 2.0f;
        RenderUtils::renderCube(
            cubeVAO, shader,
            glm::vec3(rightX, ClassroomConfig::HEIGHT / 2.0f, wallCenterZ),
            glm::vec3(WALL_THICKNESS, ClassroomConfig::HEIGHT, wallDepth),
            Colors::WALL_AMBIENT, Colors::WALL_DIFFUSE, Colors::WALL_SPECULAR
        );
    }

    // Right wall - below window
    RenderUtils::renderCube(
        cubeVAO, shader,
        glm::vec3(rightX, (WindowDimensions::Y_POSITION - WindowDimensions::HEIGHT / 2.0f) / 2.0f, centerWindowZ),
        glm::vec3(WALL_THICKNESS, WindowDimensions::Y_POSITION - WindowDimensions::HEIGHT / 2.0f, WindowDimensions::WIDTH),
        Colors::WALL_AMBIENT, Colors::WALL_DIFFUSE, Colors::WALL_SPECULAR
    );

    // Right wall - above window
    {
        float topY = WindowDimensions::Y_POSITION + WindowDimensions::HEIGHT / 2.0f;
        RenderUtils::renderCube(
            cubeVAO, shader,
            glm::vec3(rightX, topY + (ClassroomConfig::HEIGHT - topY) / 2.0f, centerWindowZ),
            glm::vec3(WALL_THICKNESS, ClassroomConfig::HEIGHT - topY, WindowDimensions::WIDTH),
            Colors::WALL_AMBIENT, Colors::WALL_DIFFUSE, Colors::WALL_SPECULAR
        );
    }

    // Right wall - back section
    {
        float wallDepth = windowBack - (-DEPTH / 2.0f);
        float wallCenterZ = -DEPTH / 2.0f + wallDepth / 2.0f;
        RenderUtils::renderCube(
            cubeVAO, shader,
            glm::vec3(rightX, ClassroomConfig::HEIGHT / 2.0f, wallCenterZ),
            glm::vec3(WALL_THICKNESS, ClassroomConfig::HEIGHT, wallDepth),
            Colors::WALL_AMBIENT, Colors::WALL_DIFFUSE, Colors::WALL_SPECULAR
        );
    }

    // Render windows with transparency
    glDepthMask(GL_FALSE);

    // Left wall window
    RenderUtils::renderWindow(
        windowVAO, shader,
        glm::vec3(leftX - 0.01f, WindowDimensions::Y_POSITION, 0.0f),
        glm::vec3(WindowDimensions::WIDTH, WindowDimensions::HEIGHT, 1.0f),
        Colors::WINDOW_AMBIENT, Colors::WINDOW_DIFFUSE, Colors::WINDOW_SPECULAR,
        0.3f, 90.0f
    );

    // Right wall window
    RenderUtils::renderWindow(
        windowVAO, shader,
        glm::vec3(rightX + 0.01f, WindowDimensions::Y_POSITION, 0.0f),
        glm::vec3(WindowDimensions::WIDTH, WindowDimensions::HEIGHT, 1.0f),
        Colors::WINDOW_AMBIENT, Colors::WINDOW_DIFFUSE, Colors::WINDOW_SPECULAR,
        0.3f, -90.0f
    );

    glDepthMask(GL_TRUE);
}

void ClassroomObjects::renderDesk(
    GLuint cubeVAO,
    GLuint planeVAO,
    Shader& shader,
    glm::mat4& view,
    glm::mat4& projection,
    glm::vec3 position
) {
    using namespace DeskDimensions;

    shader.use();
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);

    // Main surface
    RenderUtils::renderCube(
        cubeVAO, shader,
        position + glm::vec3(0.0f, HEIGHT - MAIN_THICKNESS / 2.0f, 0.0f),
        glm::vec3(MAIN_WIDTH, MAIN_THICKNESS, MAIN_DEPTH),
        Colors::WOOD_AMBIENT, Colors::WOOD_DIFFUSE, Colors::WOOD_SPECULAR
    );

    // Sub-surface
    float subSurfaceY = HEIGHT - MAIN_THICKNESS - GAP - SUB_THICKNESS / 2.0f;
    float subSurfaceZOffset = -(MAIN_DEPTH - SUB_DEPTH) / 2.0f;
    RenderUtils::renderCube(
        cubeVAO, shader,
        position + glm::vec3(0.0f, subSurfaceY, -subSurfaceZOffset),
        glm::vec3(SUB_WIDTH, SUB_THICKNESS, SUB_DEPTH),
        Colors::WOOD_AMBIENT, Colors::WOOD_DIFFUSE, Colors::WOOD_SPECULAR
    );

    // Panels
    float frontPanelY = (HEIGHT - MAIN_THICKNESS + subSurfaceY - SUB_THICKNESS / 2.0f) / 2.0f;
    float frontPanelHeight_calc = (HEIGHT - MAIN_THICKNESS) - (subSurfaceY - SUB_THICKNESS / 2.0f);

    // Front panel
    RenderUtils::renderCube(
        cubeVAO, shader,
        position + glm::vec3(0.0f, frontPanelY, MAIN_DEPTH / 2.0f - PANEL_THICKNESS / 2.0f),
        glm::vec3(MAIN_WIDTH, frontPanelHeight_calc, PANEL_THICKNESS),
        Colors::WOOD_AMBIENT, Colors::WOOD_DIFFUSE, Colors::WOOD_SPECULAR
    );

    // Left panel
    RenderUtils::renderCube(
        cubeVAO, shader,
        position + glm::vec3(-MAIN_WIDTH / 2.0f + PANEL_THICKNESS / 2.0f, frontPanelY, 0.0f),
        glm::vec3(PANEL_THICKNESS, frontPanelHeight_calc, MAIN_DEPTH),
        Colors::WOOD_AMBIENT, Colors::WOOD_DIFFUSE, Colors::WOOD_SPECULAR
    );

    // Right panel
    RenderUtils::renderCube(
        cubeVAO, shader,
        position + glm::vec3(MAIN_WIDTH / 2.0f - PANEL_THICKNESS / 2.0f, frontPanelY, 0.0f),
        glm::vec3(PANEL_THICKNESS, frontPanelHeight_calc, MAIN_DEPTH),
        Colors::WOOD_AMBIENT, Colors::WOOD_DIFFUSE, Colors::WOOD_SPECULAR
    );

    // Four legs
    float legOffsetX = MAIN_WIDTH / 2.0f - LEG_WIDTH / 2.0f;
    float frontlegOffsetZ = MAIN_DEPTH / 2.0f - LEG_WIDTH / 2.0f;
    float backlegOffsetZ = SUB_DEPTH / 2.0f - LEG_WIDTH * 4.0f;
    float legHeight = subSurfaceY - SUB_THICKNESS / 2.0f;

    // Leg positions
    glm::vec3 legPositions[4] = {
        position + glm::vec3(-legOffsetX, legHeight / 2.0f, frontlegOffsetZ),
        position + glm::vec3(legOffsetX, legHeight / 2.0f, frontlegOffsetZ),
        position + glm::vec3(-legOffsetX, legHeight / 2.0f, -backlegOffsetZ),
        position + glm::vec3(legOffsetX, legHeight / 2.0f, -backlegOffsetZ)
    };

    for (int i = 0; i < 4; i++) {
        RenderUtils::renderCube(
            cubeVAO, shader,
            legPositions[i],
            glm::vec3(LEG_WIDTH, legHeight, LEG_WIDTH),
            Colors::WOOD_AMBIENT, Colors::WOOD_DIFFUSE, Colors::WOOD_SPECULAR
        );
    }
}

void ClassroomObjects::renderBench(
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
) {
    shader.use();
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);

    // Bench seat
    RenderUtils::renderCube(
        cubeVAO, shader,
        position + glm::vec3(0.0f, benchHeight, 0.0f),
        glm::vec3(benchWidth, 0.1f, benchDepth),
        Colors::BENCH_AMBIENT, Colors::BENCH_DIFFUSE, Colors::BENCH_SPECULAR
    );

    // Four legs
    float xOffset = benchWidth / 2.0f - legWidth / 2.0f;
    float zOffset = benchDepth / 2.0f - legWidth / 2.0f;
    float yOffset = legHeight / 2.0f;

    for (int sx = -1; sx <= 1; sx += 2) {
        for (int sz = -1; sz <= 1; sz += 2) {
            RenderUtils::renderCube(
                cubeVAO, shader,
                position + glm::vec3(sx * xOffset, yOffset, sz * zOffset),
                glm::vec3(legWidth, legHeight, legWidth),
                Colors::BENCH_AMBIENT, Colors::BENCH_DIFFUSE, Colors::BENCH_SPECULAR
            );
        }
    }
}

void ClassroomObjects::renderCeilingFan(
    GLuint cubeVAO,
    GLuint cylinderVAO,
    Shader& shader,
    glm::mat4& view,
    glm::mat4& projection,
    float currentTime,
    bool fanOn
) {
    using namespace FanConfig;
    using namespace ClassroomConfig;

    shader.use();
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);

    float fanSpeed = fanOn ? SPEED : 0.0f;
    float rotation = currentTime * fanSpeed * 360.0f;

    // Ceiling rod
    RenderUtils::renderCylinder(
        cylinderVAO, shader,
        glm::vec3(0.0f, (HEIGHT + POSITION.y) / 2.0f, 0.0f),
        glm::vec3(ProjectorConfig::MOUNT_RADIUS, ClassroomConfig::HEIGHT - POSITION.y, ProjectorConfig::MOUNT_RADIUS),
        Colors::METAL_DARK_AMBIENT, Colors::METAL_DARK_DIFFUSE, Colors::METAL_DARK_SPECULAR
    );

    // Central disk (rotates)
    glm::mat4 diskTransform = glm::mat4(1.0f);
    diskTransform = glm::translate(diskTransform, POSITION);
    diskTransform = glm::rotate(diskTransform, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));

    RenderUtils::renderCylinderWithMatrix(
        cylinderVAO, shader,
        diskTransform,
        glm::vec3(DISK_RADIUS, DISK_HEIGHT, DISK_RADIUS),
        Colors::METAL_LIGHT_AMBIENT, Colors::METAL_LIGHT_DIFFUSE, Colors::METAL_LIGHT_SPECULAR
    );

    // Blades and connectors - each at different angle
    for (int i = 0; i < NUM_BLADES; i++)
    {
        float angle = rotation + (i * 360.0f / NUM_BLADES);

        // Connector stick - rotates with the fan
        glm::mat4 connectorTransform = glm::mat4(1.0f);
        connectorTransform = glm::translate(connectorTransform, POSITION);
        connectorTransform = glm::rotate(connectorTransform, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
        connectorTransform = glm::translate(connectorTransform, glm::vec3(1.2f, 0.0f, 0.0f));
        connectorTransform = glm::rotate(connectorTransform, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

        RenderUtils::renderCylinderWithMatrix(
            cylinderVAO, shader,
            connectorTransform,
            glm::vec3(0.1f, 0.5f, 0.1f),
            glm::vec3(0.2f, 0.2f, 0.2f),
            glm::vec3(0.35f, 0.35f, 0.35f),
            glm::vec3(0.5f, 0.5f, 0.5f)
        );

        // Blade - rotates with the fan around Y-axis
        glm::mat4 bladeTransform = glm::mat4(1.0f);
        bladeTransform = glm::translate(bladeTransform, POSITION);
        bladeTransform = glm::rotate(bladeTransform, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
        bladeTransform = glm::translate(bladeTransform, glm::vec3(2.5f, 0.0f, 0.0f));

        RenderUtils::renderCubeWithMatrix(
            cubeVAO, shader,
            bladeTransform,
            glm::vec3(BLADE_LENGTH, BLADE_THICKNESS, BLADE_WIDTH),
            Colors::METAL_LIGHT_AMBIENT, Colors::METAL_LIGHT_DIFFUSE, Colors::METAL_LIGHT_SPECULAR
        );
    }
}

void ClassroomObjects::renderCeilingLights(
    GLuint cubeVAO,
    Shader& lightCubeShader,
    glm::mat4& view,
    glm::mat4& projection,
    const glm::vec3 lightPositions[4],
    bool lightsOn
) {
    lightCubeShader.use();
    lightCubeShader.setMat4("projection", projection);
    lightCubeShader.setMat4("view", view);

    // Set light color based on state
    glm::vec3 lightColor = lightsOn ? glm::vec3(1.0f, 1.0f, 1.0f) : glm::vec3(0.15f, 0.15f, 0.15f);
    lightCubeShader.setVec3("lightColor", lightColor);

    glBindVertexArray(cubeVAO);

    // Render 4 elongated light fixtures
    for (int i = 0; i < 4; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, lightPositions[i]);
        model = glm::scale(model, glm::vec3(6.0f, 0.2f, 0.5f));
        lightCubeShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, Mesh::GetVertexCount(Mesh::CUBE));
    }
}

void ClassroomObjects::renderFramedPanel(
    GLuint cubeVAO,
    Shader& shader,
    const glm::mat4& view,
    const glm::mat4& projection,
    const glm::vec3& position,
    const PanelStyle& style,
    float rotationDegrees,
    bool canRotate,
    bool isOpen
) {
    shader.use();
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);

    float w = style.width;
    float h = style.height;
    float f = style.frameThickness;
    float z = style.thickness + 0.1f;

    // Build base transformation matrix for the panel
    glm::mat4 baseTransform = glm::mat4(1.0f);
    baseTransform = glm::translate(baseTransform, position);

    // If this can rotate (door), rotate around the right edge
    if (canRotate && isOpen) {
        // Move pivot to right edge
        baseTransform = glm::translate(baseTransform, glm::vec3(w / 2.0f, 0.0f, 0.0f));
        // Rotate 90 degrees clockwise (negative Y-axis rotation)
        baseTransform = glm::rotate(baseTransform, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        // Move back
        baseTransform = glm::translate(baseTransform, glm::vec3(-w / 2.0f, 0.0f, 0.0f));
    }

    // Apply initial rotation (for wall orientation)
    baseTransform = glm::rotate(baseTransform, glm::radians(rotationDegrees), glm::vec3(0.0f, 1.0f, 0.0f));

    // Main surface
    RenderUtils::renderCubeWithMatrix(
        cubeVAO, shader,
        baseTransform,
        glm::vec3(style.width, style.height, style.thickness),
        style.surfaceAmbient, style.surfaceDiffuse, style.surfaceSpecular
    );

    // Frame pieces - all relative to the base transform
    auto drawFramePiece = [&](glm::vec3 offset, glm::vec3 scale) {
        glm::mat4 m = baseTransform;
        m = glm::translate(m, offset);

        RenderUtils::renderCubeWithMatrix(
            cubeVAO, shader,
            m,
            scale,
            style.frameAmbient, style.frameDiffuse, style.frameSpecular
        );
        };

    // Top / Bottom frame - these need to rotate with the panel
    drawFramePiece(glm::vec3(0.0f, h / 2 + f / 2, 0.0f), glm::vec3(w + 2 * f, f, z));
    drawFramePiece(glm::vec3(0.0f, -h / 2 - f / 2, 0.0f), glm::vec3(w + 2 * f, f, z));

    // Left / Right frame
    drawFramePiece(glm::vec3(-w / 2 - f / 2, 0.0f, 0.0f), glm::vec3(f, h, z));
    drawFramePiece(glm::vec3(w / 2 + f / 2, 0.0f, 0.0f), glm::vec3(f, h, z));

    // Optional chalk tray
    if (style.hasTray)
    {
        glm::mat4 tray = baseTransform;
        tray = glm::translate(tray, glm::vec3(0.0f, -h / 2 - f - style.trayOffset, 0.1f));

        RenderUtils::renderCubeWithMatrix(
            cubeVAO, shader,
            tray,
            glm::vec3(w, style.trayHeight, style.trayDepth),
            style.frameAmbient, style.frameDiffuse, style.frameSpecular
        );
    }
}

void ClassroomObjects::renderProjector(
    GLuint cubeVAO,
    GLuint cylinderVAO,
    Shader& shader,
    glm::mat4& view,
    glm::mat4& projection,
    glm::vec3 position
) {
    using namespace ProjectorConfig;

    shader.use();
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);

    // Ceiling mount pipe
    RenderUtils::renderCylinder(
        cylinderVAO, shader,
        position + glm::vec3(0.0f, 1.5f, 0.0f),
        glm::vec3(MOUNT_RADIUS, MOUNT_LENGTH, MOUNT_RADIUS),
        Colors::METAL_DARK_AMBIENT, Colors::METAL_DARK_DIFFUSE, Colors::METAL_DARK_SPECULAR
    );

    // Main projector body
    RenderUtils::renderCube(
        cubeVAO, shader,
        position,
        glm::vec3(BODY_WIDTH, BODY_HEIGHT, BODY_DEPTH),
        Colors::METAL_DARK_AMBIENT, Colors::METAL_DARK_DIFFUSE, Colors::METAL_DARK_SPECULAR
    );

    // Projector lens
    RenderUtils::renderCylinder(
        cylinderVAO, shader,
        position - glm::vec3(0.0f, 0.0f, -0.5f),
        glm::vec3(LENS_RADIUS, LENS_LENGTH, LENS_RADIUS),
        Colors::LENS_AMBIENT, Colors::LENS_DIFFUSE, Colors::LENS_SPECULAR,
        1.0f, 90.0f, glm::vec3(1.0f, 0.0f, 0.0f)
    );
}// Add this function to ClassroomObjects.cpp at the end

void ClassroomObjects::renderPoster(
    GLuint planeVAO,
    Shader& shader,
    const glm::mat4& view,
    const glm::mat4& projection,
    const glm::vec3& position,
    const glm::vec3& scale,
    GLuint textureID,
    float rotationDegrees,
    const glm::vec3& rotationAxis
) {
    shader.use();
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);

    // Enable texturing
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    shader.setInt("material.diffuse", 0);
    shader.setInt("material.specular", 0);
    shader.setFloat("material.shininess", 16.0f);

    // Build transformation matrix
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);

    // Apply rotation
    if (rotationDegrees != 0.0f) {
        model = glm::rotate(model, glm::radians(rotationDegrees), rotationAxis);
    }

    // Apply scale
    model = glm::scale(model, scale);

    shader.setMat4("model", model);

    // Render the plane
    glBindVertexArray(planeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);  // Plane has 6 vertices (2 triangles)
}