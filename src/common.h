#ifndef __COMMON_H__
#define __COMMON_H__
// #pragma once 는 윈도우 프로그램 전용이구나.

#include <memory>
#include <string>
#include <optional>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <spdlog/spdlog.h>

std::optional<std::string> LoadTextFile(const std::string& Filename);

#endif // __COMMON_H__