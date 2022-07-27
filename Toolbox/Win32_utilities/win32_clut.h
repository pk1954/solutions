// win32_clut.h : 
//

#pragma once

#include <vector>

import NamedType;

using std::vector;

using CLUT_INDEX = NamedType< int, struct CLUT_INDEX_Parameter >;

class CLUT
{
public:

    CLUT() 
    { 
        m_uiBase  = CLUT_INDEX(0);
        m_colorHi = RGB(255, 255, 255);
    };
   
    virtual ~CLUT() {};

    void Allocate(CLUT_INDEX const);
    void SetClutBase(CLUT_INDEX const);
    void SetColorHi(COLORREF const);

    COLORREF GetColor(CLUT_INDEX index) const 
    {
		assert(index.GetValue() < static_cast<int>(m_data.size())); 
        return m_data[ index.GetValue() ]; 
    };

    UINT GetSize() const 
    { 
        return static_cast<UINT>(m_data.size());
    };

    COLORREF GetColorHi() const 
    { 
        return m_colorHi;
    };

private:
    void setTableValues();

    CLUT_INDEX    m_uiBase;
    vector<DWORD> m_data;
    COLORREF      m_colorHi;
};
