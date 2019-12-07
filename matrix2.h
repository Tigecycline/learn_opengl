#include <iostream>
#include <cassert>
#include <functional> // std::function
#include <initializer_list> // std::initializer_list<T>
#include <cmath>

template<int M>
class mat
{
    private:
    float values[M*M];

    public: 
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
    
    public:
    // default constructor, zero matrix
    mat(): values{0} 
    {}

    // constructor, multiple of identity matrix
    mat(float f): mat()
    {
        for (int i = 0; i < M; ++i) values[i+M*i] = f;
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

    // default destructor
    ~mat(){}

    // pointer to first / one past last element
    float* begin()
    {
        return values;
    }

    float* end()
    {
        return values[M*M];
    }

    // const pointer to first / one past last element 
    const float* begin() const
    {
        return values;
    }

    const float* end() const
    {
        return values[M*M];
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
};