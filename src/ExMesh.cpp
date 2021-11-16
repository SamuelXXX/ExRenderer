#include"ExMesh.h"

namespace ExRenderer
{
    Mesh::Mesh(VerticeType *verticeBuffer,uint32_t vSize,uint32_t *indexBuffer,uint32_t iSize)
    {
        m_verticeSize=vSize;
        m_indexSize=iSize;

        m_verticeBuffer=new VerticeType[m_verticeSize];
        memcpy(m_verticeBuffer,verticeBuffer,m_verticeSize*sizeof(VerticeType));
        m_indexBuffer=new uint32_t[m_indexSize*3];
        memcpy(m_indexBuffer,indexBuffer,m_indexSize*3*sizeof(uint32_t));      
    }

    Mesh::~Mesh()
    {
        delete [] m_verticeBuffer;
        delete [] m_indexBuffer;
    }

    Mesh::Mesh(const Mesh &other)
    {
        m_verticeSize=other.m_verticeSize;
        m_indexSize=other.m_indexSize;

        m_verticeBuffer=new VerticeType[m_verticeSize];
        memcpy(m_verticeBuffer,other.m_verticeBuffer,m_verticeSize*sizeof(VerticeType));
        m_indexBuffer=new uint32_t[m_indexSize*3];
        memcpy(m_indexBuffer,other.m_indexBuffer,m_indexSize*3*sizeof(uint32_t));     
    }

    Mesh::Mesh(Mesh &&other)
    {
        m_verticeSize=other.m_verticeSize;
        m_indexSize=other.m_indexSize;

        m_verticeBuffer=other.m_verticeBuffer;
        m_indexBuffer=other.m_indexBuffer;  

        other.m_verticeBuffer=nullptr;
        other.m_indexBuffer=nullptr;
    }

    Mesh& Mesh::operator=(const Mesh &other)
    {
        delete [] m_verticeBuffer;
        delete [] m_indexBuffer;

        m_verticeSize=other.m_verticeSize;
        m_indexSize=other.m_indexSize;

        m_verticeBuffer=new VerticeType[m_verticeSize];
        memcpy(m_verticeBuffer,other.m_verticeBuffer,m_verticeSize*sizeof(VerticeType));
        m_indexBuffer=new uint32_t[m_indexSize*3];
        memcpy(m_indexBuffer,other.m_indexBuffer,m_indexSize*3*sizeof(uint32_t)); 

        return *this; 
    }

    Mesh& Mesh::operator=(Mesh &&other)
    {
        delete [] m_verticeBuffer;
        delete [] m_indexBuffer;

        m_verticeSize=other.m_verticeSize;
        m_indexSize=other.m_indexSize;

        m_verticeBuffer=other.m_verticeBuffer;
        m_indexBuffer=other.m_indexBuffer;  

        other.m_verticeBuffer=nullptr;
        other.m_indexBuffer=nullptr;
        return *this;
    }

    void Mesh::MeshIterator::setPointers()
    {
        if(m_index<0||m_index>=m_mesh->m_indexSize)
        {
            first=second=third=nullptr;
        }
        else
        {
            int firstIndex=m_mesh->m_indexBuffer[3*m_index];
            int secondIndex=m_mesh->m_indexBuffer[3*m_index+1];
            int thirdIndex=m_mesh->m_indexBuffer[3*m_index+2];
            first=&(m_mesh->m_verticeBuffer[firstIndex]);
            second=&(m_mesh->m_verticeBuffer[secondIndex]);
            third=&(m_mesh->m_verticeBuffer[thirdIndex]);
        }
    }

    Mesh::MeshIterator::MeshIterator(Mesh *mesh,uint32_t index)
    {
        m_mesh=mesh;
        m_index=index;
        setPointers();
    }

    Mesh::MeshIterator& Mesh::MeshIterator::operator*()
    {
        return *this;
    }

    Mesh::MeshIterator& Mesh::MeshIterator::operator++()
    {
        m_index++;
        setPointers();
        return *this;
    }

    bool Mesh::MeshIterator::operator==(const Mesh::MeshIterator &other)
    {
        return m_index==other.m_index&&m_mesh==other.m_mesh;
    }

    bool Mesh::MeshIterator::operator!=(const Mesh::MeshIterator &other)
    {
        return !(*this==other);
    }

    Mesh::MeshIterator Mesh::begin()
    {
        return Mesh::MeshIterator(this,0);
    }

    Mesh::MeshIterator Mesh::end()
    {
        return Mesh::MeshIterator(this,m_indexSize);
    }
}