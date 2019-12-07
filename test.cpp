#include <iostream>

#include "matrix.h"


typedef mat<4> mat4;
typedef mat<3> mat3;
typedef vec<3> vec3;
typedef vec<4> vec4;


int main()
{
    vec3 position {3.f, 0.f, 0.f};
    vec3 world_up {0.f, 1.f, 0.f};
    vec3 target {0.f, 0.f, 0.f};

    vec<3> direction = normalize(target - position);
    vec<3> right = normalize(cross(direction, world_up));
    vec<3> up = cross(right, direction);
    std::cout << right << std::endl;
    std::cout << up << std::endl;
    std::cout << direction << std::endl;
    
    mat<4> rotation (1.f);
    for (int i = 0; i < 3; ++i){
        rotation[0][i] = right[i];
        rotation[1][i] = up[i];
        rotation[2][i] = - direction[i];
    }
    std::cout << rotation << std::endl;

    mat<4> translation = translation_matrix(- position);
    std::cout << translation << std::endl;

    std::cout << camera_view_matrix(position, target, world_up);
}