#include "shader.h"


std::string Shader::read_file(const char* filename)
{
    std::string content;
    std::ifstream ifs;

    ifs.open(filename);
    content.assign((std::istreambuf_iterator<char>(ifs)), 
                   (std::istreambuf_iterator<char>(   )) );
    
    return content;
}


GLuint Shader::compile_shader(const char* filename, GLuint shader_type)
{
    std::string source = std::move(read_file(filename));
    const GLchar *shader_source = source.c_str();
    GLuint shader;
    shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &shader_source, NULL);
    glCompileShader(shader);

    // check for shader compilation errors
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR: Compilation of " << filename << " failed.\n" << infoLog << std::endl;
    }

    return shader;
}


void Shader::activate()
{
    glUseProgram(ID);
}


void Shader::load_shaders(const char* vert, const char* frag)
{
    // compile individual GLSL files
    GLuint vertex_shader = compile_shader(vert, GL_VERTEX_SHADER);
    GLuint fragment_shader = compile_shader(frag, GL_FRAGMENT_SHADER);

    // attach shader objects to program object and link them
    ID = glCreateProgram();
    glAttachShader(ID, vertex_shader);
    glAttachShader(ID, fragment_shader);
    glLinkProgram(ID);

    // check for linking errors
    int success;
    char infoLog[512];
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR: Linking of shader program failed.\n" << infoLog << std::endl;
    }

    // shader objects no longer needed
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}