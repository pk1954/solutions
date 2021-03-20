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

    MicroMeterPointVector( ShapePtrList<ConnectionNeuron> const& shapeList )
    {
        shapeList.Apply2All( [&](ConnectionNeuron const & c) { Add( c.GetPosDir() ); } );
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

    friend wostream & operator<< ( wostream & out, MicroMeterPointVector const & v )
    {
        for ( auto & it: v.m_list )
            out << it << endl;
        return out; 
    }

private:
    vector<MicroMeterPosDir> m_list;
};
