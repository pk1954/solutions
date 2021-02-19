// MicroMeterPointVector.h
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

class MicroMeterPointVector
{
public:

    unsigned int Size() { return Cast2UnsignedInt(m_list.size()); }

    MicroMeterPoint const & GetPos( unsigned int const ui )
    {
        assert( ui < Size() );
        return m_list[ui];
    }

    void SetPosition( unsigned int const ui, MicroMeterPoint const & umPnt )
    {
        assert( ui < Size() );
        m_list[ui] = umPnt;
    }

    void Clear()
    {
        m_list.clear();
    }

    void Resize( size_t const size )
    {
        m_list.resize( size );
    }

    void Add( MicroMeterPoint const & umPnt )
    {
        m_list.push_back( umPnt );
    }

    MicroMeterPointVector& operator+= (MicroMeterPointVector const & rhs) 
    { 
        if ( m_list.size() != rhs.m_list.size() )
        {
            int x = 42;
        }
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
    vector<MicroMeterPoint> m_list;
};
