// MicroMeterPntVector.ixx
//
// NNetModel

module;

#include <vector>
#include <cassert>   
#include <iostream>   
#include "MicroMeterPosDir.h"

export module MicroMeterPntVector;

import SaveCast;
import BasicTypes;
import MoreTypes;
import PointType;
import AngleTypes;
import IoLine;

using std::vector;
using std::wostream;

export class MicroMeterPntVector
{
public:

    MicroMeterPntVector() = default;
    MicroMeterPntVector(vector<IoLine*> const&);

    template <class FUNC>
    void Apply2All(FUNC const& func)
    {
        for (auto& elem : m_list)
            func(elem);
    }

    unsigned int Size() const;

    MicroMeterPosDir GetPosDir(size_t const) const;

    void SetPosDir(unsigned int const, MicroMeterPosDir const&);
    void SetDir(Radian const);
    void SetDir(MicroMeterPntVector const&);
    void SetPos(MicroMeterPntVector const&);
    void Resize(size_t const);
    void Add(MicroMeterPosDir const&);
    void Add(MicroMeterPnt    const&, Radian const);
    void Align(MicroMeterPnt    const&, MicroMeterPnt const&);
    void Align(MicroMeterLine   const&, MicroMeter);
    void Align(MicroMeterLine   const&);
    void Pack(MicroMeter);
    void Clear();

    bool operator==(MicroMeterPntVector const&) const;

    MicroMeterPntVector& operator+= (MicroMeterPntVector const&);
    MicroMeterPntVector& operator-= (MicroMeterPntVector const&);
    MicroMeterPntVector& operator*= (float const);

    Radian         FindMaxRadian() const;
    MicroMeter     FindMaxPos() const;
    MicroMeterLine GetLine() const;

    friend MicroMeterPntVector operator+ (MicroMeterPntVector const&, MicroMeterPntVector const&);
    friend MicroMeterPntVector operator- (MicroMeterPntVector const&, MicroMeterPntVector const&);
    friend MicroMeterPntVector operator* (MicroMeterPntVector const&, float const);

    friend unsigned int CalcNrOfSteps(MicroMeterPntVector const&, MicroMeterPntVector const&);

    friend wostream& operator<< (wostream&, MicroMeterPntVector const&);

private:
    float gapCount() const { return Cast2Float(Size() - 1); };

    vector<MicroMeterPosDir> m_list;
};
