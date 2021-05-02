// MicroMeterPointVector.h
//
// NNetModel

#pragma once

#include <vector>
#include <assert.h>   
#include "MoreTypes.h"
#include "PointType.h"
#include "ConnectionNeuron.h"
#include "ShapePtrList.h"
#include "MicroMeterPosDir.h"

using std::endl;
using std::vector;
using std::unique_ptr;
using std::make_unique;
using std::max_element;

class MicroMeterPointVector
{
public:

    MicroMeterPointVector() {}

    MicroMeterPointVector( ShapePtrList<ConnNeuron> const& shapeList )
    {
        shapeList.Apply2All( [&](ConnNeuron const & c) { Add( c.GetPosDir() ); } );
    }

    void Apply2All(function<void(MicroMeterPosDir &)> const & func)
    {
        for (auto & elem: m_list)
            func(elem);
    }

    unsigned int Size() const { return Cast2UnsignedInt(m_list.size()); }

    MicroMeterPosDir const GetPosDir( unsigned int const ui) const
    {
        assert( ui < Size() );
        return m_list[ui];
    }

    void SetPosDir( unsigned int const ui, MicroMeterPosDir const & posDir )
    {
        assert( ui < Size() );
        m_list[ui] = posDir;
    }

    void SetDir(Radian const radian)
    {
        Apply2All( [&](MicroMeterPosDir & umPosDir) { umPosDir.SetDir(radian); } );
    }

    void SetDir(MicroMeterPointVector const rhs)
    {
        assert(rhs.Size() == Size());
        for ( int i = 0; i < m_list.size(); ++i )
            m_list[i].SetDir( rhs.GetPosDir(i).GetDir() );
    }

    void SetPos(MicroMeterPointVector const rhs)
    {
        assert(rhs.Size() == Size());
        for ( int i = 0; i < m_list.size(); ++i )
            m_list[i].SetPos( rhs.GetPosDir(i).GetPos() );
    }

    void Clear()
    {
        m_list.clear();
    }

    void Resize( size_t const size )
    {
        m_list.resize( size );
    }

    void Add( MicroMeterPoint const & pos, Radian const dir )
    {
        m_list.push_back( MicroMeterPosDir(pos, dir) );
    }

    void Add( MicroMeterPosDir const & posDir )
    {
        m_list.push_back( posDir );
    }

    bool operator==( MicroMeterPointVector const& rhs ) const
    {
        assert( m_list.size() == rhs.m_list.size() );
        for ( int i = 0; i < m_list.size(); ++i )
            if (m_list[i] != rhs.m_list[i])
                return false;
        return true; 
    }

    MicroMeterPointVector& operator+= (MicroMeterPointVector const & rhs) 
    { 
        assert( m_list.size() == rhs.m_list.size() );
        for ( int i = 0; i < m_list.size(); ++i )
            m_list[i] += rhs.m_list[i];
        return * this; 
    }

    MicroMeterPointVector& operator-= (MicroMeterPointVector const & rhs) 
    { 
        assert( m_list.size() == rhs.m_list.size() );
        for ( int i = 0; i < m_list.size(); ++i )
            m_list[i] -= rhs.m_list[i];
        return * this; 
    }

    MicroMeterPointVector& operator*= (float const factor) 
    { 
        for ( int i = 0; i < m_list.size(); ++i )
            m_list[i] *= factor;
        return * this; 
    }

    friend MicroMeterPointVector const operator+ (MicroMeterPointVector const a, MicroMeterPointVector const b) 
    { 
        MicroMeterPointVector res { a }; 
        res += b; 
        return res; 
    };

    friend MicroMeterPointVector const operator- (MicroMeterPointVector const a, MicroMeterPointVector const b) 
    { 
        MicroMeterPointVector res { a }; 
        res -= b; 
        return res; 
    };

    friend MicroMeterPointVector const operator* (MicroMeterPointVector const a, float const f) 
    { 
        MicroMeterPointVector res { a }; 
        res *= f; 
        return res; 
    };

    friend wostream & operator<< (wostream & out, MicroMeterPointVector const & v)
    {
        out << OPEN_BRACKET << v.m_list.size() << L":";
        for ( auto & it: v.m_list )
        {
            out << it;
            if ( &it == &v.m_list.back() )
                break;
            out << SEPARATOR;
        }
        out << CLOSE_BRACKET;
        return out; 
    }

    Radian const FindMaxRadian() const 
    {
        MicroMeterPosDir const maxElement = * std::max_element
        (
            m_list.begin(), m_list.end(), 
            [](MicroMeterPosDir const & a, MicroMeterPosDir const & b)
            { 
                return a.GetDir() < b.GetDir(); 
            }
        );
        return maxElement.GetDir();
    }

    MicroMeter const FindMaxPos() const
    {
        MicroMeterPosDir const maxElement = * std::max_element
        (
            m_list.begin(), m_list.end(), 
            [](MicroMeterPosDir const & a, MicroMeterPosDir const & b)
            { 
                return Hypot(a.GetPos()) < Hypot(b.GetPos()); 
            }
        );
        return Hypot(maxElement.GetPos());
    }

    MicroMeterLine const GetLine()
    {
        return MicroMeterLine(m_list.front().GetPos(), m_list.back().GetPos());
    }

    void Align( MicroMeterPoint const& umPntStart, MicroMeterPoint const& umPntOffset )
    {	
        MicroMeterPoint umPnt { umPntStart };
        Apply2All
        (	
            [&](MicroMeterPosDir & posDir)	
            { 
                posDir.SetPos(umPnt);
                umPnt += umPntOffset;
            }	
        );
    }

    void Align( MicroMeterLine const umLine, MicroMeter umDist )
    {
        MicroMeterPoint const umVector           { umLine.GetVector() };
        MicroMeter      const umLineLengthTarget { umDist * gapCount() };
        MicroMeterPoint const umPntSingleVector  { umVector.ScaledTo(umDist) };
        MicroMeterPoint const umPntLineTarget    { umVector.ScaledTo(umLineLengthTarget) };
        MicroMeterPoint const umPntTargetStart   { umLine.GetCenter() - umPntLineTarget * 0.5f };
        Align(umPntTargetStart, umPntSingleVector);
    }

    void Align( MicroMeterLine const umLine )
    {
        Align(umLine, umLine.Length() / gapCount());
    }

    void Pack( MicroMeter umDist )
    {
        Align(GetLine(), umDist);
    }

    inline static wchar_t const OPEN_BRACKET  { L'(' };
    inline static wchar_t const SEPARATOR     { L',' };
    inline static wchar_t const CLOSE_BRACKET { L')' };

private:
    float const gapCount() { return Cast2Float(Size() - 1); };

    vector<MicroMeterPosDir> m_list;
};
