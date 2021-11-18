#include"ExMesh_Declaration.h"

namespace ExRenderer
{
    template<class VT>
    void MeshBuilder<VT>::AddVertex(const VT &vertice)
    {
        vertices.push_back(vertice);
    }

    template<class VT>
    void MeshBuilder<VT>::AddTriangle(uint32_t index1, uint32_t index2, uint32_t index3)
    {
        indices.push_back(index1);
        indices.push_back(index2);
        indices.push_back(index3);
    }

    template<class VT>
    void MeshBuilder<VT>::AddQuad(uint32_t index0,uint32_t index1,uint32_t index2,uint32_t index3)
    {
        AddTriangle(index0, index1, index2);
        AddTriangle(index0, index2, index3);
    }

    template<class VT>
    Mesh<VT> MeshBuilder<VT>::GenerateMesh()
    {
        VT *allVertices = vertices.data();
        uint32_t verticeSize = vertices.size();
        uint32_t *allIndices = indices.data();
        uint32_t indiceSize = indices.size() / 3;
        return Mesh<VT>(allVertices, verticeSize, allIndices, indiceSize);
    }

    template<class VT>
    Mesh<VT> MeshBuilder<VT>::Cube()
    {
        MeshBuilder mBuilder;
        VT v1, v2, v3, v4, v5, v6, v7, v8;
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
        mBuilder.AddQuad(5,4,7,6); // back
        mBuilder.AddQuad(1,0,4,5); // up
        mBuilder.AddQuad(2,6,7,3); // down
        mBuilder.AddQuad(0,3,7,4); // left
        mBuilder.AddQuad(1,5,6,2); // right            
        return mBuilder.GenerateMesh();
    }
}