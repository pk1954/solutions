// PosNob.cpp
//
// NNetModel

module;

#include <iostream>

module NNetModel:PosNob;

using std::wcout;
using std::endl;

void PosNob::Dump() const
{
	Nob::Dump();
	wcout << GetPos() << endl;
}

bool PosNob::operator==(Nob const& rhs) const
{
	PosNob const& posNobRhs { static_cast<PosNob const&>(rhs) };
	return (this->Nob::operator==(rhs)) &&
		GetPos().IsCloseTo(posNobRhs.GetPos()) &&
		GetExtension().IsCloseTo(posNobRhs.GetExtension());
}

bool PosNob::Includes(MicroMeterPnt const& point) const
{
	return Distance(point, GetPos()) <= GetExtension();
}

bool PosNob::IsIncludedIn(MicroMeterRect const& umRect) const
{
	return umRect.Includes(GetPos());
}

void PosNob::Expand(MicroMeterRect& umRect) const
{
	umRect.Expand(GetPos());
}

bool PosNob::IsPrecursorOf(Pipe const& pipeSucc) const
{
	return Apply2AllOutPipesB([&pipeSucc](Pipe const& pipe) { return &pipe == &pipeSucc; });
}

bool PosNob::IsSuccessorOf(Pipe const& pipePred) const
{
	return Apply2AllInPipesB([&pipePred](Pipe const& pipe) { return &pipe == &pipePred; });
}

bool PosNob::IsPrecursorOf(PosNob const& b) const
{
	return Apply2AllOutPipesB([&b](Pipe const& p) { return p.GetEndNobPtr() == &b; });
}

bool PosNob::IsSuccessorOf(PosNob const& b) const
{
	return Apply2AllInPipesB([&b](Pipe const& p) { return p.GetStartNobPtr() == &b; });
}

bool PosNob::IsDirectlyConnectedTo(PosNob const& posNob) const
{
	return IsSuccessorOf(posNob) || IsPrecursorOf(posNob);
}

bool PosNob::IsDirectlyConnectedTo(Pipe const& pipe) const
{
	return IsDirectlyConnectedTo(*static_cast<PosNob const*>(pipe.GetStartNobPtr())) ||
		IsDirectlyConnectedTo(*static_cast<PosNob const*>(pipe.GetEndNobPtr()));
}

void PosNob::EvaluateSelectionStatus()
{
	bool bSelected { Apply2AllConnectedPipesB([](Pipe const& p) { return p.IsSelected(); }) };
	Nob::Select(bSelected);      // if any connected pipe is selected
}                                // PosNob must also be selected
