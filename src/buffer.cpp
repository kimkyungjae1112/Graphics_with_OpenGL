#include "buffer.h"

BufferUPtr Buffer::CreateWithData(uint32_t bufferType, uint32_t usage, const void* data, size_t dataSize) 
{
    BufferUPtr buffer = BufferUPtr(new Buffer());
    if (!buffer->Init(bufferType, usage, data, dataSize)) return nullptr;
    return std::move(buffer);
}

Buffer::~Buffer() 
{
    if (m_buffer) 
    {
        glDeleteBuffers(1, &m_buffer);
    }
}

void Buffer::Bind() const 
{
    // GL_ARRAY_BUFFER로 바인딩 할 버퍼 핸들을 설정
    // GL_ARRAY_BUFFER를 m_buffer로 설정해주는겨
    glBindBuffer(m_bufferType, m_buffer);
}

bool Buffer::Init(uint32_t bufferType, uint32_t usage, const void* data, size_t dataSize) 
{
    m_bufferType = bufferType;
    m_usage = usage;
    glGenBuffers(1, &m_buffer);
    Bind();
    glBufferData(m_bufferType, dataSize, data, usage);
    return true;
}