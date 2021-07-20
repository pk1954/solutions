// IoNeuronList.cpp 
//
// NNetModel

#include "stdafx.h"
#include "IoNeuron.h"
#include "IoNeuronList.h"

using std::sort;
using std::wcout;
using std::endl;

void IoNeuronList::Check() const 
{ 
	for (auto & it : m_list) { it->Check(); }; 
}

void IoNeuronList::Dump () const
{
	wcout << OPEN_BRACKET; 
	for (auto & it : m_list) { wcout << it->GetId() << L' '; };
	wcout << CLOSE_BRACKET; 
}

MicroMeterPnt const IoNeuronList::GetPos() const 
{ 
	return (GetFirst().GetPos() + GetLast().GetPos()) * 0.5f; 
}

void IoNeuronList::Replace(IoNeuron * const pDel, IoNeuron * const pAdd) 
{ 
	replace( begin(m_list), end(m_list), pDel, pAdd ); 
}

void IoNeuronList::Add( IoNeuron * const pNob )
{
	if ( pNob != nullptr )
	{
		assert( find( begin(m_list), end(m_list), pNob ) == end(m_list) );
		m_list.push_back( pNob );
	}
}

void IoNeuronList::Remove( IoNeuron * const pNob )
{
	auto res = find( begin(m_list), end(m_list), pNob );
	assert( res != end(m_list) );
	m_list.erase( res );
}

void IoNeuronList::Apply2All( function<void(IoNeuron &)> const & func ) const
{
	for ( auto pNob : m_list ) 
	{ 
		if ( pNob != nullptr )
			func( * pNob );
	}
}

bool IoNeuronList::Apply2AllB( function<bool(IoNeuron const &)> const & func ) const 
{
	bool bResult { false };
	for ( auto pNob : m_list ) 
	{ 
		if ( pNob != nullptr )
		{
			if ( func( * pNob ) )
				return true;
		}
	}
	return false;
}

bool IoNeuronList::operator==(IoNeuronList const & rhs) const
{
	if ( Size() != rhs.Size() )
		return false;
	for ( int i = 0; i < Size(); ++i )
		if ( m_list[i] != rhs.m_list[i] )
			return false;
	return true;
}

wostream & operator<< (wostream & out, IoNeuronList const & v)
{
	out << IoNeuronList::OPEN_BRACKET << v.m_list.size();
	if (v.m_list.size() > 0)
		out << L":";
	for ( auto & it: v.m_list )
	{
		out << it->GetId();
		if ( &it == &v.m_list.back() )
			break;
		out << IoNeuronList::SEPARATOR;
	}
	out << IoNeuronList::CLOSE_BRACKET;
	return out;
}

MicroMeterLine const IoNeuronList::CalcMaxDistLine() // find two nobs with maximum distance
{
	MicroMeter     maxDist { 0.0_MicroMeter };   	
	MicroMeterLine lineMax { MicroMeterLine::ZERO_VAL() };
	for ( IoNeuron * it1 : m_list )
		for ( IoNeuron * it2 : m_list )    //TODO: optimize
		{
			auto const line { MicroMeterLine( it1->GetPos(), it2->GetPos() ) };
			auto const dist { line.Length() };
			if ( dist > maxDist )
			{
				maxDist = dist;
				lineMax = line;
			}
		}
	return lineMax;
}

MicroMeterPnt const IoNeuronList::CalcOrthoVector(MicroMeterLine const & line)
{
	unsigned int uiLeftConnections  { 0 };
	unsigned int uiRightConnections { 0 };
	for (auto pIoNeuron : m_list)
	{ 
		pIoNeuron->Apply2AllInPipes
		( 
			[&](Pipe & pipe) 
			{ 
				MicroMeterPnt pnt { pipe.GetStartPoint() };
				if ( PointToLine(line, pnt) < 0.0_MicroMeter )
					++uiLeftConnections;
				else
					++uiRightConnections;
			}
		);
		pIoNeuron->Apply2AllOutPipes
		( 
			[&](Pipe & pipe) 
			{ 
				MicroMeterPnt pnt { pipe.GetEndPoint() };
				if ( PointToLine(line, pnt) < 0.0_MicroMeter )
					++uiRightConnections;
				else
					++uiLeftConnections;
			}
		);
	}	

	MicroMeterPnt orthoVector = line.OrthoVector();
	if ( uiRightConnections < uiLeftConnections )
		orthoVector = -orthoVector;
	return orthoVector;
}

void IoNeuronList::SortAccToDistFromLine(MicroMeterLine const & line)
{
	sort
	( 
		m_list.begin(), m_list.end(),
		[&](auto & p1, auto & p2) 
		{ 
			return PointToLine(line, p1->GetPos()) < PointToLine(line, p2->GetPos()); 
		} 
	);
}

void IoNeuronList::AlignDirection()
{
	MicroMeterLine const umLine(GetFirst().GetPos(), GetLast().GetPos());
	MicroMeterPnt  const umPntDir { CalcOrthoVector(umLine) };
	for (auto it : m_list) { it->SetDirVector(umPntDir); }
}

void IoNeuronList::Link(Nob2NobFunc const & dstFromSrc)
{
	for (int i = 0; i < m_list.size(); ++i)
		m_list[i] = static_cast<IoNeuron *>(dstFromSrc(m_list[i]));
}

bool const IoNeuronList::IsIncludedIn(MicroMeterRect const & umRect) const 
{
	for (auto it : m_list) { if (it->IsIncludedIn(umRect)) return true; }
	return false;
}

bool const IoNeuronList::Includes(MicroMeterPnt const & umPnt) const
{
	for (auto it : m_list) { if (it->Includes(umPnt)) return true; }
	return false;
}

void IoNeuronList::DrawExterior(DrawContext const & context, tHighlight const type) const
{
	for (auto it : m_list) { it->DrawExterior(context, type); }
}

void IoNeuronList::DrawInterior(DrawContext const & context, tHighlight const type) const
{
	for (auto it : m_list) { it->DrawInterior(context, type); }
}

void IoNeuronList::Expand(MicroMeterRect & umRect) const
{
	for (auto it : m_list) { umRect.Expand(it->GetPos()); }
}

void IoNeuronList::RotateNob(MicroMeterPnt const & umPntPivot, Radian const radDelta)
{
	for (auto it : m_list) { it->RotateNob(umPntPivot, radDelta); }
}

void IoNeuronList::MoveNob(MicroMeterPnt const & delta)       
{
	for (auto it : m_list) { it->MoveNob(delta); }
}

void IoNeuronList::Prepare()
{
	for (auto it : m_list) { it->Prepare(); }
}

bool const IoNeuronList::CompStep()
{
	for (auto it : m_list) { if (it->CompStep()) return true; }
	return false;
}

void IoNeuronList::Recalc()
{
	for (auto it : m_list) { it->Recalc(); }
}

void IoNeuronList::SetParentPointers(Nob * const pNob)
{
	for (auto it : m_list) { it->SetParentNob(pNob); }
}

void IoNeuronList::ClearParentPointers()
{
	for (auto it : m_list) { it->SetParentNob(nullptr); }
}
