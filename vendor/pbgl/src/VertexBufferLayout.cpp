#include "VertexBufferLayout.h"

template<>
void VertexBufferLayout::Push<float>(unsigned int count)
{
    m_elements.push_back({ GL_FLOAT, count, GL_FALSE});
    m_stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
}

template<>
void VertexBufferLayout::Push<unsigned int>(unsigned int count)
{
    m_elements.push_back({ GL_FLOAT, count, GL_FALSE});
    m_stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
}

template<>
void VertexBufferLayout::Push<unsigned char>(unsigned int count)
{
    m_elements.push_back({ GL_FLOAT, count, GL_TRUE });
    m_stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
}
