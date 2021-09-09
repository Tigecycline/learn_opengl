#ifndef CAMERA_H
#define CAMERA_H


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
    private: 
    const float velocity;
    const float angular_velocity;
    vec3 position;
    float yaw, pitch; 
    vec3 front;
    vec3 right;
    vec3 up;


    public: 
    // default constructor, start upright at origin facing -z direction
    Camera(): 
    velocity {4.f}, angular_velocity {1.f}, position {0.f, 0.f, 0.f}, yaw {pi}, pitch {0.f}
    {
        update_basis();
    }

    private: 
    void update_basis()
    {
        float sin_yaw = std::sin(yaw);
        float cos_yaw = std::cos(yaw);
        front = vec3 {sin_yaw * std::cos(pitch), std::sin(pitch), cos_yaw * std::cos(pitch)};
        right = vec3 {-cos_yaw, 0.f, sin_yaw};
        up = cross(right, front);
    }

    public:
    // manoeuvering functions
    void move_forward(float time = 1/60.f)
    {
        position += front * velocity * time;
    }
    void move_backward(float time = 1/60.f)
    {
        position -= front * velocity * time;
    }
    void move_right(float time = 1/60.f)
    {
        position += right * velocity * time;
    }
    void move_left(float time = 1/60.f)
    {
        position -= right * velocity * time;
    }
    void move_up(float time = 1/60.f)
    {
        position += up * velocity * time;
    }
    void move_down(float time = 1/60.f)
    {
        position -= up * velocity * time;
    }
    void ascend(float time = 1/60.f)
    {
        position[1] += velocity * time;
    }
    void descend(float time = 1/60.f)
    {
        position[1] -= velocity * time;
    }

    void rotate(float delta_x, float delta_y, float time = 1/60.f)
    {
        // update yaw and keep it between 0 and 2pi
        yaw = std::fmod(yaw + delta_x * angular_velocity * time, 2*pi);
        // update pitch and keep it between -pi/2 and +pi/2
        pitch += delta_y * angular_velocity * time;
        if (pitch >= pi/2) pitch = pi/2;
        if (pitch <= -pi/2) pitch = -pi/2;
        // update the direction vectors
        update_basis();
    }

    void reset()
    {
        position = vec3 {0.f, 0.f, 0.f};
        yaw = pi;
        pitch = 0.f;
        update_basis();
    }

    const mat4 view_matrix() const
    {
        return look_at(front, right, up) * translation_matrix(- position);
    }
};




class PerspectiveCamera: public Camera{
    private: 
    const float znear;
    const float zfar;
    const float aspect;
    float fov;

    public: 
    PerspectiveCamera()
    : znear{0.1f}, zfar{100.f}, aspect {16/9}, fov{pi/6}
    {}

    PerspectiveCamera(float znear, float zfar, float fov, float aspect)
    : znear{znear}, zfar{zfar}, aspect {aspect}, fov{fov}
    {}

    public: 
    const mat4 view_projection_matrix() const
    {
        return perspective_matrix(znear, zfar, fov, aspect) * view_matrix();
    }

    void zoom(float offset)
    {
        float new_fov = fov - offset;
        if (new_fov > pi/4 || new_fov < 0.1f) return;
        fov = new_fov;
    }

    void reset()
    {
        Camera::reset();
        fov = pi/6;
    }
};




class OrthographicCamera: public Camera{
    private: 
    const float znear;
    const float zfar;
    const float height;
    const float aspect;

    public: 
    OrthographicCamera()
    : znear{0.1f}, zfar{100.f}, height{1.f}, aspect{16/9.f}
    {}

    OrthographicCamera(float znear, float zfar, float height, float aspect)
    : znear{znear}, zfar{zfar}, height{height}, aspect{aspect}
    {}

    public: 
    const mat4 view_projection_matrix()
    {
        return orthographic_matrix(znear, zfar, height*aspect, height) * view_matrix();
    }
};

#endif