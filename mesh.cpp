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


// lightweight generated UV sphere
static std::vector<float> sphereVertices;

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




const std::vector<float>& Mesh::GetVertices(Type type)
{
    if (sphereVertices.empty())
        generateSphere();

    switch (type)
    {
    case CUBE:         return cubeVertices;
    case PLANE:        return planeVertices;
    case SPHERE:       return sphereVertices;
    case TETRAHEDRON:  return tetrahedronVertices;
    case PENTAHEDRON:  return pentahedronVertices;
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
    }

    return 0;
}
