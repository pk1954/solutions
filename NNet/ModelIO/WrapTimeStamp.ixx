// WrapTimestamp.ixx
//
// ModelIO

module;

#include <iostream>
#include <ctime>

export module WrapTimestamp;

import Wrapper;
import Script;
import NNetModelIO;

using std::wostream;
using std::endl;

export class WrapTimestamp : public Wrapper
{
public:
    using Wrapper::Wrapper;

    void operator() (Script& script) const final
    {
        wstring       const wstrName   { script.ScrReadString() };
        unsigned long const ulScanTime { script.ScrReadUlong() };
        time_t        const time       { static_cast<time_t>(ulScanTime) };
        NNetModelIO::GetImportNMWI().SetTimestamp(wstrName, time);
    }

    void Write(wostream& out) const final
    {
        NNetModelIO::GetExportNMRI().Apply2AllTimestamps
        (
            [this, &out](wstring const& name, time_t const t)
            {
                char buffer[26];
                ctime_s(buffer, sizeof(buffer), &t);
                WriteCmdName(out);
                out << DOUBLE_QUOTE << name << DOUBLE_QUOTE << SPACE << t << SPACE << COMMENT_SYMBOL << SPACE << buffer << endl;
            }
        );
    };
};
