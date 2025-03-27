#include "common.h"
#include "shader.h"
#include <spdlog/spdlog.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void OnFramebufferSizeChange(GLFWwindow* Window, int Width, int Height) 
{
    SPDLOG_INFO("framebuffer size changed: ({} x {})", Width, Height);
    glViewport(0, 0, Width, Height);
}

void OnKeyEvent(GLFWwindow* Window, int Key, int Scancode, int Action, int Mods) 
{
    SPDLOG_INFO("Key: {}, Scancode: {}, Action: {}, Mods: {}{}{}",
        Key, Scancode,
        Action == GLFW_PRESS ? "Pressed" :
        Action == GLFW_RELEASE ? "Released" :
        Action == GLFW_REPEAT ? "Repeat" : "Unknown",
        Mods & GLFW_MOD_CONTROL ? "C" : "-",
        Mods & GLFW_MOD_SHIFT ? "S" : "-",
        Mods & GLFW_MOD_ALT ? "A" : "-");
    if (Key == GLFW_KEY_ESCAPE && Action == GLFW_PRESS) 
    {
        glfwSetWindowShouldClose(Window, true);
    }
}

void Render() 
{
    glClearColor(0.1f, 0.2f, 0.3f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

int main(int argc, const char** argv) 
{
    // SPDLOG_INFO 현재 시각, 어떤 파일에서 로그를 호출했는지, 몇 번째 라인인지 표시해주는 
    // 고마운 라이브러리
    SPDLOG_INFO("Start program");

    // glfw 라이브러리 초기화, 실패하면 에러 출력후 종료
    SPDLOG_INFO("Initialize glfw");
    if (!glfwInit()) 
    {
        const char* Description = nullptr;
        glfwGetError(&Description);
        SPDLOG_ERROR("failed to initialize glfw: {}", Description);
        return -1;
    }

    // 우리가 만드는 윈도우에 OpenGL을 어떻게 세팅할거다를 명시
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw 윈도우 생성, 실패하면 에러 출력후 종료
    SPDLOG_INFO("Create glfw window");
    GLFWwindow* Window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, nullptr, nullptr);
    if (!Window) 
    {
        SPDLOG_ERROR("failed to create glfw window");
        glfwTerminate();
        return -1;
    }
    // 어떤 윈도우에 그림을 그릴건지 결정
    // OpenGL Context Setting
    glfwMakeContextCurrent(Window);

    // glad를 활용한 OpenGL 함수 로딩
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
    {
        SPDLOG_ERROR("failed to initialize glad");
        glfwTerminate();
        return -1;
    }
    auto glVersion = glGetString(GL_VERSION);
    SPDLOG_INFO("OpenGL context version: {}", reinterpret_cast<const char*>(glVersion));
    // 이후 OpenGL 함수들을 사용할 수 있음
    // OpenGL 함수들은 상태를 설정하는 함수 / 상태를 사용하는 함수로 나눌 수 있다.

auto vertexShader = Shader::CreateFromFile("./shader/simple.vs", GL_VERTEX_SHADER);
auto fragmentShader = Shader::CreateFromFile("./shader/simple.fs", GL_FRAGMENT_SHADER);
SPDLOG_INFO("vertex shader id: {}", vertexShader->Get());
SPDLOG_INFO("fragment shader id: {}", fragmentShader->Get());

    OnFramebufferSizeChange(Window, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetWindowSizeCallback(Window, OnFramebufferSizeChange);
    glfwSetKeyCallback(Window, OnKeyEvent);


    // glfw 루프 실행, 윈도우 close 버튼을 누르면 정상 종료
    SPDLOG_INFO("Start main loop");
    while (!glfwWindowShouldClose(Window)) 
    {
        glfwPollEvents();
        Render();
        glfwSwapBuffers(Window);    
    }

    glfwTerminate();
    return 0;
}