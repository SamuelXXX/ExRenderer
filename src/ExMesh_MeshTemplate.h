#include "ExMesh_Declaration.h"

namespace ExRenderer
{
    template <class VT>
    Mesh<VT>::Mesh(VT *verticeBuffer, uint32_t vSize, uint32_t *indexBuffer, uint32_t iSize)
    {
        m_verticeSize = vSize;
        m_indexSize = iSize;

        m_verticeBuffer = new VT[m_verticeSize];
        memcpy(m_verticeBuffer, verticeBuffer, m_verticeSize * sizeof(VT));
        m_indexBuffer = new uint32_t[m_indexSize * 3];
        memcpy(m_indexBuffer, indexBuffer, m_indexSize * 3 * sizeof(uint32_t));
    }

    template <class VT>
    Mesh<VT>::~Mesh()
    {
        delete[] m_verticeBuffer;
        delete[] m_indexBuffer;
    }

    template <class VT>
    Mesh<VT>::Mesh(const Mesh &other)
    {
        m_verticeSize = other.m_verticeSize;
        m_indexSize = other.m_indexSize;

        m_verticeBuffer = new VT[m_verticeSize];
        memcpy(m_verticeBuffer, other.m_verticeBuffer, m_verticeSize * sizeof(VT));
        m_indexBuffer = new uint32_t[m_indexSize * 3];
        memcpy(m_indexBuffer, other.m_indexBuffer, m_indexSize * 3 * sizeof(uint32_t));
    }

    template <class VT>
    Mesh<VT>::Mesh(Mesh &&other)
    {
        m_verticeSize = other.m_verticeSize;
        m_indexSize = other.m_indexSize;

        m_verticeBuffer = other.m_verticeBuffer;
        m_indexBuffer = other.m_indexBuffer;

        other.m_verticeBuffer = nullptr;
        other.m_indexBuffer = nullptr;
    }

    template <class VT>
    Mesh<VT> &Mesh<VT>::operator=(const Mesh<VT> &other)
    {
        delete[] m_verticeBuffer;
        delete[] m_indexBuffer;

        m_verticeSize = other.m_verticeSize;
        m_indexSize = other.m_indexSize;

        m_verticeBuffer = new VT[m_verticeSize];
        memcpy(m_verticeBuffer, other.m_verticeBuffer, m_verticeSize * sizeof(VT));
        m_indexBuffer = new uint32_t[m_indexSize * 3];
        memcpy(m_indexBuffer, other.m_indexBuffer, m_indexSize * 3 * sizeof(uint32_t));

        return *this;
    }

    template <class VT>
    Mesh<VT> &Mesh<VT>::operator=(Mesh<VT> &&other)
    {
        delete[] m_verticeBuffer;
        delete[] m_indexBuffer;

        m_verticeSize = other.m_verticeSize;
        m_indexSize = other.m_indexSize;

        m_verticeBuffer = other.m_verticeBuffer;
        m_indexBuffer = other.m_indexBuffer;

        other.m_verticeBuffer = nullptr;
        other.m_indexBuffer = nullptr;
        return *this;
    }

    template <class VT>
    void MeshIterator<VT>::setPointers()
    {
        if (m_index < 0 || m_index >= m_mesh->m_indexSize)
        {
            vertex1 = vertex2 = vertex3 = nullptr;
        }
        else
        {
            int firstIndex = m_mesh->m_indexBuffer[3 * m_index];
            int secondIndex = m_mesh->m_indexBuffer[3 * m_index + 1];
            int thirdIndex = m_mesh->m_indexBuffer[3 * m_index + 2];
            vertex1 = &(m_mesh->m_verticeBuffer[firstIndex]);
            vertex2 = &(m_mesh->m_verticeBuffer[secondIndex]);
            vertex3 = &(m_mesh->m_verticeBuffer[thirdIndex]);
        }
    }

    template <class VT>
    MeshIterator<VT>::MeshIterator(Mesh<VT> *mesh, uint32_t index)
    {
        m_mesh = mesh;
        m_index = index;
        setPointers();
    }

    template <class VT>
    MeshIterator<VT> &MeshIterator<VT>::operator*()
    {
        return *this;
    }

    template <class VT>
    MeshIterator<VT> &MeshIterator<VT>::operator++()
    {
        m_index++;
        setPointers();
        return *this;
    }

    template <class VT>
    bool MeshIterator<VT>::operator==(const MeshIterator<VT> &other)
    {
        return m_index == other.m_index && m_mesh == other.m_mesh;
    }

    template <class VT>
    bool MeshIterator<VT>::operator!=(const MeshIterator<VT> &other)
    {
        return !(*this == other);
    }

    template <class VT>
    MeshIterator<VT> Mesh<VT>::begin()
    {
        return MeshIterator<VT>(this, 0);
    }

    template <class VT>
    MeshIterator<VT> Mesh<VT>::end()
    {
        return MeshIterator<VT>(this, m_indexSize);
    }
}