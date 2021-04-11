// ShapeIdList.h
//
// NNetModel

#pragma once

#include <vector>
#include "util.h"
#include "Shape.h"
#include "UPShapeList.h"
#include "ShapeId.h"

using std::vector;
using std::endl;

class ShapeIdList
{
public:
    ShapeIdList() {}
    virtual ~ShapeIdList() {}

    void          Add   (ShapeId const   id   ) { m_list.push_back(id); }
    void          Add   (Shape   const & shape) { Add(shape.GetId()); }
    void          Resize(size_t  const   size ) { m_list.resize(size); }
    void          SetAt (int const index, ShapeId const id) { m_list.at(index) = id; }
    int     const Size() { return Cast2Int(m_list.size()); }
    ShapeId const Get(int const index) { return m_list.at(index); }

    void Apply2All( function<void(ShapeId const &)> const& func ) const
    {
        for (ShapeId const & id : m_list)
            func(id);
    }

    friend wostream & operator<< (wostream & out, ShapeIdList const & v)
    {
        out << OPEN_BRACKET << v.m_list.size() << L":";
        for (auto & it : v.m_list)
        {
            out << it;
            if ( &it == &v.m_list.back() )
                break;
            out << SEPARATOR;
        }
        out << CLOSE_BRACKET;
        return out; 
    }

    inline static wchar_t const OPEN_BRACKET  { L'(' };
    inline static wchar_t const SEPARATOR     { L',' };
    inline static wchar_t const CLOSE_BRACKET { L')' };

private:
    vector<ShapeId> m_list {};
};
