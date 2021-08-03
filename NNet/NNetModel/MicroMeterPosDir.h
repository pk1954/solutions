// MicroMeterPosDir.h
//
// NNetModel

#pragma once

#include <vector>
#include <assert.h>   
#include "MoreTypes.h"
#include "PointType.h"

using std::vector;
using std::unique_ptr;
using std::make_unique;

class Nob;

class MicroMeterPosDir
{
public:

    MicroMeterPosDir();
    MicroMeterPosDir(MicroMeterPnt const &, Radian const &);
    MicroMeterPosDir(Nob const &);
    
    MicroMeterPnt const GetPos() const { return m_pos; }
    Radian        const GetDir() const { return m_dir; }

    void SetPos(MicroMeterPnt const & pos) { m_pos = pos; }
    void SetDir(Radian        const & dir) { m_dir = dir; }

    bool operator==(MicroMeterPosDir const&) const;

    MicroMeterPosDir& operator+= (MicroMeterPnt const &);
    MicroMeterPosDir& operator+= (MicroMeterPosDir const &); 
    MicroMeterPosDir& operator-= (MicroMeterPosDir const &); 
    MicroMeterPosDir& operator*= (float const); 

    friend MicroMeterPosDir const operator+ (MicroMeterPosDir const, MicroMeterPosDir const); 
    friend MicroMeterPosDir const operator- (MicroMeterPosDir const, MicroMeterPosDir const); 
    friend MicroMeterPosDir const operator* (MicroMeterPosDir const, float const); 

    friend wostream & operator<< (wostream &, MicroMeterPosDir const &);

    inline static wchar_t const OPEN_BRACKET  { L'(' };
    inline static wchar_t const SEPARATOR     { L',' };
    inline static wchar_t const CLOSE_BRACKET { L')' };

    static MicroMeterPosDir const & NULL_VAL() 
    { 
        static MicroMeterPosDir res { MicroMeterPosDir(MicroMeterPnt::NULL_VAL(), Radian::NULL_VAL()) }; 
        return res;
    };

private:
    MicroMeterPnt m_pos;
    Radian        m_dir;
};

unsigned int const CalcNrOfSteps(MicroMeterPosDir const &, MicroMeterPosDir const &);
