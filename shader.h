#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <string>

#include <GL/glew.h> // load openGL functions

class Shader{
    private:
    GLuint ID;


    public:
    // constructor with source file names
    Shader(const std::string vert, const std::string frag)
    {
        load_shaders(vert,frag);
    }

    // PRE: shader source files
    // .vert - vertex shader
    // .tesc - tessellation control shader
    // .tese - tessellation evaluation shader
    // .geom - geometry shader
    // .frag - fragment shader
    // .comp - compute shader
    // POST: name of assembled shader program
    void load_shaders(const std::string vert, const std::string frag);

    // POST: activate this shader program
    void activate();

    // pass uniforms to the shader program
    void pass_int(const std::string name, int value)
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    void pass_float(const std::string name, float value)
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

    void pass_bool(const std::string name, bool value)
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (GLint) value);
    }

    void pass_mat4(const std::string name, float* begin)
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, begin);
    }

    GLuint uniform_location(std::string name)
    {
        return glGetUniformLocation(ID, name.c_str());
    }


    private:
    // PRE: name of a GLSL source file
    // POST: read text file into string
    std::string read_file(const std::string filename);

    // PRE: shader source file, in GLSL (OpenGL shading language)
    // POST: name of compiled shader object
    GLuint compile_shader(const std::string filename, GLuint shader_type);
};


#endif