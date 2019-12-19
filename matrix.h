#ifndef MATRIX_H
#define MATRIX_H


#include <iostream>
#include <cassert>
#include <functional> // std::function
#include <initializer_list> // std::initializer_list<T>
#include <cmath>


static const float pi = 3.14159265358979323846;
static const float right_angle = pi / 2;


/* 
    vector class template 
*/
template <int N>
class vec
{
    private: 
    float values[N];


    private:
    // same operation on all elements
    void map(std::function<void(float&)> operation)
    {
        for (int i = 0; i < N ; ++i)
            operation(values[i]);
    }


    public: 
    // default constructor, zero vector
    vec(): values {0}
    {}

    // constructor with initializer list
    vec(std::initializer_list<float> list)
    : vec()
    {
        assert(list.size() <= N);
        std::copy(list.begin(), list.end(), values);
    }

    // copy constructor
    vec(const vec& v)
    {
        std::copy(v.begin(), v.end(), begin());
    }

    // assigment
    vec& operator=(const vec& v)
    {
        for (int i = 0; i < N; ++i) values[i] = v[i];
        return *this;
    }

    // default destructor, nothing to do
    ~vec(){}

    // pointer to first/one past last element
    float* begin()
    {
        return values;
    }
    float* end()
    {
        return begin() + N;
    }

    // const pointer to first/one past last element
    const float* begin() const
    {
        return values;
    }
    const float* end() const
    {
        return begin() + N;
    }

    // access element
    float& operator[](int i)
    {
        assert(0 <= i && i < N);
        return values[i];
    }

    // access (read) element for const vec
    const float& operator[](int i) const
    {
        assert(0 <= i && i < N);
        return values[i];
    }

    // scalar addition
    const vec operator+(float f) const
    {
        vec result (*this);
        result.map([f](float& element){element += f;});
        return result;
    }

    vec& operator+=(float f)
    {
        map([f](float& element){element += f;});
        return *this; 
    }

    // scalar multiplication
    const vec operator*(float f) const
    {
        vec result (*this);
        result.map([f](float& element){element *= f;});
        return result;
    }

    // scalar division
    const vec operator/(float f) const
    {
        assert(f != 0);
        vec result (*this);
        result.map([f](float& element){element /= f;});
        return result;
    }

    vec& operator*=(float f)
    {
        map([f](float& element){element *= f;});
        return *this; 
    }

    // vector addition
    const vec operator+(const vec& v) const
    {
        vec result (*this);
        for (int i = 0; i < N; ++i) result[i] = values[i] + v[i];
        return result;
    }

    // vector subtraction
    const vec operator-(const vec& v) const
    {   
        vec result (*this);
        for (int i = 0; i < N; ++i) result[i] -= v[i];
        return result;
    }

    // negation
    const vec operator-() const{
        vec result (*this);
        result.map([](float& element){element = - element;});
        return result;
    }

    vec& operator+=(const vec& v)
    {
        for (int i = 0; i < N; ++i) values[i] += v[i];
        return *this;
    }

    // dot product
    float dot(const vec& v)
    {
        float result;
        for (int i = 0; i < N; ++i) result += values[i] * v[i];
        return result;
    }

    bool operator==(const vec& v) const
    {
        bool equal = true;
        for (int i = 0; i < N; ++i)
            if (values[i] != v[i])
                equal = false;
        return equal;
    }

    bool operator!=(const vec& v) const
    {
        return !(*this == v);
    }

    float length() const
    {
        float sum = 0;
        for (int i = 0; i < N; ++i) sum += values[i] * values[i];
        return std::sqrt(sum);
    }
};






/* 
    square matrix class template 
    NOTE: column-major implementation, since GLSL uses column-major
*/
template<int M>
class mat
{
    public:
    float values[M*M];


    private: 
    // private class for accessing elements with row-major coordinates
    struct row
    {
        float* begin;

        row(float* b)
        : begin (b)
        {}

        float& operator[](int j)
        {
            assert(0 <= j && j < M);
            return *(begin + M * j);
        }
    };

    // used for const mat
    struct const_row
    {
        const float* begin;

        const_row(const float* b)
        : begin (b)
        {}

        float operator[](int j)
        {
            assert(0 <= j && j < M);
            return *(begin + M * j);
        }
    };


    private:
    // same operation on all elements
    void map(std::function<void(float&)> operation)
    {
        for (int i = 0; i < M*M ; ++i)
            operation(values[i]);
    }
    

    public:
    // default constructor, zero matrix
    mat(): values{0} 
    {}

    // constructor, multiple of identity matrix
    mat(float f): mat()
    {
        for (int i = 0; i < M; ++i) values[i+M*i] = f;
    }

    // constructor with initializer list
    // if not enough values from the list, remaining elements are set to 0
    // NOTE: also in column-major order
    mat(std::initializer_list<float> list): mat()
    {
        assert(list.size() <= M*M);
        float* it = list.begin();
        for (int i = 0; i < list.size(); ++i){
            values[M*(i%M) + i/M] = *it;
            ++it;
        }
    }
    
    // copy constructor
    mat(const mat& m)
    {
        std::copy(m.begin(), m.end(), begin());
    }

    // assigment
    mat& operator=(const mat& m)
    {
        std::copy(m.begin(), m.end(), begin());
        return *this;
    }

    // default destructor, nothing to do
    ~mat(){}

    // pointer to first / one past last element
    float* begin()
    {
        return values;
    }

    float* end()
    {
        return begin() + M*M;
    }

    // const pointer to first / one past last element 
    const float* begin() const
    {
        return values;
    }

    const float* end() const
    {
        return begin() + M*M;
    }

    // row-major style element access
    row operator[](int i)
    {
        assert(0 <= i && i < M);
        return row(begin() + i);
    }

    // element access for const mat
    const_row operator[](int i) const
    {
        assert(0 <= i && i < M);
        return const_row(begin() + i);
    }

    // scalar addition
    const mat operator+(float f) const
    {
        mat result (*this);
        result.map([f](float& element){element += f;});
        return result;
    }

    mat& operator+=(float f)
    {
        map([f](float& element){element += f;});
        return *this;
    }

    // matrix addition
    const mat operator+(const mat& m) const
    {
        mat result (*this);
        for (int i = 0; i < M*M; ++i)
            result.values[i] = values[i] + m.values[i];
        return result;
    }

    // matrix multiplication (naive)
    // TODO: implement Strassen algorithm? 
    const mat operator*(const mat& m) const
    {
        mat result;
        for (int i = 0; i < M; ++i)
            for (int j = 0; j < M; ++j)
                for (int k = 0; k < M; ++k)
                    result[i][j] +=  (*this)[i][k] * m[k][j];
        return result;
    }
};






/*
    Non-member functions / operations
*/
// print matrix
template<int M>
std::ostream& operator<< (std::ostream& o, const mat<M>& m)
{
    for (int i = 0; i < M; ++i){
        for (int j = 0; j < M; ++j) o << m[i][j] << ' ';
        o << std::endl;
    }
    return o;
}

// print vector
template <int N>
std::ostream& operator<< (std::ostream& o, const vec<N>& v)
{
    for (int i = 0; i < N; ++i) o << v[i] << ' ';
    o << std::endl;
    return o;
}

// matrix-vector multiplication
template <int N>
const vec<N> operator*(const mat<N>& m, const vec<N>& v)
{
    vec<N> result;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            result[i] += m[i][j] * v[j];
    return result;
}

// PRE: vector must be nonzero
// normalize a vector
template <int N>
const vec<N> normalize(const vec<N>& v)
{
    float length = v.length();
    assert(length > 0);
    return v / length;
}

// cross product
const vec<3> cross(const vec<3>& a, const vec<3>& b)
{
    vec<3> result;
    result[0] = a[1]*b[2] - a[2]*b[1];
    result[1] = a[2]*b[0] - a[0]*b[2];
    result[2] = a[0]*b[1] - a[1]*b[0];
    return result;
}






/*
    Linear transformations
*/
// naive 3D translation matrix
const mat<4> translation_matrix(const vec<3>& v)
{
    mat<4> m (1.f);
    for (int i = 0; i < 3; ++i) m[i][3] = v[i];
    return m;
}
const mat<4> translation_matrix(float x, float y, float z)
{
    return translation_matrix(vec<3> {x,y,z});
}

// apply translation on an existing linear transformation
const mat<4> translate(const mat<4>& m, float x, float y, float z)
{
    return translation_matrix(x, y, z) * m;
}
const mat<4> translate(const mat<4>& m, const vec<3>& v)
{
    return translation_matrix(v) * m;
}

// PRE: angle in radian (instead of degree)
// PRE: axis must be unit vector unless it is on x-, y-, or z-axis
// naive rotation matrix around an axis through the origin
const mat<4> rotation_matrix(float angle, const vec<3>& axis)
{
    mat<4> result (1.f);
    float cos = std::cos(angle);
    float sin = std::sin(angle);

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            result[i][j] = axis[i] * axis[j] * (1 - cos);
    
    for (int i = 0; i < 3; ++i) result[i][i] += cos;
    result[0][1] -= axis[2] * sin;
    result[0][2] += axis[1] * sin;
    result[1][0] += axis[2] * sin;
    result[1][2] -= axis[0] * sin;
    result[2][0] -= axis[1] * sin;
    result[2][1] += axis[0] * sin;

    return result;
}
const mat<4> rotation_matrix(float angle, float ux, float uy, float uz)
{
    return rotation_matrix(angle, vec<3> {ux, uy, uz});
}

// apply rotation on an existing linear transformation
const mat<4> rotate(const mat<4>& m, float angle, vec<3> axis)
{
    return rotation_matrix(angle, axis) * m;
}
const mat<4> rotate(const mat<4>& m, float angle, float x, float y, float z)
{
    return rotation_matrix(angle, x, y, z) * m;
}

// naive resize_matrix
const mat<4> scale_matrix(const vec<3>& v)
{
    mat<4> m (1.f);
    for (int i = 0; i < 3; ++i) m[i][i] = v[i];
    return m;
}
const mat<4> scale_matrix(float sx, float sy, float sz)
{
    return scale_matrix(vec<3> {sx, sy, sz});
}

// apply resize on an existing linear transformation
const mat<4> scale(const mat<4>& m, const vec<3>& v)
{
    return scale_matrix(v) * m;
}
const mat<4> scale(const mat<4>& m, float sx, float sy, float sz)
{
    return scale_matrix(sx, sy, sz) * m;
}

// PRE: 0 < znear < zfar (negation of the actual z coordinates)
// PRE: theta - field of view in radian, aspect - ratio width/height (of screen)
// POST: persepctive matrix (given camera at origin, -z as front and +y as top)
const mat<4> perspective_matrix(float znear, float zfar, float theta, float aspect)
{
    assert(0 < znear && znear < zfar);
    assert(0 < theta && theta < pi / 2);
    assert(0 < aspect);
    mat<4> result;
    float f = 1.f / std::tan(theta);
    result[0][0] = f / aspect;
    result[1][1] = f;
    result[2][2] = (znear + zfar) / (znear - zfar);
    result[2][3] = (2.f * znear * zfar) / (znear - zfar);
    result[3][2] = -1.f;
    return result;
}


// PRE: front and right must be perpendicular and normalized
const mat<4> look_at(const vec<3> front, const vec<3> right)
{
    vec<3> up = cross(right, front);
    mat<4> result (1.f);
    for (int i = 0; i < 3; ++i){
        result[0][i] = right[i];
        result[1][i] = up[i];
        result[2][i] = - front[i];
    }

    return result;
}


// PRE: camera position, target position, up direction of world space
// PRE: cannot look directly down/up, i.e. target - position cannot be parallel with world_up
// POST: corresponding camera view matrix
const mat<4> camera_view_matrix(const vec<3>& position, const vec<3>& target, const vec<3>& world_up)
{
    assert(target != position); // otherwise direction is meaning less
    vec<3> direction = normalize(target - position);
    vec<3> right = normalize(cross(direction, world_up));

    return look_at(direction, right) * translation_matrix(- position);
}

// TODO? shearing
// TODO? reflection
// TODO? more complex transformations





#endif