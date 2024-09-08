// TimestampList.ixx                                    *
//                                                                          *
// Toolbox\Utilities

export module TimestampList;

import std;
import std.compat;
import Util;

using std::map;
using std::wstring;
using std::chrono::system_clock;

export class TimestampList
{
public:

    void SetTimestamp(wstring const name, time_t const t)
    {
        m_list[name] = t ? t : CurrentTime();
    }

    time_t GetTimestamp(wstring const name) const
    {
        auto const iter { m_list.find(name) };
        return (iter == m_list.end()) ? 0 : iter->second;
    }

    void Apply2All(auto const& func) const
    {
        for (auto const iter : m_list)
            func(iter.first, iter.second);
    }

private:
    map <wstring, time_t> m_list;
};