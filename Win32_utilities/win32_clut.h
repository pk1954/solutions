// win32_clut.h : 
//

#pragma once

#include "NamedType.h"
#include <vector>

//lint -esym( 613, CLUT::m_data )  possible use of null pointer
//lint -sem( CLUT::Init, initializer )

using CLUT_INDEX = NamedType< int, struct CLUT_INDEX_Parameter >;

class CLUT
{
public:

    CLUT( ) 
    { 
        m_uiBase  = CLUT_INDEX(0);
        m_data    = nullptr; 
        m_colorHi = RGB( 255, 255, 255 );
    };
   
    ~CLUT();

    void Allocate( CLUT_INDEX const );
    void SetClutBase( CLUT_INDEX const );
    void SetColorHi( COLORREF const );

    COLORREF GetColor( CLUT_INDEX index ) const 
    {
        assert( m_data != nullptr );
		assert( index.GetValue() < m_data->size() ); 
        return (*m_data)[ index.GetValue() ]; 
    };

    UINT GetSize( ) const 
    { 
        assert( m_data != nullptr );
        return static_cast<UINT>(m_data->size());
    };

    COLORREF GetColorHi( ) const 
    { 
        return m_colorHi;
    };

private:
    void setTableValues( );

    CLUT_INDEX           m_uiBase;
    std::vector<DWORD> * m_data;
    COLORREF             m_colorHi;
};
