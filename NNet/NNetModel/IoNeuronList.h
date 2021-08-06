// IoNeuronList.h
//
// NNetModel

#pragma once

#include <vector>
#include "Nob.h"
#include "Pipe.h"
#include "MoreTypes.h"
#include "IoNeuron.h"

using std::vector;

class DrawContext;

class IoNeuronList
{
public:
	IoNeuronList() {}
	IoNeuronList(IoNeuronList const & src) { m_list = src.m_list; }
	virtual ~IoNeuronList() {}

	bool operator==(IoNeuronList const &) const;

	size_t const Size()    const { return m_list.size (); }
	bool   const IsEmpty() const { return m_list.empty(); }
	Radian const GetDir()  const; 

	IoNeuron       & GetFirst()       { return * m_list.front(); }
	IoNeuron const & GetFirst() const { return * m_list.front(); }

	IoNeuron       & GetLast()       { return * m_list.back(); }
	IoNeuron const & GetLast() const { return * m_list.back(); }

	IoNeuron       & GetElem(size_t const i)       { return * m_list.at(i); }
	IoNeuron const & GetElem(size_t const i) const { return * m_list.at(i); }

	void Check() const;
	void Dump () const;
	
	MicroMeterPnt const GetPos() const;

	void Clear()      {	m_list.clear(); }
	void RemoveLast() {	m_list.pop_back(); }

	void Replace(IoNeuron * const, IoNeuron * const); 
	void Add    (IoNeuron * const);
	void Remove (IoNeuron * const);

	void         Apply2All (function<void(IoNeuron       &)> const &) const;
	bool   const Apply2AllB(function<bool(IoNeuron const &)> const &) const; 

	size_t const Count (NobType const nobType) const;

	MicroMeterLine const CalcMaxDistLine() const // find two nobs with maximum distance
	{
		return ::CalcMaxDistLine<IoNeuron>(m_list);
	}

	MicroMeterPnt const CalcOrthoVector(MicroMeterLine const& line) const
	{
		return ::CalcOrthoVector<IoNeuron>(m_list, line);
	}

	void       SortAccToDistFromLine(MicroMeterLine const &);
	void       AlignDirection();
	void       Link        (Nob2NobFunc    const &);
	void       Expand      (MicroMeterRect       &) const;
	bool const IsIncludedIn(MicroMeterRect const &) const; 
	bool const Includes    (MicroMeterPnt  const &) const;
	void       RotateNobs  (MicroMeterPnt  const &, Radian const);
	void       MoveNobs    (MicroMeterPnt  const &);
	void       DrawExterior(DrawContext    const &, tHighlight const) const;
	void       DrawInterior(DrawContext    const &, tHighlight const) const;
	void       Prepare();
	bool const CompStep();
	void       Recalc();
	void       Reverse();
	void       SetParentPointers(Nob * const);
	void       ClearParentPointers();
	void       SelectAll(bool const);

	friend wostream & operator<< (wostream &, IoNeuronList const &);

private:
	vector<IoNeuron *> m_list;
};
