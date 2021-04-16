// ShapeIdList.h
//
// NNetModel

#pragma once

#include <vector>
#include "util.h"
#include "ShapeId.h"
#include "ShapePtrList.h"
#include "Connector.h"
#include "ConnectionNeuron.h"

using std::vector;
using std::endl;

class Shape;

class ShapeIdList
{
public:
    ShapeIdList() {}
    ShapeIdList(Connector                const &);
    ShapeIdList(ShapePtrList<Shape>      const &);
    ShapeIdList(ShapePtrList<ConnNeuron> const &);

    virtual ~ShapeIdList() {}

    void          Add   (ShapeId const   id   ) { m_list.push_back(id); }
    void          Add   (Shape   const & shape) { Add(shape.GetId()); }
    void          Resize(size_t  const   size ) { m_list.resize(size); }
    void          SetAt (int const index, ShapeId const id) { m_list.at(index) = id; }
    int     const Size() { return Cast2Int(m_list.size()); }
    ShapeId const Get(int const index) { return m_list.at(index); }

    void Apply2All( function<void(ShapeId const &)> const& func ) const;

    friend wostream & operator<< (wostream &, ShapeIdList const &);

    inline static wchar_t const OPEN_BRACKET  { L'(' };
    inline static wchar_t const SEPARATOR     { L',' };
    inline static wchar_t const CLOSE_BRACKET { L')' };

private:
    vector<ShapeId> m_list {};
};
