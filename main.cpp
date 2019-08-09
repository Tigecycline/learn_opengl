#include <iostream>

// #define GLEW_STATIC // link to GLEW statically
#include <GL/glew.h> // for loading openGL functions, must be included before GLFW
#include <GLFW/glfw3.h> // for window creation

#include "shader.h" // for shader plumbing


// global variables
GLuint vao; // vertex array object
GLuint vbo; // vertex buffer object


void process_input(GLFWwindow* window){
     // close window when pressing esc
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}


// set up data for later use in program, e.g. vertices, images, shaders
void initialize()
{   
    // create and bind vertex array object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // vertices for rendering a single triangle
    float vertices[][3] = {
        {-0.5f, -0.5f, 0.0f}, // x, y , z-coordinates of first point
        {0.5f, -0.5f, 0.0f}, // second point
        {0.0f, 0.5f, 0.0f} // third point
    };

    // create and bind vertex buffer object
    glGenBuffers(1, &vbo); 
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // copy vertex data to buffer
    // GL_ARRAY_BUFFER is bound to vbo so data will be copied to vbo
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 

    // load shaders
    GLuint shader_program = load_shaders();
    glUseProgram(shader_program);

    // instructions on how to interpret vertex data
    glVertexAttribPointer(0, // index of vertex attribute to be configured 
                          3, // size of vertex attribute (#values for each vertex)
                          GL_FLOAT, // type of data in vertex attribute
                          GL_FALSE, // if the data is to be normalised
                          3*sizeof(float), // stride
                          (void*)0 // offset, where the position data begins in buffer
                          );
    glEnableVertexAttribArray(0);
}


// rendering
void display()
{
    glClearColor(0.2, 0.3, 0.3, 1.0); // set clear colour
    glClear(GL_COLOR_BUFFER_BIT); // clear window with predefined colour
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}


int main()
{
    // initialise and configure GLFW library
    glfwInit();
    /* configure options for GLFW, leads to error 
    // use opengl version 3.3 (check installed version by glxinfo | grep "OpenGL version")
    glfwWindowHint(GLFW_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_VERSION_MINOR, 3);
    // use only the core profile -> results in failure in window creation, why?
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    */

    // create window
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    // check if window is successfully created
    if (window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return 1;
    }

    // make OpenGL context active
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE; // force GLEW to use a modern OpenGL method for checking if a function is available
    glewInit(); // initialise GLEW, must be done after OpenGL context is created

    initialize();

    // main loop
    while (!glfwWindowShouldClose(window)){
        process_input(window); 
        display();  
        glfwSwapBuffers(window); // swap back and front buffer after drawing
        glfwPollEvents(); // retrieve window events
    }
    
    glfwTerminate();

    return 0;
}