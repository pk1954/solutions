// NobIdList.ixx
//
// NNetModel

module;

#include <vector>
#include <ostream>

export module NNetModel:NobIdList;

import Util;
import :NobId;
import :Nob;

using std::vector;
using std::wostream;

export class NobIdList
{
public:
    NobIdList() = default;

    virtual ~NobIdList() = default;

    bool   IsEmpty()                            const { return m_list.empty(); }
    bool   IsNotEmpty()                         const { return !IsEmpty(); }
    size_t Size()                               const { return m_list.size(); }
    NobId  Get   (size_t const index)           const { return m_list.at(index); }
    void   Resize(size_t const size)                  { m_list.resize(size); }
    void   SetAt (size_t const index, NobId const id) { m_list.at(index) = id; }
    void   Push  (NobId  const id)                    { m_list.push_back(id); }
    void   Push  (Nob    const &);
    NobId  Pop();

    void Apply2All(auto const& func) const
    {
        for (NobId const& id : m_list)
            func(id);
    }

    friend wostream& operator<< (wostream&, NobIdList const&);

private:
    vector<NobId> m_list{};
};
