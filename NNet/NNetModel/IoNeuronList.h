// IoNeuronList.h
//
// NNetModel

#pragma once

#include <vector>
#include "MoreTypes.h"
#include "IoNeuron.h"

using std::vector;
using std::sort;
using std::wcout;
using std::endl;

class IoNeuronList
{
public:

	IoNeuronList() {}
	virtual ~IoNeuronList() {}

	size_t const Size()    const { return m_list.size (); }
	bool   const IsEmpty() const { return m_list.empty(); }

	IoNeuron       & GetFirst()       { return * m_list.front(); }
	IoNeuron const & GetFirst() const { return * m_list.front(); }

	IoNeuron       & GetLast()       { return * m_list.back(); }
	IoNeuron const & GetLast() const { return * m_list.back(); }

	IoNeuron       & GetElem(size_t const i)       { return * m_list.at(i); }
	IoNeuron const & GetElem(size_t const i) const { return * m_list.at(i); }

	void Check() const { for (auto & it : m_list) { it->Check(); }; }
	void Dump () const
	{
		wcout << endl; 
		for (auto & it : m_list) { wcout << L"       " << Scanner::COMMENT_SYMBOL << it << endl; };
	}

	MicroMeterPnt const GetPos() const 
	{ 
		return (GetFirst().GetPos() + GetLast().GetPos()) * 0.5f; 
	}

	void Clear()      {	m_list.clear(); }
	void RemoveLast() {	m_list.pop_back(); }
	void Replace(IoNeuron * const pDel, IoNeuron * const pAdd) 
	{ 
		replace( begin(m_list), end(m_list), pDel, pAdd ); 
	}

	void Add( IoNeuron * const pNob )
	{
		if ( pNob != nullptr )
		{
			assert( find( begin(m_list), end(m_list), pNob ) == end(m_list) );
			m_list.push_back( pNob );
		}
	}

	void Remove( IoNeuron * const pNob )
	{
		auto res = find( begin(m_list), end(m_list), pNob );
		assert( res != end(m_list) );
		m_list.erase( res );
	}

	void Apply2All( function<void(IoNeuron &)> const & func ) const
	{
		for ( auto pNob : m_list ) 
		{ 
			if ( pNob != nullptr )
				func( * pNob );
		}
	}

	bool Apply2AllB( function<bool(IoNeuron const &)> const & func ) const 
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

	bool operator==(IoNeuronList const & rhs) const
	{
		if ( Size() != rhs.Size() )
			return false;
		for ( int i = 0; i < Size(); ++i )
			if ( m_list[i] != rhs.m_list[i] )
				return false;
		return true;
	}

	friend wostream & operator<< (wostream & out, IoNeuronList const & v)
	{
		out << OPEN_BRACKET << v.m_list.size();
		if (v.m_list.size() > 0)
			out << L":";
		for ( auto & it: v.m_list )
		{
			out << it->GetId();
			if ( &it == &v.m_list.back() )
				break;
			out << SEPARATOR;
		}
		out << CLOSE_BRACKET;
		return out;
	}

	MicroMeterLine const CalcMaxDistLine() // find two nobs with maximum distance
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

	MicroMeterPnt const CalcOrthoVector(MicroMeterLine const & line)
	{
		unsigned int uiLeftConnections  { 0 };
		unsigned int uiRightConnections { 0 };
		for (auto pIoNeuron : m_list)
		{ 
			pIoNeuron->m_connections.Apply2AllInPipes
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
			pIoNeuron->m_connections.Apply2AllOutPipes
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
	void SortAccToDistFromLine(MicroMeterLine const & line)
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

	void AlignDirection()
	{
		MicroMeterLine const umLine(GetFirst().GetPos(), GetLast().GetPos());
		MicroMeterPnt  const umPntDir { CalcOrthoVector(umLine) };
		for (auto it : m_list) { it->SetDirVector(umPntDir); }
	}

	void Link(Nob2NobFunc const & dstFromSrc)
	{
		Clear();
		for (auto & it : m_list) { it = static_cast<IoNeuron *>(dstFromSrc(it)); }
	}

	bool const IsIncludedIn(MicroMeterRect const & umRect) const 
	{
		bool bRes { false };
		for (auto it : m_list) { if (it->IsIncludedIn(umRect)) bRes = true; }
		return bRes;
	}

	bool const Includes(MicroMeterPnt const & umPnt) const
	{
		bool bRes { false };
		for (auto it : m_list) { if (it->Includes(umPnt)) bRes = true; }
		return bRes;
	}

	void DrawExterior(DrawContext const & context, tHighlight const type) const
	{
		for (auto it : m_list) { it->DrawExterior(context, type); }
	}

	void DrawInterior(DrawContext const & context, tHighlight const type) const
	{
		for (auto it : m_list) { it->DrawInterior(context, type); }
	}

	void Expand(MicroMeterRect & umRect) const
	{
		for (auto it : m_list) { umRect.Expand(it->GetPos()); }
	}

	void RotateNob(MicroMeterPnt const & umPntPivot, Radian const radDelta)
	{
		for (auto it : m_list) { it->RotateNob(umPntPivot, radDelta); }
	}

	void MoveNob(MicroMeterPnt const & delta)       
	{
		for (auto it : m_list) { it->MoveNob(delta); }
	}

	void Prepare()
	{
		for (auto it : m_list) { it->Prepare(); }
	}

	bool const CompStep()
	{
		bool bStop { false };
		for (auto it : m_list) { if (it->CompStep()) bStop = true; }
		return bStop;
	}

	void Recalc()
	{
		for (auto it : m_list) { it->Recalc(); }
	}

	void SetParentPointers(Nob * const pNob)
	{
		for (auto it : m_list) { it->SetParentNob(pNob); }
	}

	void ClearParentPointers()
	{
		for (auto it : m_list) { it->SetParentNob(nullptr); }
	}

	inline static wchar_t const OPEN_BRACKET  { L'(' };
	inline static wchar_t const SEPARATOR     { L',' };
	inline static wchar_t const CLOSE_BRACKET { L')' };

private:
	vector<IoNeuron *> m_list;
};
