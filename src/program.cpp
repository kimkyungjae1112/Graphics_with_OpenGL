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

void Program::Use() const
{
    // 해당 프로그램을 가지고 그림을 그리겠다.
    glUseProgram(m_program);
}

void Program::SetUniform(const std::string& name, int value) const 
{
    auto loc = glGetUniformLocation(m_program, name.c_str());
    glUniform1i(loc, value);
}
  
void Program::SetUniform(const std::string& name,const glm::mat4& value) const 
{
    auto loc = glGetUniformLocation(m_program, name.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}

bool Program::Link(const std::vector<ShaderPtr>& shaders) 
{
    // gl function 을 이용해서 program id 를 얻는다.
    m_program = glCreateProgram();

    // shader의 id를 가져와 program에 등록한다.
    for (auto& shader: shaders) glAttachShader(m_program, shader->Get());

    // vertex shader, fragment shader를 링크 -> 파이프라인을 만드는 것 같음.
    // 링크할 때 Qualifier(In / Out) 값이 같은지 확인한다.
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
