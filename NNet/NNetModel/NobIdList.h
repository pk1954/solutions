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
    NobIdList(Connector              const &);
    NobIdList(NobPtrList<Nob>        const &);
    NobIdList(NobPtrList<IoNeuron> const &);

    virtual ~NobIdList() {}

    void        Add   (NobId  const   id  )             { m_list.push_back(id); }
    void        Add   (Nob    const & nob )             { Add(nob.GetId()); }
    void        Resize(size_t const   size)             { m_list.resize(size); }
    void        SetAt (int const index, NobId const id) { m_list.at(index) = id; }
    int   const Size  ()                                { return Cast2Int(m_list.size()); }
    NobId const Get   (int const index)                 { return m_list.at(index); }

    void Apply2All( function<void(NobId const &)> const& func ) const;

    friend wostream & operator<< (wostream &, NobIdList const &);

    inline static wchar_t const OPEN_BRACKET  { L'(' };
    inline static wchar_t const SEPARATOR     { L',' };
    inline static wchar_t const CLOSE_BRACKET { L')' };

private:
    vector<NobId> m_list {};
};
