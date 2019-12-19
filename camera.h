#include<cassert>
#include<cmath>

#include "matrix.h"

typedef vec<2> vec2;
typedef vec<3> vec3;
typedef vec<4> vec4;
typedef mat<2> mat2;
typedef mat<3> mat3;
typedef mat<4> mat4;

class Camera
{
    public: 
    const float velocity;
    const float angular_velocity;
    vec3 position;
    float pitch;
    float yaw;
    float roll;
    float fov;

    public: 
    // default constructor, start at origin facing -z direction
    Camera(): 
    velocity {0.05f}, angular_velocity {0.05f}, position {0.f, 0.f, 0.f}, pitch {0.f}, yaw {0.f}, roll {0.f}, fov {pi/6}
    {}

    // custom starting point and orientation
    Camera(vec3 position, float pitch, float yaw, float roll):
    velocity {0.05f}, angular_velocity {0.05f}, position {position}, pitch {0.f}, yaw {0.f}, roll {0.f}, fov {pi/6}
    {}

    ~Camera(){}

    const vec3 front()
    {
        return vec3 {std::sin(yaw), std::sin(pitch), - std::cos(pitch) * std::cos(yaw)};
    }

    const vec3 right()
    {
        return vec3 {std::cos(yaw), 0.f, std::sin(yaw)};
    }

    void reset()
    {
        position = vec3 {0.f, 0.f, 0.f};
        pitch = 0.f;
        yaw = 0.f;
        roll = 0.f;
    }

    mat4 view_matrix()
    {
        mat4 translation = translation_matrix(- position);
        mat4 rotation = look_at(front(), right());
        return rotation * translation;
    }
};
