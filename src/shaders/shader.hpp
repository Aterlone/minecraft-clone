#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

std::string LoadShaderSource(const std::string& filePath);
void checkCompileErrors(unsigned int shader, const std::string& type);
unsigned int createShader();

#endif