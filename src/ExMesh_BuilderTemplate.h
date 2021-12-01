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
    void MeshBuilder<VT>::AppendQuad(const Vector3& pos0,const Vector3&pos1,const Vector3&pos2,const Vector3&pos3,const Vector3&normal)
    {
        VT v0, v1, v2, v3;

        int index0=vertices.size();
        int index1=index0+1;
        int index2=index0+2;
        int index3=index0+3;

        v0.position=pos0;v0.normal=normal;v0.uv=Vector2(0,0);
        v1.position=pos1;v1.normal=normal;v1.uv=Vector2(1,0);
        v2.position=pos2;v2.normal=normal;v2.uv=Vector2(1,1);
        v3.position=pos3;v3.normal=normal;v3.uv=Vector2(0,1);

        vertices.push_back(v0);
        vertices.push_back(v1);
        vertices.push_back(v2);
        vertices.push_back(v3);

        AddQuad(index0,index1,index2,index3);

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
        Vector3 luf(-0.5, 0.5, 0.5); // luf 
        Vector3 ruf(0.5, 0.5, 0.5); // ruf 
        Vector3 rdf(0.5, -0.5, 0.5); // rdf 
        Vector3 ldf(-0.5, -0.5, 0.5); // ldf 

        Vector3 lub(-0.5, 0.5, -0.5); // lub 
        Vector3 rub(0.5, 0.5, -0.5); //rub 
        Vector3 rdb(0.5, -0.5, -0.5); // rdb 
        Vector3 ldb(-0.5, -0.5, -0.5); // ldb 

        mBuilder.AppendQuad(luf,ruf,rdf,ldf,Vector3(0,0,1));
        mBuilder.AppendQuad(lub,ldb,rdb,rub,Vector3(0,0,-1));

        mBuilder.AppendQuad(luf,lub,rub,ruf,Vector3(0,1,0));
        mBuilder.AppendQuad(ldb,ldf,rdf,rdb,Vector3(0,-1,0));

        mBuilder.AppendQuad(rub,rdb,rdf,ruf,Vector3(1,0,0));
        mBuilder.AppendQuad(luf,ldf,ldb,lub,Vector3(-1,0,0));
          
        return mBuilder.GenerateMesh();
    }

    template<class VT>
    Mesh<VT> MeshBuilder<VT>::Quad()
    {
        MeshBuilder mBuilder;
        Vector3 lu(-0.5, 0.5, 0); // lu 
        Vector3 ru(0.5, 0.5, 0); // ru
        Vector3 rd(0.5, -0.5, 0); // rd
        Vector3 ld(-0.5, -0.5, 0); // ld


        mBuilder.AppendQuad(lu,ru,rd,ld,Vector3(0,0,1));
          
        return mBuilder.GenerateMesh();
    }

    template<class VT>
    Mesh<VT> MeshBuilder<VT>::Sphere()
    {
        MeshBuilder mBuilder;

        int poly=64;
        int hPoly=poly/2;
        float PI=3.1415926;

        for(int i=1;i<hPoly;i++)
        {
            float angle=i*PI/hPoly;
            float y=-0.5*cos(angle);
            // y=0;
            float r=sqrtf(0.25-y*y);
            for(int j=0;j<poly;j++)
            {
                float angle=PI*2*j/poly;
                float x=cos(angle)*r;
                float z=sin(angle)*r;
                VT v;
                v.position=Vector3(x,y,z);
                v.normal=v.position*2;
                mBuilder.AddVertex(v);
            }
        }

        for(int j=0;j<hPoly-2;j++)
        {
            int base=j*poly;
            for(int i=0;i<poly-1;i++)
            {
                mBuilder.AddQuad(base+i,base+i+1,base+i+poly+1,base+i+poly);
            }
            mBuilder.AddQuad(base+poly-1,base,base+poly,base+poly*2-1);
        }

        VT vBottom;
        vBottom.position=Vector3(0,-0.5,0);
        vBottom.normal=vBottom.position*2;
        mBuilder.AddVertex(vBottom);
        VT vUp;
        vUp.position=Vector3(0,0.5,0);
        vUp.normal=vUp.position*2;
        mBuilder.AddVertex(vUp);

        int vupIndex=mBuilder.vertices.size()-1;
        int vbottomIndex=vupIndex-1;
        for(int i=0;i<poly-1;i++)
        {
            mBuilder.AddTriangle(vbottomIndex,i+1,i);
        }
        mBuilder.AddTriangle(vbottomIndex,0,poly-1);

        int base=vupIndex-poly-1;
        for(int i=0;i<poly-1;i++)
        {
            mBuilder.AddTriangle(vupIndex,base+i,base+i+1);
        }
        mBuilder.AddTriangle(vupIndex,base+poly-1,base);

        

          
        return mBuilder.GenerateMesh();
    }
}