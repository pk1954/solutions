// MicroMeterPosDir.ixx
//
// NNetModel

module;

#include <iostream>
#include <vector>
#include <cassert>   

export module MicroMeterPosDir;

import Types;

using std::vector;
using std::unique_ptr;
using std::make_unique;
using std::wostream;

export class MicroMeterPosDir
{
public:

    MicroMeterPosDir();
    MicroMeterPosDir(MicroMeterPnt const&, Radian const&);

    MicroMeterPnt GetPos() const { return m_pos; }
    Radian        GetDir() const { return m_dir; }

    void SetPos(MicroMeterPnt const& pos) { m_pos = pos; }
    void SetDir(Radian        const& dir) { m_dir = dir; }

    bool operator==(MicroMeterPosDir const&) const = default;

    MicroMeterPosDir& operator+= (MicroMeterPnt    const&);
    MicroMeterPosDir& operator+= (MicroMeterPosDir const&);
    MicroMeterPosDir& operator-= (MicroMeterPosDir const&);
    MicroMeterPosDir& operator*= (float const);

    friend MicroMeterPosDir operator+ (MicroMeterPosDir const, MicroMeterPosDir const);
    friend MicroMeterPosDir operator- (MicroMeterPosDir const, MicroMeterPosDir const);
    friend MicroMeterPosDir operator* (MicroMeterPosDir const, float const);

    friend wostream& operator<< (wostream&, MicroMeterPosDir const&);

    static MicroMeterPosDir const& NULL_VAL()
    {
        static MicroMeterPosDir res
        {
            MicroMeterPosDir(MicroMeterPnt::NULL_VAL(),
            Radian::NULL_VAL())
        };
        return res;
    };

    friend MicroMeter Hypot(MicroMeterPosDir const& p) { return Hypot(p.GetPos()); }

private:
    MicroMeterPnt m_pos;
    Radian        m_dir;
};

