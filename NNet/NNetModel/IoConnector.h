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
using std::ranges::for_each;

class DrawContext;
class MicroMeterPosDir;
class IoNeuron;

class IoConnector: public Nob
{
public:
	static bool TypeFits(NobType const type) { return type.IsIoConnectorType(); }

	explicit IoConnector(NobType const);

	~IoConnector() override = default;

	void AppendMenuItems(AddMenuFunc const &) const override;

	void Check() const override;
	void Dump () const override;

	MicroMeterPnt GetPos() const override;

	void DrawExterior(DrawContext    const &, tHighlight const) const override;
	void DrawInterior(DrawContext    const &, tHighlight const) const override;
	void Expand      (MicroMeterRect       &)                   const override;
	bool IsIncludedIn(MicroMeterRect const &)                   const override;
	bool Includes    (MicroMeterPnt  const &)                   const override;
	void RotateNob   (MicroMeterPnt  const &, Radian const)           override;
	void MoveNob     (MicroMeterPnt  const &)                         override;
	void Recalc      ()                                               override;
	void Link        (Nob const &, Nob2NobFunc const &)               override;
	void Select      (bool const)                                     override;
	void Prepare     () override { /* nothing to prepare */ }
	bool CompStep    () override { return false; }

	bool             IsCompositeNob() const final { return true; }
	MicroMeterPosDir GetPosDir     () const final;

	void             Push(IoNeuron * const);
	IoNeuron       * Pop();
	IoNeuron       & GetElem(size_t const) const;
	size_t           Size() const;
	void             SetParentPointers();
	void             ClearParentPointers() const;

	void AlignDirection();

	size_t GetNrOfElements() const { return m_list.size(); }

	void Rotate(MicroMeterPnt const &, MicroMeterPnt const &);

	Radian GetDir() const override;

	void SetDir   (Radian           const  ) override;
	void SetPos   (MicroMeterPnt    const &) override;
	void SetPosDir(MicroMeterPosDir const &) override;

	void Apply2All(auto const & f) const
	{
		for_each(m_list, [&f](auto * const p) { if (p) f(*p); } );
	}                        

	MicroMeterLine CalcMaxDistLine() const;
	MicroMeterPnt  CalcOrthoVector(MicroMeterLine const &) const;

	friend wostream & operator<< (wostream &, IoConnector const &);

protected:
	vector<IoNeuron *> m_list {};
};

IoConnector const * Cast2IoConnector(Nob const *);
IoConnector       * Cast2IoConnector(Nob       *);
