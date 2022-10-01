// MicroMeterPosDir.cpp
//
// NNetModel

module;

#include <iostream>
#include <algorithm>

module NNetModel:MicroMeterPosDir;

import Types;
import IoConstants;
import :NNetParameters;

using std::max;
using std::wostream;

MicroMeterPosDir::MicroMeterPosDir()
  : m_pos(),
    m_dir()
{}

MicroMeterPosDir::MicroMeterPosDir(MicroMeterPnt const & pos, Radian const & dir)
    : m_pos(pos),
    m_dir(dir)
{}

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

MicroMeterPosDir operator+ (MicroMeterPosDir const a, MicroMeterPosDir const b) 
{ 
    MicroMeterPosDir res { a }; 
    res += b; 
    return res; 
};

MicroMeterPosDir operator- (MicroMeterPosDir const a, MicroMeterPosDir const b) 
{ 
    MicroMeterPosDir res { a }; 
    res -= b; 
    return res; 
};

MicroMeterPosDir operator* (MicroMeterPosDir const a, float const f) 
{ 
    MicroMeterPosDir res { a }; 
    res *= f; 
    return res; 
};

wostream & operator<< (wostream & out, MicroMeterPosDir const & posDir)
{
    out << OPEN_BRACKET << posDir.m_pos 
        << SEPARATOR    << posDir.m_dir.GetValue() 
        << CLOSE_BRACKET;
    return out;
}
