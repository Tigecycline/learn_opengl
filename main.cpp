#include <iostream>
#include <cmath>
#include <random>

// #define GLEW_STATIC // link to GLEW statically (why?)
#include <GL/glew.h> // loads pointers to OpenGL functions at runtime, must be included before GLFW
#include <GLFW/glfw3.h> // API for creating windows, contexts and surfaces, receiving input and events

#include "shader.h" // compile and link shader programs
#include "stb_image.h" // load images and textures
#include "matrix.h" // useful classes vec<N> and mat<M>
#include "camera.h" // camera object

// aliases for class templates in "matrix.h"
typedef vec<2> vec2;
typedef vec<3> vec3;
typedef vec<4> vec4;
typedef mat<2> mat2;
typedef mat<3> mat3;
typedef mat<4> mat4;

// global variables
GLuint vao; // vertex array object, store state configurations
GLuint vbo, vbo2; // vertex buffer object
GLuint ebo; // element buffer object

// camera attributes
float fieldOfView = pi / 6;
float aspectRatio = 16 / 9.f;
// camera object
PerspectiveCamera cam(0.1f, 100.f, fieldOfView, aspectRatio); 

// Frame time track
float current_frame = 0.f;
float last_frame = 0.f;
float frame_time;


// process inputs, determine if loop is to end
void process(GLFWwindow* window){
     // close window when pressing esc
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    // switch to wireframe mode
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) 
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // GL_FILL is default
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) 
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    // camera control: translation
    if (glfwGetKey(window, GLFW_KEY_W))
        cam.move_forward(frame_time);
    if (glfwGetKey(window, GLFW_KEY_S))
        cam.move_backward(frame_time);
    if (glfwGetKey(window, GLFW_KEY_D))
        cam.move_right(frame_time);
    if (glfwGetKey(window, GLFW_KEY_A))
        cam.move_left(frame_time);
    if (glfwGetKey(window, GLFW_KEY_SPACE))
        cam.ascend(frame_time);
    if (glfwGetKey(window, GLFW_KEY_C))
        cam.descend(frame_time);
    
    // camera control: rotation
    if (glfwGetKey(window, GLFW_KEY_LEFT))
        cam.rotate(1.f, 0.f, frame_time);
    if (glfwGetKey(window, GLFW_KEY_RIGHT))
        cam.rotate(-1.f, 0.f, frame_time);
    if (glfwGetKey(window, GLFW_KEY_UP))
        cam.rotate(0.f, 1.f, frame_time);
    if (glfwGetKey(window, GLFW_KEY_DOWN))
        cam.rotate(0.f, -1.f, frame_time);
    
    // reset camera position and orientation 
    if (glfwGetKey(window, GLFW_KEY_R))
        cam.reset();
}




// initialisation, set up environment
void initialise()
{
    // create and vertex array object
    glGenVertexArrays(1, &vao);
    // generate buffer objects
    glGenBuffers(1, &vbo); 
    glGenBuffers(1, &ebo); 

    // set clear colour
    glClearColor(0., 0., 0., 1.0); 

    // enable depth testing
    glEnable(GL_DEPTH_TEST);
}




void draw_elements()
{
    // vertices & colours to be used
    float vertices[] = { 
        // format: x, y , z, r, g, b, tex_x, tex_y
        -0.5f, -0.5f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, // bottom left
        0.5f, -0.5f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.f, // bottom right
        0.5f, 0.5f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f, // top right
        -0.5f, 0.5f, 0.f, 0.5f, 0.5f, 0.f, 0.f, 1.f, // top left
    };

    // each three vertices constitute a triangle
    unsigned int indices[] = {
        0, 1, 2, 
        0, 2, 3, 
    };

    // bind buffer objects
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        
    // copy data into currently bound buffer objects
    // static / dynamic / stream draw: indicates how often the data is changed
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // instructions on how to interpret vertex data, will be stored in vao
    glVertexAttribPointer(0, // index of vertex attribute to be configured, corresponds to location in vertex shader 
                          3, // size of each vertex attribute (number of components)
                          GL_FLOAT, // type of data in vertex attribute
                          GL_FALSE, // if the data is to be normalised
                          8*sizeof(float), // stride (size of data for a single vertex), determined automatically if set to 0
                          (void*) 0 // offset, where the position data begins in buffer
                          );
    glEnableVertexAttribArray(0); // enables the vertex attribute with the specified index
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*) (3*sizeof(float)) );
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*) (6*sizeof(float)) );
    glEnableVertexAttribArray(2);

    // glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // (mode, size, index type, offset)

    // unbind current buffers, often not necessary
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}




void draw_array()
{
    // vertices & texture coordinates
    float vertices[] = {
        // x, y, z, tex_x, tex_y
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*) (3*sizeof(float)));
    glEnableVertexAttribArray(2);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}




// callback function that gets executed each time the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    if ((float) width / height > aspectRatio)
        glViewport((width - height*aspectRatio)/2, 0, height*aspectRatio, height); 
    else 
        glViewport(0, (height - width/aspectRatio)/2, width, width / aspectRatio);
}



float mouse_x;
float mouse_y;
bool new_session = true;
const float mouse_sensitivity = 0.125f;
void mouse_callback(GLFWwindow* window, double new_x, double new_y)
{
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1)){
        if (new_session){
            mouse_x = new_x;
            mouse_y = new_y;
            new_session = false;
        } else {
            float offset_x = (new_x - mouse_x) * mouse_sensitivity;
            float offset_y = (new_y - mouse_y) * mouse_sensitivity;
            mouse_x = new_x;
            mouse_y = new_y;
            cam.rotate(offset_x, offset_y, frame_time);
        }
    } else {
        new_session = true;
    }
}


void scroll_callback(GLFWwindow* window, double offset_x, double offset_y)
{
    cam.zoom((float) offset_y * mouse_sensitivity);
}




int main()
{
    // initialise and configure GLFW library
    glfwInit();
    /* 
    // configure options for GLFW, leads to failure, cause unknown 
    // use opengl version 3.3 (check installed version by glxinfo | grep "OpenGL version")
    // It seems later versions such as 4.6 work as well
    glfwWindowHint(GLFW_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_VERSION_MINOR, 3);
    // use only the core profile -> results in failure in window creation, why?
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    */

    // create window
    const int screen_height = 720;
    GLFWwindow* window = glfwCreateWindow(screen_height * aspectRatio, screen_height, "TestOpenGL", NULL, NULL);
    // check if window is successfully created
    if (window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return 1;
    }

    // make OpenGL context active
    glfwMakeContextCurrent(window);

    // settings & callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // callback for window resizing
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // force GLEW to use a modern OpenGL method for checking if a function is available (only needed for GLEW 1.13 or earlier) 
    // glewExperimental = GL_TRUE; 

    glewInit(); // initialise GLEW, must be done after OpenGL context is created

    // maximal number of attributes that can be declared in vertex shader (guaranteed to be at least 16)
    // int nrAttributes;
    // glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    // std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

    // set up environment
    initialise();

    // load shader program
    Shader shader_program ("triangle.vert", "triangle.frag");
    shader_program.activate();

    // load textures
    GLuint texture1, texture2; // texture objects
    int tex_width, tex_height, num_channels;
    stbi_set_flip_vertically_on_load(true); // flip y coordinates when loading (bottom = 0, top = 1)
    // first texture
    glActiveTexture(GL_TEXTURE0); // activate texture unit (0 is activated by default)
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // GL_REPEAT is default
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image file
    unsigned char *data = stbi_load("./texture/rusty_dirt.jpg", &tex_width, &tex_height, &num_channels, 0);
    if (data){
        // generate actual texture 
        glTexImage2D(GL_TEXTURE_2D, // texture target
                    0, // mipmap level, used when manually setting mipmap levels
                    GL_RGB, // format of the texture
                    tex_width, tex_height, // size of the texture
                    0, // leave it 0 for now
                    GL_RGB, // format of source image, JPEG has three channels, PNG has four
                    GL_UNSIGNED_BYTE, // data type of source image
                    data // source image
                    );
        glGenerateMipmap(GL_TEXTURE_2D); // generate mipmaps for the texture currently bound to the target
    } else {
        std::cout << "ERROR: Failed to load texture." << std::endl;
    }
    stbi_image_free(data); // free the memory anyway to avoid memory leakage (Does delete[] also work?)

    // second texture, same procedure
    glActiveTexture(GL_TEXTURE1);
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
    data = stbi_load("./texture/barque.jpg", &tex_width, &tex_height, &num_channels, 0); 
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); 
        glGenerateMipmap(GL_TEXTURE_2D); 
    } else {
        std::cout << "ERROR: Failed to load texture." << std::endl; 
    }
    stbi_image_free(data);
    

    // pass texture locations to shader program as uniforms
    // N.B. pass the index of the texture unit, not the texture object
    shader_program.pass_int("texture1", 0); 
    shader_program.pass_int("texture2", 1); 

    // bind texture objects, not needed now
    // glActiveTexture(GL_TEXTURE1); // activate a texture unit (GL_TEXTURE0 is activated by default)
    // glBindTexture(GL_TEXTURE_2D, texture1); // bind corresponding texture object
    // glActiveTexture(GL_TEXTURE2);
    // glBindTexture(GL_TEXTURE_2D, texture2);

    vec3 positions[] = {
        vec3 { -2.0f,  0.0f, -2.0f},
        vec3 { 2.0f,  5.0f, -15.0f}, 
        vec3 {-1.5f, -2.2f, -2.5f},  
        vec3 { 0.0f,  0.0f, -12.3f},  
        vec3 { 2.4f, -0.4f, -3.5f},  
        vec3 {-1.7f,  3.0f, -7.5f},  
        vec3 { 2.3f, -2.0f, -2.5f},  
        vec3 { 3.5f,  2.0f, -2.5f}, 
        vec3 { 1.5f,  0.2f, -1.5f}, 
        vec3 {-1.3f,  2.0f, -1.5f}
    };

    vec3 axes[10];

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.f, 2*pi);

    for (int i = 0; i < 10; ++i) axes[i] = vec3 {dis(gen), dis(gen), dis(gen)};
    

    // main loop
    while (!glfwWindowShouldClose(window)){
        // Keep track of frame time
        current_frame = glfwGetTime(); 
        frame_time = current_frame - last_frame;
        last_frame = current_frame;
        
        process(window); 

        // clear window with predefined colour, clear depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

        // bind vertex array object
        // glBindVertexArray(vao);

        for (int i = 0; i < 10; ++i){
            
            mat4 model_view_projection = cam.view_projection_matrix() * translation_matrix(positions[i]) * rotation_matrix(glfwGetTime(), axes[i]); // model space -> world space
            shader_program.pass_mat4("modelviewprojection", model_view_projection.begin());

            draw_array();
        }

        glfwSwapBuffers(window); // swap back and front buffer after drawing
        glfwPollEvents(); // retrieve window events (e.g. key press)
    }
    
    glfwTerminate();

    return 0;
}