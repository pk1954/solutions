// WindowLayout.h : 
//
// Win32_utilities

#pragma once

#include "PixelTypes.h"

struct PixHorzEdge
{
    PIXEL m_horzStart;
    PIXEL m_horzEnd;
    PIXEL m_vertical;
};

struct PixVertEdge
{
    PIXEL m_vertStart;
    PIXEL m_vertEnd;
    PIXEL m_horizontal;
};