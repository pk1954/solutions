// win32_clut.cpp
//
// Win32_utilities

#include "stdafx.h"
#include <d3d9.h>
#include "win32_clut.h"

void CLUT::Allocate(CLUT_INDEX const uiMaxIndex)
{
    m_data.resize(static_cast<size_t>(uiMaxIndex.GetValue()) + 1);
    setTableValues();
}

void CLUT::SetClutBase(CLUT_INDEX const uiMinLevel)
{
    m_uiBase = uiMinLevel;
    setTableValues();
}
    
void CLUT::SetColorHi(COLORREF const colorHi)
{
    m_colorHi = colorHi;
    setTableValues();
}
    
void CLUT::setTableValues()
{
	UINT const uiMaxIndex = GetSize() - 1;

    UINT const uiRedHi = GetRValue (m_colorHi); 
    UINT const uiGreHi = GetGValue (m_colorHi); 
    UINT const uiBluHi = GetBValue (m_colorHi);

    UINT const uiRedLo = (m_uiBase.GetValue() * uiRedHi) / 100;
    UINT const uiGreLo = (m_uiBase.GetValue() * uiGreHi) / 100;
    UINT const uiBluLo = (m_uiBase.GetValue() * uiBluHi) / 100;

    UINT const uiRedDif = uiRedHi - uiRedLo;
    UINT const uiGreDif = uiGreHi - uiGreLo;
    UINT const uiBluDif = uiBluHi - uiBluLo;

    for (unsigned int uiIndex = 0; uiIndex <= uiMaxIndex; ++uiIndex)
    {
        UINT const uiR = uiRedLo + (uiRedDif * uiIndex) / uiMaxIndex;
        UINT const uiG = uiGreLo + (uiGreDif * uiIndex) / uiMaxIndex;
        UINT const uiB = uiBluLo + (uiBluDif * uiIndex) / uiMaxIndex;

        m_data[uiIndex] = D3DCOLOR_ARGB(255, uiR, uiG, uiB);
    }
}
