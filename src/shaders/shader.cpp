#include "shader.hpp"

using namespace std;

std::string LoadShaderSource(const std::string& filePath) {
    std::ifstream file(filePath);
    std::stringstream buffer;

    if (!file.is_open()) {
        std::cerr << "Failed to open shader file: " << filePath << std::endl;
        return "";
    }

    buffer << file.rdbuf();
    return buffer.str();
}

void checkCompileErrors(unsigned int shader, const std::string& type) {
    int success;
    char infoLog[1024];
    
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type 
                      << "\n" << infoLog << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR\n" << infoLog << std::endl;
        }
    }
}

unsigned int createShader() {
    std::string vertexShaderPath = "src/shaders/vertex_shader.glsl";
    std::string fragmentShaderPath = "src/shaders/frag_shader.glsl";

    std::string vertexShaderCode = LoadShaderSource(vertexShaderPath);
    std::string fragmentShaderCode = LoadShaderSource(fragmentShaderPath);

    const char* vertexShaderSource = vertexShaderCode.c_str();
    const char* fragmentShaderSource = fragmentShaderCode.c_str();

    cout << "Vertex Shader" << endl;
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    cout << "Fragment Shader" << endl;
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    cout << "Link Shaders" << endl;
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glCompileShader(vertexShader);
    checkCompileErrors(vertexShader, "VERTEX");

    glCompileShader(fragmentShader);
    checkCompileErrors(fragmentShader, "FRAGMENT");

    glLinkProgram(shaderProgram);
    checkCompileErrors(shaderProgram, "PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return shaderProgram;
}