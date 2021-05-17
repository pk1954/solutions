// NobIdList.cpp 
//
// NNetModel

#include "stdafx.h"
#include "Nob.h"
#include "NobIdList.h"

NobIdList::NobIdList(Connector const & connector) 
{
    connector.Apply2All([&](Nob const & s) { Add(s.GetId()); } );
}

NobIdList::NobIdList(NobPtrList<IoNeuron> const& list) 
{
    list.Apply2All([&](Nob & s) { Add(s.GetId()); } );
}

NobIdList::NobIdList(NobPtrList<Nob> const& list) 
{
    list.Apply2All([&](Nob & s) { Add(s.GetId()); } );
}

void NobIdList::Apply2All( function<void(NobId const &)> const& func ) const
{
    for (NobId const & id : m_list)
        func(id);
}

wostream & operator<< (wostream & out, NobIdList const & v)
{
    out << NobIdList::OPEN_BRACKET << v.m_list.size() << L":";
    for (auto & it : v.m_list)
    {
        out << it;
        if ( &it == &v.m_list.back() )
            break;
        out << NobIdList::SEPARATOR;
    }
    out << NobIdList::CLOSE_BRACKET;
    return out; 
}
