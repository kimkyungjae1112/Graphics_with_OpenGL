#ifndef __SHADER_H__
#define __SHADER_H__

#include "common.h"

// 쉐이더 코드를 읽어와서 컴파일 후 사용할 수 있게 관리해주는 클래스
CLASS_PTR(Shader);
class Shader 
{
public:
  static ShaderUPtr CreateFromFile(const std::string& filename, GLenum shaderType);

  ~Shader();
  inline uint32_t Get() const { return m_shader; }    

private:
  Shader() {}
  bool LoadFile(const std::string& filename, GLenum shaderType);
  uint32_t m_shader { 0 }; //OpenGL Shader Object ID
};

#endif // __SHADER_H__