// helper function to read text file into string
std::string read_file(std::string filename);

// PRE: shader source files, written in GLSL (OpenGL shading language)
// .vert - vertex shader
// .tesc - tessellation control shader
// .tese - tessellation evaluation shader
// .geom - geometry shader
// .frag - fragment shader
// .comp - compute shader
// POST: assemble shader program, return its name
GLuint load_shaders();