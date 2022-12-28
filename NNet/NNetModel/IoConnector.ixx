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
	void PosChanged  ()                                               override;
	void Link        (Nob const &, Nob2NobFunc const &)               override;
	void Select      (bool const)                                     override;
	bool CompStep    ()                                               override { return false; }

	bool             IsCompositeNob() const final { return true; }
	MicroMeterPosDir GetPosDir() const final;

	IoLine * Pop();
	void     Push(IoLine* const p)         { m_list.push_back(p); }
	IoLine & GetElem(size_t const n) const { return *m_list.at(n); }
	size_t   Size ()                 const { return m_list.size(); }
	bool     Empty()                 const { return m_list.empty(); }
	size_t   GetNrOfElements()       const { return m_list.size(); }
	void     ClearParentPointers()   const;
	void     SetParentPointers();

	void AlignDirection();

	void Rotate(MicroMeterPnt const&, MicroMeterPnt const&);

	Radian GetDir() const override;

	void SetDir   (Radian           const  ) override;
	void SetPos   (MicroMeterPnt    const &) override;
	void SetPosDir(MicroMeterPosDir const &) override;

	void Apply2All(auto const& func)
	{
		for_each(m_list, [&func](auto* p) { if (p) func(*p); });
	}

	void SelectAllConnected(bool const bFirst, bool const bOn) final
	{
		if ((IsSelected() != bOn) || bFirst)
		{
			Nob::Select(bOn);
			for_each
			(
				m_list,
				[bOn](IoLine* p) { if (p) p->SelectAllConnected(false, bOn); }
			);
		}
	}

	MicroMeterLine CalcMaxDistLine() const;

	friend wostream& operator<< (wostream &, IoConnector const &);

protected:
	vector<IoLine *> m_list {};
};

export IoConnector const * Cast2IoConnector(Nob const *);
export IoConnector       * Cast2IoConnector(Nob       *);
