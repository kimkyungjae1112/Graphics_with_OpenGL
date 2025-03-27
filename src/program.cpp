#include "program.h"

ProgramUPtr Program::Create(const std::vector<ShaderPtr>& shaders) 
{
    ProgramUPtr program = ProgramUPtr(new Program());
    if (!program->Link(shaders)) return nullptr;
    return std::move(program);
}

Program::~Program() 
{
    if (m_program) 
    {
        glDeleteProgram(m_program);
    }
}

bool Program::Link(const std::vector<ShaderPtr>& shaders) 
{
    // gl function 을 이용해서 program id 를 얻는다.
    m_program = glCreateProgram();

    // shader의 id를 가져와 program에 등록한다.
    for (auto& shader: shaders) glAttachShader(m_program, shader->Get());

    // vertex shader, fragment shader를 링크 -> 파이프라인을 만드는 것 같음.
    glLinkProgram(m_program);

    int success = 0;
    glGetProgramiv(m_program, GL_LINK_STATUS, &success);
    if (!success) 
    {
        char infoLog[1024];
        glGetProgramInfoLog(m_program, 1024, nullptr, infoLog);
        SPDLOG_ERROR("failed to link program: {}", infoLog);
        return false;
    }

    return true;
}
