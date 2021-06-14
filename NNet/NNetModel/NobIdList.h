// NobIdList.h
//
// NNetModel

#pragma once

#include <vector>
#include "util.h"
#include "NobId.h"
#include "Connector.h"
#include "IoNeuron.h"

using std::vector;
using std::endl;

class Nob;

class NobIdList
{
public:
    NobIdList() {}
    NobIdList(Connector            const &);
    NobIdList(NobPtrList<Nob>      const &);
    NobIdList(NobPtrList<IoNeuron> const &);

    virtual ~NobIdList() {}

    void        Resize(size_t const   size)             { m_list.resize(size); }
    int   const Size  ()                                { return Cast2Int(m_list.size()); }
    void        Push  (NobId  const   id  )             { m_list.push_back(id); }
    void        Push  (Nob    const & nob )             { Push(nob.GetId()); }
    void        SetAt (int const index, NobId const id) { m_list.at(index) = id; }
    NobId const Get   (size_t const index)              { return m_list.at(index); }
    NobId const Pop()
    {
        NobId const id { m_list.back() };
        m_list.pop_back();
        return id;
    }

    void Apply2All( function<void(NobId const &)> const& func ) const;

    friend wostream & operator<< (wostream &, NobIdList const &);

    inline static wchar_t const OPEN_BRACKET  { L'(' };
    inline static wchar_t const SEPARATOR     { L',' };
    inline static wchar_t const CLOSE_BRACKET { L')' };

private:
    vector<NobId> m_list {};
};
