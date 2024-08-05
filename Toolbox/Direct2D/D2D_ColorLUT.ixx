// D2D_ColorLUT.ixx
//
// Toolbox\Direct2D

module;

#include <cassert>
#include "d2d1.h"

export module D2D_ColorLUT;

import std;
import Direct2D;
import ColorLUT;

using std::array;

export class D2D_ColorLUT
{
public:

    D2D_ColorLUT
    (
        ColorLUT   const * pLut,
        D2D_driver const * pDriver
    )
      : m_pColorLut(pLut),
        m_pDriver(pDriver)
    {}
 
    ~D2D_ColorLUT()
    {
        clear();
    }

    void Reset(ColorLUT const* pLut)
    {
        m_pColorLut = pLut;
        clear();
    }

    ID2D1Brush* GetBrush(ColIndex const index) const
    {
        BrushHandle hBrush { m_brushTable[index] };
        if (hBrush == nullptr)
        {
            assert(m_pColorLut);
            Color const color { m_pColorLut->GetColor(index) };
            assert(m_pDriver);
            hBrush = m_pDriver->CreateBrushHandle(color);
            m_brushTable[index] = hBrush;
        }
        return hBrush;
    }

private:

    void clear()
    {
        for (auto &hBrush : m_brushTable)
            if (hBrush)
                SafeRelease(&hBrush);
    }

    D2D_driver const * m_pDriver   { nullptr };
    ColorLUT   const * m_pColorLut { nullptr };

    mutable array<BrushHandle, 256> m_brushTable {};
};