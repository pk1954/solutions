// TimestampList.ixx                                    *
//                                                                          *
// Toolbox\Utilities

module;

#include <string>
#include <chrono>
#include <ctime>
#include <map>

export module TimestampList;

using std::map;
using std::wstring;
using std::chrono::system_clock;

export class TimestampList
{
public:

    void DefineTimestamp(wstring const name)
    {
        m_list[name] = 0;
    }

    void SetTimestamp(wstring const name, time_t const t)
    {
        m_list[name] = t;
    }

    void SetTimestampNow(wstring const name)
    {
        auto   currentTime{ system_clock::now() };
        time_t currentTime_t { system_clock::to_time_t(currentTime)
    };
        SetTimestamp(name, currentTime_t);
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