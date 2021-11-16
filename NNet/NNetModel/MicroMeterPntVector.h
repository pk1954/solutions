// MicroMeterPntVector.h
//
// NNetModel

#pragma once

#include <vector>
#include <assert.h>   
#include "MoreTypes.h"
#include "PointType.h"
#include "IoNeuron.h"
#include "MicroMeterPosDir.h"

using std::endl;
using std::vector;
using std::unique_ptr;
using std::make_unique;
using std::max_element;

class MicroMeterPntVector
{
public:

    MicroMeterPntVector() {}
    MicroMeterPntVector(vector<IoNeuron *> const&);

    void Apply2All(function<void(MicroMeterPosDir &)> const &);

    unsigned int Size() const;

    MicroMeterPosDir GetPosDir(size_t const) const;

    void SetPosDir(unsigned int const, MicroMeterPosDir const &);
    void SetDir   (Radian const);
    void SetDir   (MicroMeterPntVector const &);
    void SetPos   (MicroMeterPntVector const &);
    void Resize   (size_t const);
    void Add      (MicroMeterPosDir const &);
    void Add      (MicroMeterPnt    const &, Radian const);
    void Align    (MicroMeterPnt    const &, MicroMeterPnt const &);
    void Align    (MicroMeterLine   const &, MicroMeter);
    void Align    (MicroMeterLine   const &);
    void Pack     (MicroMeter);
    void Clear();

    bool operator==(MicroMeterPntVector const&) const;

    MicroMeterPntVector& operator+= (MicroMeterPntVector const &); 
    MicroMeterPntVector& operator-= (MicroMeterPntVector const &); 
    MicroMeterPntVector& operator*= (float const);

    Radian         FindMaxRadian() const; 
    MicroMeter     FindMaxPos() const;
    MicroMeterLine GetLine() const;

    friend MicroMeterPntVector operator+ (MicroMeterPntVector const, MicroMeterPntVector const);
    friend MicroMeterPntVector operator- (MicroMeterPntVector const, MicroMeterPntVector const); 
    friend MicroMeterPntVector operator* (MicroMeterPntVector const, float const);

    friend unsigned int CalcNrOfSteps(MicroMeterPntVector const &, MicroMeterPntVector const &);

    friend wostream & operator<< (wostream &, MicroMeterPntVector const &);

    inline static wchar_t const OPEN_BRACKET  { L'(' };
    inline static wchar_t const SEPARATOR     { L',' };
    inline static wchar_t const CLOSE_BRACKET { L')' };

private:
    float gapCount() const { return Cast2Float(Size() - 1); };

    vector<MicroMeterPosDir> m_list;
};
