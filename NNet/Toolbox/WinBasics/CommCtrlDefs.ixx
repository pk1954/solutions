// CommCtrlDefs.ixx
//
// Toolbox\WinBasics

export module WinBasics:CommCtrlDefs;

import :WinDefs;

export 
{
    const UINT TTS_ALWAYSTIP = 0x01;
    const UINT TTS_NOPREFIX  = 0x02;
    const UINT TTS_NOANIMATE = 0x10;
    const UINT TTS_NOFADE    = 0x20;
    const UINT TTS_BALLOON   = 0x40;
    const UINT TTS_CLOSE     = 0x80;
}
