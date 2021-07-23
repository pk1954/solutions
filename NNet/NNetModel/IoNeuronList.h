// IoNeuronList.h
//
// NNetModel

#pragma once

#include <vector>
#include "Nob.h"
#include "MoreTypes.h"

using std::vector;

class IoNeuron;
class DrawContext;

class IoNeuronList
{
public:

	IoNeuronList() {}
	virtual ~IoNeuronList() {}

	bool operator==(IoNeuronList const &) const;

	size_t const Size()    const { return m_list.size (); }
	bool   const IsEmpty() const { return m_list.empty(); }

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

	MicroMeterLine const CalcMaxDistLine();
	MicroMeterPnt  const CalcOrthoVector(MicroMeterLine const &);

	void       SortAccToDistFromLine(MicroMeterLine const &);
	void       AlignDirection();
	void       Link        (Nob2NobFunc const &);
	void       Expand      (MicroMeterRect       &) const;
	bool const IsIncludedIn(MicroMeterRect const &) const; 
	bool const Includes    (MicroMeterPnt  const &) const;
	void       RotateNob   (MicroMeterPnt  const &, Radian const);
	void       MoveNob     (MicroMeterPnt  const &);
	void       DrawExterior(DrawContext const &, tHighlight const) const;
	void       DrawInterior(DrawContext const &, tHighlight const) const;
	void       Prepare();
	bool const CompStep();
	void       Recalc();
	void       SetParentPointers(Nob * const);
	void       ClearParentPointers();

	inline static wchar_t const OPEN_BRACKET  { L'(' };
	inline static wchar_t const SEPARATOR     { L',' };
	inline static wchar_t const CLOSE_BRACKET { L')' };

	friend wostream & operator<< (wostream &, IoNeuronList const &);

private:
	vector<IoNeuron *> m_list;
};
