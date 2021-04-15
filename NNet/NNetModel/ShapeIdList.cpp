// ShapeIdList.cpp 
//
// NNetModel

#include "stdafx.h"
#include "Shape.h"
#include "ShapeIdList.h"

ShapeIdList::ShapeIdList(ShapePtrList<ConnNeuron> const& list) 
{
    list.Apply2All([&](Shape & s) { Add(s.GetId()); } );
}

void ShapeIdList::Apply2All( function<void(ShapeId const &)> const& func ) const
{
    for (ShapeId const & id : m_list)
        func(id);
}

wostream & operator<< (wostream & out, ShapeIdList const & v)
{
    out << ShapeIdList::OPEN_BRACKET << v.m_list.size() << L":";
    for (auto & it : v.m_list)
    {
        out << it;
        if ( &it == &v.m_list.back() )
            break;
        out << ShapeIdList::SEPARATOR;
    }
    out << ShapeIdList::CLOSE_BRACKET;
    return out; 
}
