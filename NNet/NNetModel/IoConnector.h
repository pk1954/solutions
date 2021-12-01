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

	explicit IoConnector(NobType const);

	~IoConnector() override = default;

	void Check() const override;
	void Dump () const override;

	MicroMeterPnt GetPos() const override;

	void DrawExterior(DrawContext    const &, tHighlight const) const override;
	void DrawInterior(DrawContext    const &, tHighlight const) const override;
	void Expand      (MicroMeterRect       &)                   const override;
	bool IsIncludedIn(MicroMeterRect const &)                   const override;
	bool Includes    (MicroMeterPnt  const &)                   const override;
	void RotateNob   (MicroMeterPnt  const &, Radian const) override;
	void MoveNob     (MicroMeterPnt  const &) override;
	void Recalc      () override;
	void Link        (Nob const &, Nob2NobFunc const &) override;
	void Select      (bool const) override;
	void SetParentPointers();
	void ClearParentPointers();

	void Prepare     () override {}
	bool CompStep    () override { return false; }

	bool             IsCompositeNob() const final { return true; }
	MicroMeterPosDir GetPosDir     () const final;

	void             Push(IoNeuron * const);
	IoNeuron       * Pop();
	IoNeuron const & GetElem(size_t const) const;
	size_t           Size() const;

	void LockDirection();
	void UnlockDirection();
	void AlignDirection();

	void Rotate(MicroMeterPnt const &, MicroMeterPnt const &);

	Radian GetDir() const override;

	void SetDir   (Radian           const  ) override;
	void SetPos   (MicroMeterPnt    const &) override;
	void SetPosDir(MicroMeterPosDir const &) override;

	void Apply2All(function<void(IoNeuron &)> const & func) const;

	MicroMeterLine CalcMaxDistLine() const;
	MicroMeterPnt  CalcOrthoVector(MicroMeterLine const &) const;

	friend wostream & operator<< (wostream &, IoConnector const &);

protected:
	vector<IoNeuron *> m_list {};
};

IoConnector const * Cast2IoConnector(Nob const *);
IoConnector       * Cast2IoConnector(Nob       *);
