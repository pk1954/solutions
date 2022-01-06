// NobIdList.cpp 
//
// NNetModel

#include "stdafx.h"
#include "Nob.h"
#include "NobIdList.h"

using std::endl;

void NobIdList::Push(Nob const & nob)
{ 
    Push(nob.GetId()); 
}

NobId NobIdList::Pop()
{
    NobId const id { m_list.back() };
    m_list.pop_back();
    return id;
}

wostream & operator<< (wostream & out, NobIdList const & v)
{
    out << NobIdList::OPEN_BRACKET << v.m_list.size() << L":";
    for (auto & it : v.m_list)
    {
        out << it;
        if (&it == &v.m_list.back())
            break;
        out << NobIdList::SEPARATOR;
    }
    out << NobIdList::CLOSE_BRACKET;
    return out; 
}
