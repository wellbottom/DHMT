#include "Mesh.h"
#include <cmath>
#include<glm/glm.hpp>

static std::vector<float> cubeVertices = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

static std::vector<float> planeVertices = {
     0.5f, 0.0f,  0.5f,   0,1,0,   1,1,
    -0.5f, 0.0f,  0.5f,   0,1,0,   0,1,
    -0.5f, 0.0f, -0.5f,   0,1,0,   0,0,

     0.5f, 0.0f,  0.5f,   0,1,0,   1,1,
    -0.5f, 0.0f, -0.5f,   0,1,0,   0,0,
     0.5f, 0.0f, -0.5f,   0,1,0,   1,0
};

static std::vector<float> tetrahedronVertices = {
    // positions          // normals           // texcoords
     0.0f,  0.577f,  0.0f,   0,1,0,  0.5f,1.0f,
    -0.5f, -0.289f,  0.5f,   0,1,0,  0,0,
     0.5f, -0.289f,  0.5f,   0,1,0,  1,0,

     0.0f,  0.577f,  0.0f,   0,0,1,  0.5f,1.0f,
     0.5f, -0.289f,  0.5f,   0,0,1,  1,0,
     0.0f, -0.289f, -0.5f,   0,0,1,  0,0,

     0.0f,  0.577f,  0.0f,   1,0,0,  0.5f,1.0f,
     0.0f, -0.289f, -0.5f,   1,0,0,  0,0,
    -0.5f, -0.289f,  0.5f,   1,0,0,  1,0,

    -0.5f, -0.289f,  0.5f,   0,-1,0,  0,1,
     0.0f, -0.289f, -0.5f,   0,-1,0,  0.5f,0,
     0.5f, -0.289f,  0.5f,   0,-1,0,  1,1
};


static std::vector<float> pentahedronVertices = {
    // Front triangle
     0.0f,  0.5f,  0.5f,   0,0,1,  0.5f,1,
    -0.5f, -0.5f,  0.5f,   0,0,1,  0,0,
     0.5f, -0.5f,  0.5f,   0,0,1,  1,0,

     // Back triangle
      0.0f,  0.5f, -0.5f,   0,0,-1, 0.5f,1,
      0.5f, -0.5f, -0.5f,   0,0,-1, 1,0,
     -0.5f, -0.5f, -0.5f,   0,0,-1, 0,0,

     // Side rectangles
     -0.5f, -0.5f, -0.5f,  -1,0,0, 0,0,
     -0.5f, -0.5f,  0.5f,  -1,0,0, 1,0,
     -0.5f,  0.5f,  0.0f,  -1,0,0, 0.5f,1,

      0.5f, -0.5f,  0.5f,   1,0,0, 0,0,
      0.5f, -0.5f, -0.5f,   1,0,0, 1,0,
      0.5f,  0.5f,  0.0f,   1,0,0, 0.5f,1
};

// Window geometry - a frame with 4 rectangular panes (2x2 grid)
static std::vector<float> windowVertices;
static std::vector<float> sphereVertices;
static std::vector<float> cylinderVertices;
static std::vector<float> paraboloidVertices;

static void generateWindow()
{
    windowVertices.clear();

    // Window dimensions (in XY plane, facing +Z)
    float width = 1.0f;   // Total width
    float height = 1.0f;  // Total height
    float depth = 0.02f;  // Frame thickness

    // Frame dimensions
    float frameWidth = 0.08f;  // Thickness of the frame bars

    // Pane divisions (2x2 grid)
    float midX = 0.0f;  // Middle vertical bar position
    float midY = 0.0f;  // Middle horizontal bar position

    auto addQuad = [](glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4, glm::vec3 normal,
        float u1, float v1, float u2, float v2, float u3, float v3, float u4, float v4)
        {
            // Triangle 1
            windowVertices.insert(windowVertices.end(), { p1.x, p1.y, p1.z, normal.x, normal.y, normal.z, u1, v1 });
            windowVertices.insert(windowVertices.end(), { p2.x, p2.y, p2.z, normal.x, normal.y, normal.z, u2, v2 });
            windowVertices.insert(windowVertices.end(), { p3.x, p3.y, p3.z, normal.x, normal.y, normal.z, u3, v3 });

            // Triangle 2
            windowVertices.insert(windowVertices.end(), { p1.x, p1.y, p1.z, normal.x, normal.y, normal.z, u1, v1 });
            windowVertices.insert(windowVertices.end(), { p3.x, p3.y, p3.z, normal.x, normal.y, normal.z, u3, v3 });
            windowVertices.insert(windowVertices.end(), { p4.x, p4.y, p4.z, normal.x, normal.y, normal.z, u4, v4 });
        };

    glm::vec3 normalFront(0, 0, 1);
    glm::vec3 normalBack(0, 0, -1);

    // Create the window frame as multiple rectangles
    float halfW = width * 0.5f;
    float halfH = height * 0.5f;
    float halfFW = frameWidth * 0.5f;
    float halfD = depth * 0.5f;

    // OUTER FRAME - 4 borders (FRONT SIDE)
    // Top border
    addQuad(
        glm::vec3(-halfW, halfH - frameWidth, halfD),
        glm::vec3(halfW, halfH - frameWidth, halfD),
        glm::vec3(halfW, halfH, halfD),
        glm::vec3(-halfW, halfH, halfD),
        normalFront, 0, 0, 1, 0, 1, 1, 0, 1
    );

    // Bottom border
    addQuad(
        glm::vec3(-halfW, -halfH, halfD),
        glm::vec3(halfW, -halfH, halfD),
        glm::vec3(halfW, -halfH + frameWidth, halfD),
        glm::vec3(-halfW, -halfH + frameWidth, halfD),
        normalFront, 0, 0, 1, 0, 1, 1, 0, 1
    );

    // Left border
    addQuad(
        glm::vec3(-halfW, -halfH + frameWidth, halfD),
        glm::vec3(-halfW + frameWidth, -halfH + frameWidth, halfD),
        glm::vec3(-halfW + frameWidth, halfH - frameWidth, halfD),
        glm::vec3(-halfW, halfH - frameWidth, halfD),
        normalFront, 0, 0, 1, 0, 1, 1, 0, 1
    );

    // Right border
    addQuad(
        glm::vec3(halfW - frameWidth, -halfH + frameWidth, halfD),
        glm::vec3(halfW, -halfH + frameWidth, halfD),
        glm::vec3(halfW, halfH - frameWidth, halfD),
        glm::vec3(halfW - frameWidth, halfH - frameWidth, halfD),
        normalFront, 0, 0, 1, 0, 1, 1, 0, 1
    );

    // MIDDLE CROSS - Vertical bar
    addQuad(
        glm::vec3(midX - halfFW, -halfH + frameWidth, halfD),
        glm::vec3(midX + halfFW, -halfH + frameWidth, halfD),
        glm::vec3(midX + halfFW, halfH - frameWidth, halfD),
        glm::vec3(midX - halfFW, halfH - frameWidth, halfD),
        normalFront, 0, 0, 1, 0, 1, 1, 0, 1
    );

    // MIDDLE CROSS - Horizontal bar
    addQuad(
        glm::vec3(-halfW + frameWidth, midY - halfFW, halfD),
        glm::vec3(halfW - frameWidth, midY - halfFW, halfD),
        glm::vec3(halfW - frameWidth, midY + halfFW, halfD),
        glm::vec3(-halfW + frameWidth, midY + halfFW, halfD),
        normalFront, 0, 0, 1, 0, 1, 1, 0, 1
    );

    // Back side (same geometry but facing -Z)
    // Top border
    addQuad(
        glm::vec3(-halfW, halfH, -halfD),
        glm::vec3(halfW, halfH, -halfD),
        glm::vec3(halfW, halfH - frameWidth, -halfD),
        glm::vec3(-halfW, halfH - frameWidth, -halfD),
        normalBack, 0, 1, 1, 1, 1, 0, 0, 0
    );

    // Bottom border
    addQuad(
        glm::vec3(-halfW, -halfH + frameWidth, -halfD),
        glm::vec3(halfW, -halfH + frameWidth, -halfD),
        glm::vec3(halfW, -halfH, -halfD),
        glm::vec3(-halfW, -halfH, -halfD),
        normalBack, 0, 1, 1, 1, 1, 0, 0, 0
    );

    // Left border
    addQuad(
        glm::vec3(-halfW, halfH - frameWidth, -halfD),
        glm::vec3(-halfW + frameWidth, halfH - frameWidth, -halfD),
        glm::vec3(-halfW + frameWidth, -halfH + frameWidth, -halfD),
        glm::vec3(-halfW, -halfH + frameWidth, -halfD),
        normalBack, 0, 1, 1, 1, 1, 0, 0, 0
    );

    // Right border
    addQuad(
        glm::vec3(halfW - frameWidth, halfH - frameWidth, -halfD),
        glm::vec3(halfW, halfH - frameWidth, -halfD),
        glm::vec3(halfW, -halfH + frameWidth, -halfD),
        glm::vec3(halfW - frameWidth, -halfH + frameWidth, -halfD),
        normalBack, 0, 1, 1, 1, 1, 0, 0, 0
    );

    // Vertical bar
    addQuad(
        glm::vec3(midX - halfFW, halfH - frameWidth, -halfD),
        glm::vec3(midX + halfFW, halfH - frameWidth, -halfD),
        glm::vec3(midX + halfFW, -halfH + frameWidth, -halfD),
        glm::vec3(midX - halfFW, -halfH + frameWidth, -halfD),
        normalBack, 0, 1, 1, 1, 1, 0, 0, 0
    );

    // Horizontal bar
    addQuad(
        glm::vec3(-halfW + frameWidth, midY + halfFW, -halfD),
        glm::vec3(halfW - frameWidth, midY + halfFW, -halfD),
        glm::vec3(halfW - frameWidth, midY - halfFW, -halfD),
        glm::vec3(-halfW + frameWidth, midY - halfFW, -halfD),
        normalBack, 0, 1, 1, 1, 1, 0, 0, 0
    );

    // ============================================
    // ADD GLASS PANES (4 panes in 2x2 grid)
    // These will be rendered with transparency
    // ============================================

    float glassZ = 0.0f; // Glass sits in the middle

    // Top-left pane
    addQuad(
        glm::vec3(-halfW + frameWidth, midY + halfFW, glassZ),
        glm::vec3(midX - halfFW, midY + halfFW, glassZ),
        glm::vec3(midX - halfFW, halfH - frameWidth, glassZ),
        glm::vec3(-halfW + frameWidth, halfH - frameWidth, glassZ),
        normalFront, 0, 0.5f, 0.5f, 0.5f, 0.5f, 1, 0, 1
    );

    // Top-right pane
    addQuad(
        glm::vec3(midX + halfFW, midY + halfFW, glassZ),
        glm::vec3(halfW - frameWidth, midY + halfFW, glassZ),
        glm::vec3(halfW - frameWidth, halfH - frameWidth, glassZ),
        glm::vec3(midX + halfFW, halfH - frameWidth, glassZ),
        normalFront, 0.5f, 0.5f, 1, 0.5f, 1, 1, 0.5f, 1
    );

    // Bottom-left pane
    addQuad(
        glm::vec3(-halfW + frameWidth, -halfH + frameWidth, glassZ),
        glm::vec3(midX - halfFW, -halfH + frameWidth, glassZ),
        glm::vec3(midX - halfFW, midY - halfFW, glassZ),
        glm::vec3(-halfW + frameWidth, midY - halfFW, glassZ),
        normalFront, 0, 0, 0.5f, 0, 0.5f, 0.5f, 0, 0.5f
    );

    // Bottom-right pane
    addQuad(
        glm::vec3(midX + halfFW, -halfH + frameWidth, glassZ),
        glm::vec3(halfW - frameWidth, -halfH + frameWidth, glassZ),
        glm::vec3(halfW - frameWidth, midY - halfFW, glassZ),
        glm::vec3(midX + halfFW, midY - halfFW, glassZ),
        normalFront, 0.5f, 0, 1, 0, 1, 0.5f, 0.5f, 0.5f
    );
}

// lightweight generated UV sphere
static void generateSphere()
{
    const unsigned int X_SEGMENTS = 32;
    const unsigned int Y_SEGMENTS = 16;
    const float PI = 3.1415926f;

    for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
    {
        for (unsigned int x = 0; x < X_SEGMENTS; ++x)
        {
            float x0 = (float)x / X_SEGMENTS;
            float x1 = (float)(x + 1) / X_SEGMENTS;
            float y0 = (float)y / Y_SEGMENTS;
            float y1 = (float)(y + 1) / Y_SEGMENTS;

            float theta0 = x0 * 2.0f * PI;
            float theta1 = x1 * 2.0f * PI;
            float phi0 = y0 * PI;
            float phi1 = y1 * PI;

            glm::vec3 p0(
                cos(theta0) * sin(phi0),
                cos(phi0),
                sin(theta0) * sin(phi0)
            );
            glm::vec3 p1(
                cos(theta1) * sin(phi0),
                cos(phi0),
                sin(theta1) * sin(phi0)
            );
            glm::vec3 p2(
                cos(theta1) * sin(phi1),
                cos(phi1),
                sin(theta1) * sin(phi1)
            );
            glm::vec3 p3(
                cos(theta0) * sin(phi1),
                cos(phi1),
                sin(theta0) * sin(phi1)
            );

            auto pushVertex = [](glm::vec3 p, float u, float v)
                {
                    sphereVertices.push_back(p.x);
                    sphereVertices.push_back(p.y);
                    sphereVertices.push_back(p.z);


                    sphereVertices.push_back(p.x);
                    sphereVertices.push_back(p.y);
                    sphereVertices.push_back(p.z);

                    sphereVertices.push_back(u);
                    sphereVertices.push_back(v);
                };

            // Triangle 1
            pushVertex(p0, x0, y0);
            pushVertex(p1, x1, y0);
            pushVertex(p2, x1, y1);

            // Triangle 2
            pushVertex(p0, x0, y0);
            pushVertex(p2, x1, y1);
            pushVertex(p3, x0, y1);
        }
    }
}


static void generateCylinder()
{
    const unsigned int SEGMENTS = 32;
    const float PI = 3.1415926f;
    const float radius = 0.5f;
    const float height = 1.0f;

    auto pushVertex = [](float x, float y, float z, float nx, float ny, float nz, float u, float v)
        {
            cylinderVertices.push_back(x);
            cylinderVertices.push_back(y);
            cylinderVertices.push_back(z);
            cylinderVertices.push_back(nx);
            cylinderVertices.push_back(ny);
            cylinderVertices.push_back(nz);
            cylinderVertices.push_back(u);
            cylinderVertices.push_back(v);
        };

    // Top and bottom circles
    for (unsigned int i = 0; i < SEGMENTS; ++i)
    {
        float angle1 = (float)i / SEGMENTS * 2.0f * PI;
        float angle2 = (float)(i + 1) / SEGMENTS * 2.0f * PI;

        float x1 = cos(angle1) * radius;
        float z1 = sin(angle1) * radius;
        float x2 = cos(angle2) * radius;
        float z2 = sin(angle2) * radius;

        // Top cap
        pushVertex(0.0f, height / 2, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f);
        pushVertex(x1, height / 2, z1, 0.0f, 1.0f, 0.0f, 0.5f + x1, 0.5f + z1);
        pushVertex(x2, height / 2, z2, 0.0f, 1.0f, 0.0f, 0.5f + x2, 0.5f + z2);

        // Bottom cap
        pushVertex(0.0f, -height / 2, 0.0f, 0.0f, -1.0f, 0.0f, 0.5f, 0.5f);
        pushVertex(x2, -height / 2, z2, 0.0f, -1.0f, 0.0f, 0.5f + x2, 0.5f + z2);
        pushVertex(x1, -height / 2, z1, 0.0f, -1.0f, 0.0f, 0.5f + x1, 0.5f + z1);

        // Side faces
        float nx1 = cos(angle1);
        float nz1 = sin(angle1);
        float nx2 = cos(angle2);
        float nz2 = sin(angle2);

        // First triangle of quad
        pushVertex(x1, -height / 2, z1, nx1, 0.0f, nz1, (float)i / SEGMENTS, 0.0f);
        pushVertex(x1, height / 2, z1, nx1, 0.0f, nz1, (float)i / SEGMENTS, 1.0f);
        pushVertex(x2, height / 2, z2, nx2, 0.0f, nz2, (float)(i + 1) / SEGMENTS, 1.0f);

        // Second triangle of quad
        pushVertex(x1, -height / 2, z1, nx1, 0.0f, nz1, (float)i / SEGMENTS, 0.0f);
        pushVertex(x2, height / 2, z2, nx2, 0.0f, nz2, (float)(i + 1) / SEGMENTS, 1.0f);
        pushVertex(x2, -height / 2, z2, nx2, 0.0f, nz2, (float)(i + 1) / SEGMENTS, 0.0f);
    }
}

// Generate paraboloid shape (y = a * (x^2 + z^2))
static void generateParaboloid()
{
    paraboloidVertices.clear();

    const unsigned int RADIAL_SEGMENTS = 32;
    const unsigned int HEIGHT_SEGMENTS = 16;
    const float PI = 3.1415926f;
    const float maxRadius = 0.5f;
    const float height = 1.0f;
    const float a = height / (maxRadius * maxRadius); // Parabola coefficient

    auto pushVertex = [](float x, float y, float z, float nx, float ny, float nz, float u, float v)
        {
            paraboloidVertices.push_back(x);
            paraboloidVertices.push_back(y);
            paraboloidVertices.push_back(z);
            paraboloidVertices.push_back(nx);
            paraboloidVertices.push_back(ny);
            paraboloidVertices.push_back(nz);
            paraboloidVertices.push_back(u);
            paraboloidVertices.push_back(v);
        };

    // Generate the paraboloid surface
    for (unsigned int h = 0; h < HEIGHT_SEGMENTS; ++h)
    {
        for (unsigned int r = 0; r < RADIAL_SEGMENTS; ++r)
        {
            // Calculate radii for current and next height level
            float t0 = (float)h / HEIGHT_SEGMENTS;
            float t1 = (float)(h + 1) / HEIGHT_SEGMENTS;

            float y0 = t0 * height - height / 2.0f;  // Map to [-height/2, height/2]
            float y1 = t1 * height - height / 2.0f;

            // For paraboloid: r = sqrt(y/a), but we reverse it
            float radius0 = sqrt(abs(y0 + height / 2.0f) / a);
            float radius1 = sqrt(abs(y1 + height / 2.0f) / a);

            // Calculate angles
            float angle0 = (float)r / RADIAL_SEGMENTS * 2.0f * PI;
            float angle1 = (float)(r + 1) / RADIAL_SEGMENTS * 2.0f * PI;

            // Calculate positions
            float x00 = cos(angle0) * radius0;
            float z00 = sin(angle0) * radius0;
            float x01 = cos(angle1) * radius0;
            float z01 = sin(angle1) * radius0;
            float x10 = cos(angle0) * radius1;
            float z10 = sin(angle0) * radius1;
            float x11 = cos(angle1) * radius1;
            float z11 = sin(angle1) * radius1;

            // Calculate normals (derivative of paraboloid)
            glm::vec3 p00(x00, y0, z00);
            glm::vec3 p01(x01, y0, z01);
            glm::vec3 p10(x10, y1, z10);
            glm::vec3 p11(x11, y1, z11);

            // Approximate normal for each vertex
            float dydx0 = 2.0f * a * radius0 * cos(angle0);
            float dydz0 = 2.0f * a * radius0 * sin(angle0);
            glm::vec3 n00 = glm::normalize(glm::vec3(-dydx0, 1.0f, -dydz0));

            float dydx1 = 2.0f * a * radius0 * cos(angle1);
            float dydz1 = 2.0f * a * radius0 * sin(angle1);
            glm::vec3 n01 = glm::normalize(glm::vec3(-dydx1, 1.0f, -dydz1));

            float dydx2 = 2.0f * a * radius1 * cos(angle0);
            float dydz2 = 2.0f * a * radius1 * sin(angle0);
            glm::vec3 n10 = glm::normalize(glm::vec3(-dydx2, 1.0f, -dydz2));

            float dydx3 = 2.0f * a * radius1 * cos(angle1);
            float dydz3 = 2.0f * a * radius1 * sin(angle1);
            glm::vec3 n11 = glm::normalize(glm::vec3(-dydx3, 1.0f, -dydz3));

            // UV coordinates
            float u0 = (float)r / RADIAL_SEGMENTS;
            float u1 = (float)(r + 1) / RADIAL_SEGMENTS;
            float v0 = t0;
            float v1 = t1;

            // First triangle
            pushVertex(p00.x, p00.y, p00.z, n00.x, n00.y, n00.z, u0, v0);
            pushVertex(p10.x, p10.y, p10.z, n10.x, n10.y, n10.z, u0, v1);
            pushVertex(p11.x, p11.y, p11.z, n11.x, n11.y, n11.z, u1, v1);

            // Second triangle
            pushVertex(p00.x, p00.y, p00.z, n00.x, n00.y, n00.z, u0, v0);
            pushVertex(p11.x, p11.y, p11.z, n11.x, n11.y, n11.z, u1, v1);
            pushVertex(p01.x, p01.y, p01.z, n01.x, n01.y, n01.z, u1, v0);
        }
    }

    // Add bottom cap (optional)
    for (unsigned int i = 0; i < RADIAL_SEGMENTS; ++i)
    {
        float angle1 = (float)i / RADIAL_SEGMENTS * 2.0f * PI;
        float angle2 = (float)(i + 1) / RADIAL_SEGMENTS * 2.0f * PI;

        float x1 = cos(angle1) * 0.01f;  // Very small radius at bottom
        float z1 = sin(angle1) * 0.01f;
        float x2 = cos(angle2) * 0.01f;
        float z2 = sin(angle2) * 0.01f;

        pushVertex(0.0f, -height / 2.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.5f, 0.5f);
        pushVertex(x2, -height / 2.0f, z2, 0.0f, -1.0f, 0.0f, 0.5f + x2, 0.5f + z2);
        pushVertex(x1, -height / 2.0f, z1, 0.0f, -1.0f, 0.0f, 0.5f + x1, 0.5f + z1);
    }
}

const std::vector<float>& Mesh::GetVertices(Type type)
{
    if (sphereVertices.empty())
        generateSphere();

    if (windowVertices.empty())
        generateWindow();

    if (cylinderVertices.empty())
        generateCylinder();

    if (paraboloidVertices.empty())
        generateParaboloid();

    switch (type)
    {
    case CUBE:         return cubeVertices;
    case PLANE:        return planeVertices;
    case SPHERE:       return sphereVertices;
    case TETRAHEDRON:  return tetrahedronVertices;
    case PENTAHEDRON:  return pentahedronVertices;
    case WINDOW:       return windowVertices;
    case CYLINDER:     return cylinderVertices;
    case PARABOLOID:   return paraboloidVertices;
    }

    return cubeVertices;
}



int Mesh::GetVertexCount(Type type)
{
    switch (type)
    {
    case CUBE:         return (int)cubeVertices.size() / 8;
    case PLANE:        return (int)planeVertices.size() / 8;
    case SPHERE:       return (int)sphereVertices.size() / 8;
    case TETRAHEDRON:  return (int)tetrahedronVertices.size() / 8;
    case PENTAHEDRON:  return (int)pentahedronVertices.size() / 8;
    case WINDOW:       return (int)windowVertices.size() / 8;
    case CYLINDER:     return (int)cylinderVertices.size() / 8;
    case PARABOLOID:   return (int)paraboloidVertices.size() / 8;
    }

    return 0;
}