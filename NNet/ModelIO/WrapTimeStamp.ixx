// WrapTimestamp.ixx
//
// ModelIO

export module WrapTimestamp;

import std;
import std.compat;
import Commands;
import RunTime;
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
                WriteCmdName(out);
                out << Quoted(name) << SPACE << t << SPACE << COMMENT_SYMBOL << SPACE << ctime(&t) << endl;
            }
        );
    };
};
