#include <spdlog/spdlog.h>
#include <GLFW/glfw3.h>

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

    // glfw 윈도우 생성, 실패하면 에러 출력후 종료
    SPDLOG_INFO("Create glfw window");
    GLFWwindow* Window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, nullptr, nullptr);
    if (!Window) 
    {
        SPDLOG_ERROR("failed to create glfw window");
        glfwTerminate();
        return -1;
    }

    // glfw 루프 실행, 윈도우 close 버튼을 누르면 정상 종료
    SPDLOG_INFO("Start main loop");
    while (!glfwWindowShouldClose(Window)) 
    {
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}