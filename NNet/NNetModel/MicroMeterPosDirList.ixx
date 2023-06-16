// MicroMeterPosDirList.ixx
//
// NNetModel

module;

#include <vector>
#include <cassert>   
#include <iostream>   

export module NNetModel:MicroMeterPosDirList;

import SaveCast;
import Types;
import :IoLine;

using std::vector;
using std::wostream;

export class MicroMeterPosDirList
{
public:

    MicroMeterPosDirList() = default;
    MicroMeterPosDirList(vector<IoLine *> const &);

    template <class FUNC>
    void Apply2All(FUNC const& func)
    {
        for (auto& elem : m_list)
            func(elem);
    }

    unsigned int Size() const;

    MicroMeterPosDir const& GetPosDir(size_t const) const;

    void SetPosDir(unsigned int const, MicroMeterPosDir const &);
    void SetDir(Radian const);
    void SetDirVector(MicroMeterPnt const &p) { SetDir(Vector2Radian(p)); }
    void SetDir(MicroMeterPosDirList const &);
    void SetPos(MicroMeterPosDirList const &);
    void Resize(size_t const);
    void Add(MicroMeterPosDir * const);
    //void Add(MicroMeterPnt    const &, Radian const);
    void Align(MicroMeterPnt  const &, MicroMeterPnt const &);
    void Align(MicroMeterLine const &, MicroMeter);
    void Align(MicroMeterLine const &);
    void Pack(MicroMeter);
    void Clear();

    bool operator==(MicroMeterPosDirList const &) const;

    MicroMeterPosDirList& operator+= (MicroMeterPosDirList const&);
    MicroMeterPosDirList& operator-= (MicroMeterPosDirList const&);
    MicroMeterPosDirList& operator*= (float const);

    Radian         FindMaxRadian() const;
    MicroMeter     FindMaxPos() const;
    MicroMeterLine GetLine() const;

    friend MicroMeterPosDirList operator+ (MicroMeterPosDirList const&, MicroMeterPosDirList const&);
    friend MicroMeterPosDirList operator- (MicroMeterPosDirList const&, MicroMeterPosDirList const&);
    friend MicroMeterPosDirList operator* (MicroMeterPosDirList const&, float const);

    friend unsigned int CalcNrOfSteps(MicroMeterPosDirList const&, MicroMeterPosDirList const&);

    friend wostream& operator<< (wostream&, MicroMeterPosDirList const&);

private:
    float gapCount() const { return Cast2Float(Size() - 1); };

    vector<MicroMeterPosDir *> m_list;
};

