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
        ID2D1Brush * pBrush { m_brushTable[index] };
        if (pBrush == nullptr)
        {
            assert(m_pColorLut);
            Color const color { m_pColorLut->GetColor(index) };
            assert(m_pDriver);
            pBrush = m_pDriver->CreateBrush(color);
            m_brushTable[index] = pBrush;
        }
        return pBrush;
    }

private:

    void clear()
    {
        for (auto &pBrush : m_brushTable)
            if (pBrush)
                SafeRelease(&pBrush);
    }

    D2D_driver const * m_pDriver   { nullptr };
    ColorLUT   const * m_pColorLut { nullptr };

    mutable array<ID2D1Brush *, 256> m_brushTable {};
};