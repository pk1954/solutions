// ClosedConnector.h
//
// NNetModel

#pragma once

#include <vector>
#include "BoolOp.h"
#include "MoreTypes.h"
#include "Nob.h"
#include "IoNeuronList.h"
#include "NobType.h"

using std::vector;

class Neuron;
class IoConnector;

class ClosedConnector: public Nob
{
public:

	static bool const TypeFits(NobType const type) { return type.IsClosedConnectorType(); }

	ClosedConnector() :	Nob(NobType::Value::closedConnector) {};
	virtual ~ClosedConnector() {}

	virtual void Check() const;
	virtual void Dump () const;

	virtual MicroMeterPnt const GetPos() const;
	virtual Radian        const GetDir() const;

	virtual void       DrawExterior(DrawContext    const &, tHighlight const) const;
	virtual void       DrawInterior(DrawContext    const &, tHighlight const) const;
	virtual void       Expand      (MicroMeterRect       &)                   const;
	virtual bool const IsIncludedIn(MicroMeterRect const &)                   const;
	virtual bool const Includes    (MicroMeterPnt  const &)                   const;
	virtual void       RotateNob   (MicroMeterPnt  const &, Radian const);
	virtual void       MoveNob     (MicroMeterPnt  const &);
	virtual void       Prepare     ();
	virtual bool const CompStep    ();
	virtual void       Recalc      ();
	virtual void       Clear       ();
	virtual void       Link        (Nob const &, Nob2NobFunc const &);
	virtual void       Select      (bool const);

	virtual NobIoMode const GetIoMode() const { return NobIoMode::internal; }

	virtual bool const IsCompositeNob() { return true; }

	void Push(Neuron * const p) { m_list.push_back(p); }
	Neuron * const Pop();

	void Apply2All(function<void(Neuron const &)> const & func) const;

	void SetParentPointers();
	void ClearParentPointers();

	size_t const Size() const { return m_list.size(); };

	MicroMeterLine const CalcMaxDistLine() // find two nobs with maximum distance
	{
		return ::CalcMaxDistLine<Neuron>(m_list);
	}

	MicroMeterPnt const CalcOrthoVector(MicroMeterLine const & line)
	{
		return ::CalcOrthoVector<Neuron>(m_list, line);
	}

	vector<Neuron *> const & GetNeurons() const { return m_list; }

	friend wostream & operator<< (wostream &, ClosedConnector const &);

private:
	vector<Neuron *> m_list{};
};

ClosedConnector const * Cast2ClosedConnector(Nob const *);
ClosedConnector       * Cast2ClosedConnector(Nob       *);
