#include <iostream>
#include <fstream>
#include <string>

#include <GL/glew.h> // for loading openGL functions

#include "shader.h"


// helper function to read text file into string
std::string read_file(std::string filename)
{
    std::ifstream ifs (filename);
    std::string content;
    content.assign((std::istreambuf_iterator<char>(ifs)), 
                   (std::istreambuf_iterator<char>(   )) );

    return content;
}


GLuint load_shaders()
{
    // vertex shader
    std::string str = std::move(read_file("triangle.vert"));
    const GLchar *vertex_shader_source = str.c_str();
    GLuint vertex_shader;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment shader
    str = std::move(read_file("triangle.frag"));
    const GLchar *fragment_shader_source = str.c_str();
    GLuint fragment_shader;
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);
    // check for shader compile errors
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // attach shader objects to a program object and link them
    GLuint shader_program;
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    // check for linking errors
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // shader objects no longer needed
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return shader_program;
}