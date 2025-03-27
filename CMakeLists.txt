# 우리 소스 코드 관리용 파일
cmake_minimum_required(VERSION 3.13)

set(PROJECT_NAME Graphics)
set(CMAKE_CXX_STANDARD 17)

set(WINDOW_NAME "Graphics with GLFW")
set(WINDOW_WIDTH 960)
set(WINDOW_HEIGHT 540)

project(${PROJECT_NAME})
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/common.cpp src/common.h
    src/shader.cpp src/shader.h
    src/program.cpp src/program.h
    src/context.cpp src/context.h
    src/buffer.cpp src/buffer.h
    src/vertex_layout.cpp src/vertex_layout.h
)

include(Dependency.cmake)

# 우리 프로젝트에 include / lib 관련 옵션 추가
# 실행 파일 이름이 Target / 여기선 PROJECT_NAME/src/소스파일들
# 링킹 과정을 직접 해주는거네
target_include_directories(${PROJECT_NAME} PUBLIC ${DEP_INCLUDE_DIR})
target_link_directories(${PROJECT_NAME} PUBLIC ${DEP_LIB_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${DEP_LIBS})

target_compile_definitions(${PROJECT_NAME} PUBLIC
    WINDOW_NAME="${WINDOW_NAME}"
    WINDOW_WIDTH=${WINDOW_WIDTH}
    WINDOW_HEIGHT=${WINDOW_HEIGHT}
    )

# Dependency들이 먼저 build 될 수 있게 관계 설정
add_dependencies(${PROJECT_NAME} ${DEP_LIST})