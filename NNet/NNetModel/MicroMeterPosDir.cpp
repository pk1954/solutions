// MicroMeterPosDir.cpp
//
// NNetModel

#include "stdafx.h"
#include "Nob.h"
#include "NNetParameters.h"
#include "MicroMeterPosDir.h"

MicroMeterPosDir::MicroMeterPosDir()
    : m_pos(),
    m_dir()
{}

MicroMeterPosDir::MicroMeterPosDir(MicroMeterPnt const & pos, Radian const & dir)
    : m_pos(pos),
    m_dir(dir)
{}

MicroMeterPosDir::MicroMeterPosDir(Nob const & nob)
  : m_pos(nob.GetPos()),
    m_dir(nob.GetDir())
{}

bool MicroMeterPosDir::operator==(MicroMeterPosDir const& rhs) const
{
    return (m_pos == rhs.m_pos) && (m_dir == rhs.m_dir); 
}

MicroMeterPosDir& MicroMeterPosDir::operator+= (MicroMeterPnt const & pnt) 
{ 
    m_pos += pnt;
    return * this; 
}

MicroMeterPosDir& MicroMeterPosDir::operator+= (MicroMeterPosDir const & rhs) 
{ 
    m_pos += rhs.m_pos;
    m_dir += rhs.m_dir;
    return * this; 
}

MicroMeterPosDir& MicroMeterPosDir::operator-= (MicroMeterPosDir const & rhs) 
{ 
    m_pos -= rhs.m_pos;
    m_dir -= rhs.m_dir;
    return * this; 
}

MicroMeterPosDir& MicroMeterPosDir::operator*= (float const factor) 
{ 
    m_pos *= factor;
    m_dir *= factor;
    return * this; 
}

MicroMeterPosDir const operator+ (MicroMeterPosDir const a, MicroMeterPosDir const b) 
{ 
    MicroMeterPosDir res { a }; 
    res += b; 
    return res; 
};

MicroMeterPosDir const operator- (MicroMeterPosDir const a, MicroMeterPosDir const b) 
{ 
    MicroMeterPosDir res { a }; 
    res -= b; 
    return res; 
};

MicroMeterPosDir const operator* (MicroMeterPosDir const a, float const f) 
{ 
    MicroMeterPosDir res { a }; 
    res *= f; 
    return res; 
};

wostream & operator<< (wostream & out, MicroMeterPosDir const & posDir)
{
    out << MicroMeterPosDir::OPEN_BRACKET << posDir.m_pos 
        << MicroMeterPosDir::SEPARATOR    << posDir.m_dir.GetValue() 
        << MicroMeterPosDir::CLOSE_BRACKET;
    return out;
}

unsigned int const CalcNrOfSteps
(
    MicroMeterPosDir const & umPosDirStart,
    MicroMeterPosDir const & umPosDirTarget
)
{
    MicroMeterPosDir const umPosDirDiff   { umPosDirTarget - umPosDirStart };
    float            const fStepsFromRot  { CalcNrOfSteps(Normalize(umPosDirDiff.GetDir())) };
    float            const fStepsFromMove { CalcNrOfSteps(Hypot    (umPosDirDiff.GetPos())) };
    float            const fSteps         { max(fStepsFromRot, fStepsFromMove) };
    unsigned int     const uiSteps        { Cast2UnsignedInt(fSteps) + 1 };
    return uiSteps;
}
