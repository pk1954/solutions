// NobIdList.h
//
// NNetModel

#pragma once

#include <vector>
#include "util.h"
#include "NobId.h"

using std::vector;

class Nob;

class NobIdList
{
public:
    NobIdList() {}

    virtual ~NobIdList() {}

    void        Resize(size_t const   size)             { m_list.resize(size); }
    int   const Size  ()                                { return Cast2Int(m_list.size()); }
    void        SetAt (int const index, NobId const id) { m_list.at(index) = id; }
    NobId const Get   (size_t const index)              { return m_list.at(index); }
    void        Push  (NobId  const   id  )             { m_list.push_back(id); }
    void        Push  (Nob    const &);
    NobId const Pop   ();

    void Apply2All(function<void(NobId const &)> const &) const;

    friend wostream & operator<< (wostream &, NobIdList const &);

    inline static wchar_t const OPEN_BRACKET  { L'(' };
    inline static wchar_t const SEPARATOR     { L',' };
    inline static wchar_t const CLOSE_BRACKET { L')' };

private:
    vector<NobId> m_list {};
};
