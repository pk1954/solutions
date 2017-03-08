// win32_clut.h : 
//

#pragma once

#include <vector>

//lint -esym( 613, CLUT::m_data )  possible use of null pointer
//lint -sem( CLUT::Init, initializer )

class CLUT
{
public:

    CLUT( ) 
    { 
        m_uiBase  = 0;
        m_data    = nullptr; 
        m_colorHi = RGB( 255, 255, 255 );
    };
   
    ~CLUT();

    void Allocate( UINT const );
    void SetClutBase( UINT const );
    void SetColorHi( COLORREF const );

    DWORD Get( UINT uiIndex ) const 
    {
        assert( m_data != nullptr );
        return (*m_data)[ uiIndex ]; 
    };

    UINT GetSize( ) const 
    { 
        assert( m_data != nullptr );
        return static_cast<UINT>(m_data->size());
    };

private:
    void setTableValues( );

    UINT                 m_uiBase;
    std::vector<DWORD> * m_data;
    COLORREF             m_colorHi;
};
