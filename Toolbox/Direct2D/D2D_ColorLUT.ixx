// D2D_ColorLUT.ixx
//
// Toolbox\Direct2D

module;

#include <cassert>
#include <array>
#include <memory>
#include "d2d1.h"

export module D2D_ColorLUT;

import Direct2D;
import ColorLUT;

using std::array;
using std::unique_ptr;
using std::make_unique;

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

    D2D_driver const * m_pDriver   { nullptr };
    ColorLUT   const * m_pColorLut { nullptr };

    mutable array<ID2D1Brush *, 256> m_brushTable {};
};