// TimeStamp.ixx                                    *
//                                                                          *
// Toolbox\Commands

module;

#include <string>
#include <ctime>

export module TimeStamp;

import Symtab;

using std::wstring;

inline wstring const PREFIX { L"TimeStamp_" };

export void DefTimeStamp(wstring const name)
{
    SymbolTable::ScrDefConst(PREFIX + name, 0L);
}

export void SetTimeStamp(wstring const name, time_t const t)
{
    SymbolTable::ScrDefConst(PREFIX + name, 0L);
}

//export time_t GetTimeStamp(wstring const name)
//{
//    return SymbolTable::ScrDefConst(PREFIX + name, 0L);
//}
