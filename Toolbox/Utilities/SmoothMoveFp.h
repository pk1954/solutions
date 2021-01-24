// SmoothMoveFp.h
//
// Utilities

#pragma once

#include <type_traits>

template <typename T> requires std::is_floating_point_v<T>
class SmoothMoveFp
{
public:

    void SetUp()
    {
        m_pos = START_POINT;
        m_velocity = static_cast<T>(0);
    }

    T const GetPos() const { return m_pos; }

    bool Next( ) // returns true if target reached
    {
        if ( m_pos <= BREAK_POINT )
            m_velocity += ACCELERATION;
        else 
            m_velocity -= ACCELERATION;
        m_pos += m_velocity;
        if ( m_pos >= END_POINT * static_cast<T>(0.99f) )
        {
            m_pos = END_POINT;
            return true;
        }
        return false;
    }

    int const GetNrOfSteps() const 
    {
        return static_cast<int>(NR_OF_STEPS);
    }

private:
    inline static T const START_POINT { static_cast<T>(0) };
    inline static T const END_POINT   { static_cast<T>(1) };
                 
    inline static T const DISTANCE     { END_POINT - START_POINT };
    inline static T const BREAK_POINT  { START_POINT + DISTANCE / static_cast<T>(2) };
    inline static T const NR_OF_STEPS  { 20 };
    inline static T const ACCELERATION { ( static_cast<T>(4) * DISTANCE ) / (static_cast<T>(NR_OF_STEPS) * static_cast<T>(NR_OF_STEPS)) };

    T m_pos      { START_POINT };       // runs from START_POINT to END_POINT
    T m_velocity { 0.0f };
};