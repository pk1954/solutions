// MicroMeterPointVector.h
//
// NNetModel

#pragma once

#include <vector>
#include <assert.h>   
#include "MoreTypes.h"
#include "PointType.h"
#include "MicroMeterPosDir.h"

using std::vector;
using std::unique_ptr;
using std::make_unique;

class MicroMeterPointVector
{
public:

    unsigned int Size() const { return Cast2UnsignedInt(m_list.size()); }

    MicroMeterPoint const GetPos(unsigned int const ui) const
    {
        assert( ui < Size() );
        return m_list[ui].GetPos();
    }

    Degrees const GetDir(unsigned int const ui) const
    {
        assert( ui < Size() );
        return m_list[ui].GetDir();
    }

    void SetPosition( unsigned int const ui, MicroMeterPoint const & umPnt )
    {
        assert( ui < Size() );
        m_list[ui].SetPos( umPnt );
    }

    void SetDirection( unsigned int const ui, Degrees const & degrees )
    {
        assert( ui < Size() );
        m_list[ui].SetDir( degrees );
    }

    void Clear()
    {
        m_list.clear();
    }

    void Resize( size_t const size )
    {
        m_list.resize( size );
    }

    void Add( MicroMeterPoint const & pos, Degrees const dir )
    {
        m_list.push_back( MicroMeterPosDir(pos, dir) );
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

private:
    vector<MicroMeterPosDir> m_list;
};
