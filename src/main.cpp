#include "context.h"
#include <spdlog/spdlog.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void OnFramebufferSizeChange(GLFWwindow* Window, int Width, int Height) 
{
    SPDLOG_INFO("framebuffer size changed: ({} x {})", Width, Height);
    auto context = reinterpret_cast<Context*>(glfwGetWindowUserPointer(Window));
    context->Reshape(Width, Height);
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

void OnCursorPos(GLFWwindow* window, double x, double y) 
{
    auto context = reinterpret_cast<Context*>(glfwGetWindowUserPointer(window));
    context->MouseMove(x, y);
}

void OnMouseButton(GLFWwindow* window, int button, int action, int modifier) 
{
    auto context = (Context*)glfwGetWindowUserPointer(window);
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    context->MouseButton(button, action, x, y);
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

    ContextUPtr context = Context::Create();
    if (!context) 
    {
      SPDLOG_ERROR("failed to create context");
      glfwTerminate();
      return -1;
    }

    glfwSetWindowUserPointer(Window, context.get());

    OnFramebufferSizeChange(Window, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetWindowSizeCallback(Window, OnFramebufferSizeChange);
    glfwSetKeyCallback(Window, OnKeyEvent);
    glfwSetCursorPosCallback(Window, OnCursorPos);
    glfwSetMouseButtonCallback(Window, OnMouseButton);

    // glfw 루프 실행, 윈도우 close 버튼을 누르면 정상 종료
    SPDLOG_INFO("Start main loop");
    while (!glfwWindowShouldClose(Window)) 
    {
        glfwPollEvents();
        context->ProcessInput(Window);
        context->Render();
        glfwSwapBuffers(Window);    
    }

    // 우리가 만든 쉐이더, 프로그램 메모리를 미리 정리하는 것이 좋다.
    context.reset();

    glfwTerminate();
    return 0;
}