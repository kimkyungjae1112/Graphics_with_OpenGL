#include "shader.h"

ShaderUPtr Shader::CreateFromFile(const std::string& filename, GLenum shaderType) 
{
    ShaderUPtr shader = std::unique_ptr<Shader>(new Shader());
    if (!shader->LoadFile(filename, shaderType)) return nullptr;
    return std::move(shader);
}

Shader::~Shader()
{
    if(m_shader)
    {
        glDeleteShader(m_shader);
    }
}

bool Shader::LoadFile(const std::string& filename, GLenum shaderType) 
{
    auto result = LoadTextFile(filename);
    if (!result.has_value()) return false;
    
    // 여기서 로딩하는 텍스트는 쉐이더 코드
    // 쉐이더 코드의 크기를 알 수 없으므로 참조로 받자.
    auto& code = result.value(); // 코드
    const char* codePtr = code.c_str(); // 코드의 포인터
    int32_t codeLength = (int32_t)code.length(); // 코드 길이

    // create and compile shader
    // 쉐이더 생성 -> 쉐이더 번호가 나옴
    m_shader = glCreateShader(shaderType);

    // 쉐이더에 코드 넣기
    // 코드가 하나 들어가고, 코드의 포인터와 코드의 길이가 들어감
    glShaderSource(m_shader, 1, (const GLchar* const*)&codePtr, &codeLength);

    // 쉐이더를 컴파일 해줌.
    glCompileShader(m_shader);

    // check compile error
    int success = 0;

    // i = integer / v = vector or 인자로 포인터를 받음
    glGetShaderiv(m_shader, GL_COMPILE_STATUS, &success);
    if (!success) 
    {
        char infoLog[1024];
        glGetShaderInfoLog(m_shader, 1024, nullptr, infoLog);
        SPDLOG_ERROR("failed to compile shader: \"{}\"", filename);
        SPDLOG_ERROR("reason: {}", infoLog);
        return false;
    }

    return true;
}