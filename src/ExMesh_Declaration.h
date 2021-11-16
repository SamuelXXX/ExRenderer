#ifndef __EXMESH_DECLARATION_H__
#define __EXMESH_DECLARATION_H__

#include <stdint.h>
#include "ExMath.h"
#include <iostream>
#include <vector>

namespace ExRenderer
{
    struct BasicVertex
    {
        Vector3 position;
    };

    template <class VT>
    class MeshIterator;

    template <class VT>
    class Mesh
    {
        VT *m_verticeBuffer;
        uint32_t m_verticeSize;
        uint32_t *m_indexBuffer;
        uint32_t m_indexSize;

    public:
        Mesh<VT>() = default;
        Mesh<VT>(VT *verticeBuffer, uint32_t vSize, uint32_t *indexBuffer, uint32_t iSize);
        ~Mesh<VT>();

        Mesh<VT>(const Mesh<VT> &other);
        Mesh<VT>(Mesh<VT> &&other);
        Mesh<VT> &operator=(const Mesh<VT> &other);
        Mesh<VT> &operator=(Mesh<VT> &&other);

    public: // Mesh Iterators
        
        MeshIterator<VT> begin();
        MeshIterator<VT> end();

        friend class MeshIterator<VT>;
    };

    template<class VT>
    class MeshIterator
    {
        Mesh<VT> *m_mesh;
        uint32_t m_index;
        void setPointers();

    public:
        VT *vertex1;
        VT *vertex2;
        VT *vertex3;
        MeshIterator<VT>(Mesh<VT> *mesh, uint32_t index);
        MeshIterator<VT> &operator*();
        MeshIterator<VT> &operator++();
        bool operator==(const MeshIterator<VT> &other);
        bool operator!=(const MeshIterator<VT> &other);
    };

    template <class VT>
    class MeshBuilder
    {
        std::vector<VT> vertices;
        std::vector<uint32_t> indices;

    public:
        static Mesh<VT> Cube();

    public:
        void AddVertex(const VT &vertice);
        void AddTriangle(uint32_t index1, uint32_t index2, uint32_t index3);
        void AddQuad(uint32_t index0,uint32_t index1,uint32_t index2,uint32_t index3);
        Mesh<VT> GenerateMesh();
    };
}

#endif