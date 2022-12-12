// PosNob.cpp
//
// NNetModel

module;

#include <cassert>
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
	bool bRes = Apply2AllOutPipesB([&pipeSucc](Pipe const& pipe) { return &pipe == &pipeSucc; });
	return bRes;
}

bool PosNob::IsSuccessorOf(Pipe const& pipePred) const
{
	bool bRes = Apply2AllInPipesB([&pipePred](Pipe const& pipe) { return &pipe == &pipePred; });
	return bRes;
}

bool PosNob::IsPrecursorOf(PosNob const& b) const
{
	bool bRes = Apply2AllOutPipesB([&b](Pipe const& p) { return p.GetEndNobPtr() == &b; });
	return bRes;
}

bool PosNob::IsSuccessorOf(PosNob const& b) const
{
	bool bRes = Apply2AllInPipesB([&b](Pipe const& p) { return p.GetStartNobPtr() == &b; });
	return bRes;
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

bool PosNob::Apply2AllConnectedPipesB(PipeCrit const& c) const
{
	return Apply2AllInPipesB(c) || Apply2AllOutPipesB(c);
}

PosNob const* Cast2PosNob(Nob const* pNob)
{
	assert(pNob);
	assert(pNob->IsPosNob());
	return static_cast<PosNob const*>(pNob);
}

PosNob* Cast2PosNob(Nob* pNob)
{
	assert(pNob);
	assert(pNob->IsPosNob());
	return static_cast<PosNob*>(pNob);
}
