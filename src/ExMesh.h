#ifndef __EXMESH_H__
#define __EXMESH_H__

#include <stdint.h>
#include "ExMath.h"
#include <iostream>
#include <vector>

namespace ExRenderer
{
    struct VerticeType
    {
        Vector3 position;
    };

    // template <class VerticeType>
    class Mesh
    {
        VerticeType *m_verticeBuffer;
        uint32_t m_verticeSize;
        uint32_t *m_indexBuffer;
        uint32_t m_indexSize;

    public:
        Mesh() = default;
        Mesh(VerticeType *verticeBuffer, uint32_t vSize, uint32_t *indexBuffer, uint32_t iSize);
        ~Mesh();

        Mesh(const Mesh &other);
        Mesh(Mesh &&other);
        Mesh &operator=(const Mesh &other);
        Mesh &operator=(Mesh &&other);

    public: // Mesh Iterators
        class MeshIterator
        {
            Mesh *m_mesh;
            uint32_t m_index;
            void setPointers();

        public:
            VerticeType *first;
            VerticeType *second;
            VerticeType *third;
            MeshIterator(Mesh *mesh, uint32_t index);
            MeshIterator &operator*();
            MeshIterator &operator++();
            bool operator==(const MeshIterator &other);
            bool operator!=(const MeshIterator &other);
        };
        MeshIterator begin();
        MeshIterator end();
    };

    class MeshBuilder
    {
        std::vector<VerticeType> vertices;
        std::vector<uint32_t> indices;

    public:
        void AppendVertex(const VerticeType &vertice)
        {
            vertices.push_back(vertice);
        }

        void AppendTriangle(uint32_t index1, uint32_t index2, uint32_t index3)
        {
            indices.push_back(index1);
            indices.push_back(index2);
            indices.push_back(index3);
        }

        Mesh GenerateMesh()
        {
            VerticeType *allVertices = vertices.data();
            uint32_t verticeSize = vertices.size();
            uint32_t *allIndices = indices.data();
            uint32_t indiceSize = indices.size() / 3;
            return Mesh(allVertices, verticeSize, allIndices, indiceSize);
        }
    };
}

#endif