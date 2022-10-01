// IoConnector.ixx
//
// NNetModel

module;

#include <vector>
#include <algorithm>
#include <iostream>

export module NNetModel:IoConnector;

import BoolOp;
import DrawContext;
import Types;
import :tHighlight;
import :MicroMeterPosDir;
import :NobType;
import :Nob;
import :IoLine;

using std::vector;
using std::wostream;
using std::unique_ptr;
using std::ranges::for_each;

export class IoConnector : public Nob
{
public:
	static bool TypeFits(NobType const type) { return type.IsIoConnectorType(); }

	explicit IoConnector(NobType const);

	~IoConnector() override = default;

	void AppendMenuItems(AddMenuFunc const&) const override;

	void Check() const override;
	void Dump() const override;

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
	bool CompStep    ()                                               override { return false; }

	bool             IsCompositeNob() const final { return true; }
	MicroMeterPosDir GetPosDir() const final;

	void             Push(IoLine* const);
	IoLine         * Pop();
	IoLine         & GetElem(size_t const) const;
	size_t           Size() const;
	void             SetParentPointers();
	void             ClearParentPointers() const;

	void AlignDirection();

	size_t GetNrOfElements() const { return m_list.size(); }

	void Rotate(MicroMeterPnt const&, MicroMeterPnt const&);

	Radian GetDir() const override;

	void SetDir   (Radian           const  ) override;
	void SetPos   (MicroMeterPnt    const &) override;
	void SetPosDir(MicroMeterPosDir const &) override;

	void Apply2All(auto const& func)
	{
		for_each(m_list, [&func](auto* p) { if (p) func(*p); });
	}

	MicroMeterLine CalcMaxDistLine() const;
	MicroMeterPnt  CalcOrthoVector(MicroMeterLine const &) const;

	friend wostream& operator<< (wostream &, IoConnector const &);

protected:
	vector<IoLine *> m_list {};
};

export IoConnector const * Cast2IoConnector(Nob const *);
export IoConnector       * Cast2IoConnector(Nob       *);
