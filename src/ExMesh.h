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
        static Mesh Cube()
        {
            MeshBuilder mBuilder;
            VerticeType v1, v2, v3, v4, v5, v6, v7, v8;
            v1.position = Vector3(-0.5, 0.5, 0.5); // luf 0
            v2.position = Vector3(0.5, 0.5, 0.5); // ruf 1
            v3.position = Vector3(0.5, -0.5, 0.5); // rdf 2
            v4.position = Vector3(-0.5, -0.5, 0.5); // ldf 3
            v5.position = Vector3(-0.5, 0.5, -0.5); // lub 4
            v6.position = Vector3(0.5, 0.5, -0.5); //rub 5
            v7.position = Vector3(0.5, -0.5, -0.5); // rdb 6
            v8.position = Vector3(-0.5, -0.5, -0.5); // ldb 7
            mBuilder.AddVertex(v1);
            mBuilder.AddVertex(v2);
            mBuilder.AddVertex(v3);
            mBuilder.AddVertex(v4);
            mBuilder.AddVertex(v5);
            mBuilder.AddVertex(v6);
            mBuilder.AddVertex(v7);
            mBuilder.AddVertex(v8);
            mBuilder.AddQuad(0,1,2,3); // forward
            mBuilder.AddQuad(4,5,6,7); // back
            mBuilder.AddQuad(0,1,5,4); // up
            mBuilder.AddQuad(2,3,7,6); // down
            mBuilder.AddQuad(0,4,7,3); // left
            mBuilder.AddQuad(1,2,6,5); // right            
            return mBuilder.GenerateMesh();
        }

    public:
        void AddVertex(const VerticeType &vertice)
        {
            vertices.push_back(vertice);
        }

        void AddTriangle(uint32_t index1, uint32_t index2, uint32_t index3)
        {
            indices.push_back(index1);
            indices.push_back(index2);
            indices.push_back(index3);
        }

        void AddQuad(uint32_t index0,uint32_t index1,uint32_t index2,uint32_t index3)
        {
            AddTriangle(index0, index1, index2);
            AddTriangle(index0, index3, index2);
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