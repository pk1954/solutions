// Bitmap.ixx
//
// Toolbox\win32_utilities

module;

#include <windows.h>

export module Bitmap;

import std;
import std.compat;

using std::bit_cast;

export HBITMAP CreateBitmapFromIconData(unsigned char const &iconData) 
{
#pragma pack(push, 2)
    struct IconData
    {
        BITMAPFILEHEADER bmpFileHeader;
        BITMAPINFOHEADER bmpInfoHeader;
        unsigned char    pixelData;
    };
#pragma pack(pop)    

    IconData   const * const pData    { bit_cast<IconData const*>(&iconData) };
    void             *       pBits    { nullptr };
    BITMAPINFO const * const pBmpInfo { bit_cast<BITMAPINFO const *>(&(pData->bmpInfoHeader)) };
    HBITMAP hBitmap = CreateDIBSection(NULL, pBmpInfo, DIB_RGB_COLORS, &pBits, NULL, 0);

    if (hBitmap && pBits)
        memcpy(pBits, &(pData->pixelData), pData->bmpInfoHeader.biSizeImage);

    return hBitmap;
}