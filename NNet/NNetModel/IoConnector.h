// IoConnector.h
//
// NNetModel

#pragma once

#include <vector>
#include "BoolOp.h"
#include "MoreTypes.h"
#include "NobType.h"
#include "Nob.h"

using std::vector;
using std::unique_ptr;

class DrawContext;
class MicroMeterPosDir;
class IoNeuron;

class IoConnector: public Nob
{
public:
	static bool TypeFits(NobType const type) { return type.IsIoConnectorType(); }

	IoConnector(NobType const);

	virtual ~IoConnector() {}

	virtual void Check() const;
	virtual void Dump () const;

	virtual MicroMeterPnt GetPos() const;

	virtual void DrawExterior(DrawContext    const &, tHighlight const) const;
	virtual void DrawInterior(DrawContext    const &, tHighlight const) const;
	virtual void Expand      (MicroMeterRect       &)                   const;
	virtual bool IsIncludedIn(MicroMeterRect const &)                   const;
	virtual bool Includes    (MicroMeterPnt  const &)                   const;
	virtual void RotateNob   (MicroMeterPnt  const &, Radian const);
	virtual void MoveNob     (MicroMeterPnt  const &);
	virtual void Recalc      ();
	virtual void Clear       ();
	virtual void Link        (Nob const &, Nob2NobFunc const &);
	virtual void Select      (bool const);
	virtual void SetParentPointers();
	virtual void ClearParentPointers();

	virtual void Prepare     () {}
	virtual bool CompStep    () { return false; }

	virtual bool             IsCompositeNob() { return true; }
	virtual MicroMeterPosDir GetPosDir() const;

	void             Push(IoNeuron * const);
	IoNeuron       * Pop();
	IoNeuron const & GetElem(size_t const) const;
	size_t           Size() const;

	void LockDirection();
	void UnlockDirection();

	void AlignDirection();

	void Rotate(MicroMeterPnt const &, MicroMeterPnt const &);

	Radian GetDir() const;

	virtual void SetDir   (Radian           const );
	virtual void SetPos   (MicroMeterPnt    const &);
	virtual void SetPosDir(MicroMeterPosDir const &);

	void Apply2All(function<void(IoNeuron &)> const & func) const;

	MicroMeterLine CalcMaxDistLine() const;
	MicroMeterPnt  CalcOrthoVector(MicroMeterLine const &) const;

	friend wostream & operator<< (wostream &, IoConnector const &);

protected:
	vector<IoNeuron *> m_list {};
};

IoConnector const * Cast2IoConnector(Nob const *);
IoConnector       * Cast2IoConnector(Nob       *);
