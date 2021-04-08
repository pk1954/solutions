// Compound.cpp 
//
// NNetModel

#include "stdafx.h"
#include "Compound.h"

using std::wcout;
using std::endl;

void Compound::CheckShape() const
{
    Shape::CheckShape();
}

void Compound::Dump() const
{
    Shape::Dump();
    Apply2All([&](Shape const & s){ wcout << s << endl; } );
}

void Compound::Apply2All(function<void(Shape &)> const & func)
{
    m_list.Apply2All([&](Shape & s){ func(s); } );
}

void Compound::Apply2All(function<void(Shape const &)> const & func) const
{
    m_list.Apply2All([&](Shape const & s){ func(s); } );
}

void Compound::SetParentPointers()
{
    Apply2All([&](Shape & n){ n.SetParent(this); } );
}

void Compound::ClearParentPointers()
{
    Apply2All([&](Shape & n){ n.SetParent(nullptr); } );
}

void Compound::DrawExterior(DrawContext const & context, tHighlight const type) const
{
    Apply2All([&](Shape const & s){ s.DrawExterior(context, type); } );
}

void Compound::DrawInterior(DrawContext const & context, tHighlight const type) const
{
    Apply2All([&](Shape const & s){ s.DrawInterior(context, type); } );
}

void Compound::MoveShape(MicroMeterPoint const & delta)       
{
    Apply2All([&](Shape & s){ s.MoveShapeFromParent(delta); } );
}

void Compound::Expand(MicroMeterRect & umRect) const
{
    Apply2All([&](Shape const & s){ umRect.Expand(s.GetPosition()); } );
}

void Compound::Select(bool const bOn, bool const bRecursive) 
{ 
    Shape::Select(bOn);
    if (bRecursive)
        Apply2All([&](Shape & s){ s.Select(bOn, false); } );
}
