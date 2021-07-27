// Connector.h
//
// NNetModel

#pragma once

#include <vector>
#include "BoolOp.h"
#include "MoreTypes.h"
#include "NobType.h"

using std::vector;
using std::unique_ptr;

class IoNeuronList;
class DrawContext;
class MicroMeterPosDir;
class IoNeuron;

class Connector: public Nob
{
public:

	static bool    const TypeFits(NobType const type) { return type.IsConnectorType(); }
	static NobType const GetNobType()                 { return NobType::Value::connector; }

	Connector(NobIoMode const);
	Connector(unique_ptr<IoNeuronList>);
	Connector(Connector const &);   // copy constructor

	virtual ~Connector() {}

	virtual void Check() const;
	virtual void Dump () const;

	virtual NobIoMode     const GetIoMode() const;
	virtual MicroMeterPnt const GetPos() const;

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

	virtual bool const IsCompositeNob() { return true; }

	void               Push(IoNeuron * const);
	IoNeuron * const   Pop();
	IoNeuron   const & GetElem(size_t const) const;
	size_t     const Size() const;

	bool const IsInputConnector () const;
	bool const IsOutputConnector() const;

	void SetParentPointers();
	void ClearParentPointers();
	void AlignDirection();

	void Rotate(MicroMeterPnt const &, MicroMeterPnt const &);

	MicroMeterPosDir const GetPosDir() const;
	Radian           const GetDir()    const;

	void SetDir   (Radian           const  );
	void SetPos   (MicroMeterPnt    const &);
	void SetPosDir(MicroMeterPosDir const &);

	void Apply2All(function<void(IoNeuron const &)> const & func) const;

	friend wostream & operator<< (wostream &, Connector const &);

private:
	NobIoMode          const m_IoMode;
	unique_ptr<IoNeuronList> m_upList {};
};

Connector const * Cast2Connector(Nob const *);
Connector       * Cast2Connector(Nob       *);
