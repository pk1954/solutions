// SmoothMoveFp.h
//
// Utilities

#pragma once

class SmoothMoveFp
{
public:

    void Start(unsigned int const uiNrOfSteps)
    {
        m_pos          = START_POINT;
        m_velocity     = 0.0f;
        m_acceleration = 4.0f * DISTANCE / static_cast<float>(uiNrOfSteps * uiNrOfSteps);
    }

    float const GetPos() const { return m_pos; }

    bool Next() // returns true if target reached
    {
        if (m_pos <= BREAK_POINT)
            m_velocity += m_acceleration;
        else 
            m_velocity -= m_acceleration;
        m_pos += m_velocity;
        if (m_pos >= END_POINT * 0.999f)
        {
            m_pos          = END_POINT;
            m_velocity     = 0.0f;
            m_acceleration = 0.0f;
            return true;
        }
        return false;
    }

private:
    inline static float const START_POINT { 0.0f };
    inline static float const END_POINT   { 1.0f };            
    inline static float const DISTANCE    { END_POINT - START_POINT };
    inline static float const BREAK_POINT { START_POINT + DISTANCE * 0.5f };

    float m_pos          { START_POINT };       // runs from START_POINT to END_POINT
    float m_velocity     { 0.0f };
    float m_acceleration { 0.0f };
};