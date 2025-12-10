#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>

class Mesh
{
public:
    enum Type {
        CUBE,
        PLANE,
        SPHERE,
        TETRAHEDRON,
        PENTAHEDRON
    };


    static const std::vector<float>& GetVertices(Type type);
    static int GetVertexCount(Type type);
};

#endif
