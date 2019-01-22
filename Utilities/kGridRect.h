// kGridRect.h : 
//
// library Utilities

#pragma once

#include "kGridPoint.h"

class KGridRect
{
public:
    KGridRect
	( 
		KGridPoint const kpPos, 
		KGridPoint const kpSize 
	) 
	 :	m_pos(kpPos), 
		m_size(kpSize) 
	{};

    bool const operator== (KGridRect const &a) const { return (a.m_pos == m_pos) && (a.m_size == m_size); };
    bool const operator!= (KGridRect const &a) const { return (a.m_pos != m_pos) || (a.m_size != m_size); };

    KGridPoint const GetPos ( ) const { return m_pos;  };
    KGridPoint const GetSize( ) const { return m_size; };

    static KGridRect const KGRID_RECT_EMPTY;

    bool const IsEmpty( )    const { return ( *this == KGRID_RECT_EMPTY ); }
    bool const IsNotEmpty( ) const { return ( *this != KGRID_RECT_EMPTY ); }

private:
    KGridPoint m_pos;
    KGridPoint m_size;
};
