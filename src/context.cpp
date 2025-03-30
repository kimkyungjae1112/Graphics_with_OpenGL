#include "context.h"
#include "image.h"

ContextUPtr Context::Create() 
{
    ContextUPtr context = ContextUPtr(new Context());
    if (!context->Init()) return nullptr;
    return std::move(context);
}

bool Context::Init() 
{
    float vertices[] = {
         0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    };

    uint32_t indices[] = { // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3, // second triangle
    };

    // VBO를 만들기 전에 만들어야 한다.
    m_vertexLayout = VertexLayout::Create();

    // 1. vertex Buffer 생성
    // 2. 각 vertex 에 위치값, 색상값들이 들어가는 용도의 buffer
    // 3. vertext buffer 에 데이터 복사 -> m_vertextBuffer
    m_vertexBuffer = Buffer::CreateWithData(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices, sizeof(float) * 32);

    // 0번 attribute를 사용한다.
    // 0번 attribute는 어떻게 생겼냐? 
    // vertex shader에 있는 변수를 attribute 라고 하는듯
    // attribute가 몇 개의 값으로 구성되어 있는지와 type, 정규화된 값, stride (attribute 간 간격)
    // 첫 정점의 해당 attribute까지의 간격 (byte 단위)
    //m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
    m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0);
    m_vertexLayout->SetAttrib(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, sizeof(float) * 3);
    m_vertexLayout->SetAttrib(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, sizeof(float) * 6);

    m_indexBuffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(uint32_t) * 6);

    // program을 생성하기 위해 인자로 들어갈 포인터 유형을 shared pointer로 변경한다.
    ShaderPtr vertShader = Shader::CreateFromFile("./shader/texture.vs", GL_VERTEX_SHADER);
    ShaderPtr fragShader = Shader::CreateFromFile("./shader/texture.fs", GL_FRAGMENT_SHADER);
    if (!vertShader || !fragShader) return false;
    SPDLOG_INFO("vertex shader id: {}", vertShader->Get());
    SPDLOG_INFO("fragment shader id: {}", fragShader->Get());

    m_program = Program::Create({fragShader, vertShader});
    if (!m_program) return false;
    SPDLOG_INFO("program id: {}", m_program->Get());

    glClearColor(0.1f, 0.2f, 0.3f, 0.0f);
    
    auto image = Image::Load("./image/container.jpg");
    if (!image) return false;
    SPDLOG_INFO("image: {}x{}, {} channels", image->GetWidth(), image->GetHeight(), image->GetChannelCount());

    auto image2 = Image::Load("./image/awesomeface.png");

    m_texture = Texture::CreateFromImage(image.get());
	m_texture2 = Texture::CreateFromImage(image2.get());
   
    glActiveTexture(GL_TEXTURE0); // 0번 슬롯과 
    glBindTexture(GL_TEXTURE_2D, m_texture->Get());
    glActiveTexture(GL_TEXTURE1); // 1번 슬롯을 쓸거다 전달!
    glBindTexture(GL_TEXTURE_2D, m_texture2->Get());

    m_program->Use();
    glUniform1i(glGetUniformLocation(m_program->Get(), "tex"), 0);
    glUniform1i(glGetUniformLocation(m_program->Get(), "tex2"), 1);
    

    // 0.5배 축소후 z축으로 90도 회전하는 행렬
    auto transform = glm::rotate(
        glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)),
        glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f)
    );
        
    auto transformLoc = glGetUniformLocation(m_program->Get(), "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

    return true;
}

void Context::Render() 
{
    glClear(GL_COLOR_BUFFER_BIT);

    m_program->Use();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}