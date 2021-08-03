// SmoothMoveFp.h
//
// Utilities

#pragma once

#include <type_traits>

template <typename T> requires std::is_floating_point_v<T>
class SmoothMoveFp
{
public:

    void Start(unsigned int const uiNrOfSteps)
    {
        m_pos          = START_POINT;
        m_velocity     = static_cast<T>(0);
        m_acceleration = static_cast<T>(4) * DISTANCE / 
                         static_cast<T>(uiNrOfSteps * uiNrOfSteps);
    }

    T const GetPos() const { return m_pos; }

    bool Next() // returns true if target reached
    {
        if (m_pos <= BREAK_POINT)
            m_velocity += m_acceleration;
        else 
            m_velocity -= m_acceleration;
        m_pos += m_velocity;
        if (m_pos >= END_POINT * static_cast<T>(0.999f))
        {
            m_pos          = END_POINT;
            m_velocity     = static_cast<T>(0);
            m_acceleration = static_cast<T>(0);
            return true;
        }
        return false;
    }

private:
    inline static T const START_POINT { static_cast<T>(0) };
    inline static T const END_POINT   { static_cast<T>(1) };            
    inline static T const DISTANCE    { END_POINT - START_POINT };
    inline static T const BREAK_POINT { START_POINT + DISTANCE / static_cast<T>(2) };

    T m_pos          { START_POINT };       // runs from START_POINT to END_POINT
    T m_velocity     { 0.0f };
    T m_acceleration { 0.0f };
};