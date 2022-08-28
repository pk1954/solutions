// Accelerators.ixx
//
// NNetSimu

module;

#include <cassert>
#include <array>
#include <Windows.h>
#include "Resource.h"
#include "win32_util_resource.h"

export module Accelerators;

import SaveCast;

using std::array;

export class Accelerators
{
public:
    HACCEL Get()
    {
        HACCEL haccel { CreateAcceleratorTable(LPACCEL(m_acc.data()), Cast2Int(m_acc.size()))};
        assert(haccel);
        ACCEL a1 { m_acc[2] };
        return haccel;
    }

private:

    inline static const array<ACCEL, 14> m_acc
    {
        ACCEL(FVIRTKEY | FCONTROL, WORD('z'), IDM_UNDO),
        ACCEL(FVIRTKEY | FCONTROL, WORD('y'), IDM_REDO),
        ACCEL(FVIRTKEY | FCONTROL, WORD('a'), IDM_SELECT_ALL),
        ACCEL(FVIRTKEY | FCONTROL, WORD('s'), IDM_SAVE_MODEL),
        ACCEL(FVIRTKEY | FCONTROL, WORD('c'), IDM_COPY_SELECTION),
        ACCEL(           FALT,     WORD('c'), IDM_CRSR_WINDOW),
        ACCEL(           FALT,     WORD('d'), IDM_DESC_WINDOW),
        ACCEL(           FALT,     WORD('p'), IDM_PARAM_WINDOW),
        ACCEL(           FALT,     WORD('m'), IDM_MINI_WINDOW),
        ACCEL(           FALT,     WORD('?'), IDM_ABOUT),
        ACCEL(           FALT,     WORD('n'), IDM_NEXT_SCRIPT_CMD),
        ACCEL(FVIRTKEY | FALT,     VK_BACK,   IDM_UNDO),
        ACCEL(FVIRTKEY,            VK_DELETE, IDM_DELETE),
        ACCEL(FVIRTKEY,            VK_ESCAPE, IDM_ESCAPE)
    };
};
