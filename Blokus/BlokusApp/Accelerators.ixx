// Accelerators.ixx
//
// BlokusApp 

export module Accelerators;

import std;
import Win32_Util_Resource;
import WinBasics;
import SaveCast;
import Resource;

using std::array;

export class Accelerators
{
public:
    HACCEL Get()
    {
        HACCEL haccel { CreateAcceleratorTableW(LPACCEL(m_acc), sizeof(m_acc) / sizeof(ACCEL)) };
        return haccel;
    }

private:

    inline static ACCEL m_acc[14]
    {
        //ACCEL(FVIRTKEY | FCONTROL, WORD('z'), IDM_UNDO),
        //ACCEL(FVIRTKEY | FCONTROL, WORD('y'), IDM_REDO),
        //ACCEL(FVIRTKEY | FALT,     VK_BACK,   IDM_UNDO),
        //ACCEL(FVIRTKEY,            VK_DELETE, IDM_DELETE),
        //ACCEL(FVIRTKEY,            VK_ESCAPE, IDM_ESCAPE),
        //ACCEL(FALT,                WORD('r'), IDM_RUN_TIL_END),
        //ACCEL(FALT,                WORD('i'), IDM_INITIALIZE),
        ACCEL(FALT,                WORD('?'), IDM_ABOUT),
        ACCEL(FALT,                WORD('n'), IDM_NEXT_SCRIPT_CMD)
    };
};
