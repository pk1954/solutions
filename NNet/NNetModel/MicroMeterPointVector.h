// MicroMeterPointVector.h
//
// NNetModel

#pragma once

#include <vector>
#include <assert.h>   
#include "MoreTypes.h"
#include "PointType.h"
#include "ShapePtrList.h"
#include "ConnectionNeuron.h"
#include "MicroMeterPosDir.h"

using std::endl;
using std::vector;
using std::unique_ptr;
using std::make_unique;

class MicroMeterPointVector
{
public:

    MicroMeterPointVector() {}

    MicroMeterPointVector( ShapePtrList<ConnNeuron> const& shapeList )
    {
        shapeList.Apply2All( [&](ConnNeuron const & c) { Add( c.GetPosDir() ); } );
    }

    void Apply2All(function<void(MicroMeterPosDir const &)> const & func)
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

    inline static wchar_t const OPEN_BRACKET  { L'(' };
    inline static wchar_t const SEPARATOR     { L',' };
    inline static wchar_t const CLOSE_BRACKET { L')' };

private:
    vector<MicroMeterPosDir> m_list;
};
