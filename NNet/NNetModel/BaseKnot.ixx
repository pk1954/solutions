// BaseKnot.ixx
//
// NNetModel

module;

#include <type_traits>
#include <vector>
#include <compare>

export module NNetModel:BaseKnot;

import Types;
import DrawContext;
import :Pipe;
import :PipeList;
import :NobType;
import :Nob;

using std::vector;
using std::is_base_of;
using std::remove_pointer_t;

export class BaseKnot : public Nob
{
public:

	BaseKnot(MicroMeterPnt const &, NobType const, MicroMeter const);
	BaseKnot(BaseKnot const &) = default;
	~BaseKnot() override = default;

	virtual bool operator==(Nob const &) const;

	virtual BaseKnot & operator*=(float const);
	virtual BaseKnot & operator+=(BaseKnot const &);
	virtual BaseKnot & operator-=(BaseKnot const &);

	void AppendMenuItems(AddMenuFunc const&) const override;

	MicroMeterPnt GetPos()      const final { return m_circle.GetPos(); }
	mV            GetVoltage()  const       { return m_mVinputBuffer; }
	void          SetVoltage(mV const v)    { m_mVinputBuffer = v; }

	void Dump()                                       const override;
	void Check()                                      const override;
	void Reconnect()                                        override;
	void SetPos      (MicroMeterPnt  const &)               override;
	void MoveNob     (MicroMeterPnt  const &)               override;
	bool Includes    (MicroMeterPnt  const &)         const override;
	bool IsIncludedIn(MicroMeterRect const &)         const override;
	void Expand      (MicroMeterRect       &)         const override;
	void RotateNob   (MicroMeterPnt  const &, Radian const) override;
	void Link        (Nob const &, Nob2NobFunc const &)     override;

	static bool TypeFits(NobType const type) { return type.IsBaseKnotType(); }

	MicroMeterCircle GetCircle()    const { return m_circle; }
	MicroMeter       GetExtension() const { return m_circle.GetRadius(); }

	void SetExtension(MicroMeter const um) { m_circle.SetRadius(um); }

	bool IsPrecursorOf(Pipe const &) const;
	bool IsSuccessorOf(Pipe const &) const;

	bool IsPrecursorOf(BaseKnot const &) const;
	bool IsSuccessorOf(BaseKnot const &) const;

	bool IsDirectlyConnectedTo(BaseKnot const &) const;
	bool IsDirectlyConnectedTo(Pipe     const &) const;

	bool HasIncoming() const { return m_inPipes.IsNotEmpty(); }
	bool HasOutgoing() const { return m_outPipes.IsNotEmpty(); }
	bool IsOrphan()    const { return !(HasIncoming() || HasOutgoing()); }

	void AddIncoming(BaseKnot const &);
	void AddOutgoing(BaseKnot const &);

	void SetConnections(BaseKnot const &);
	void ClearConnections();

	size_t GetNrOfInConns    () const { return m_inPipes.Size(); }
	size_t GetNrOfOutConns   () const { return m_outPipes.Size(); }
	size_t GetNrOfConnections() const { return m_inPipes.Size() + m_outPipes.Size(); }

	Pipe       & GetIncoming(size_t const i) { return m_inPipes .Get(i); }
	Pipe       & GetOutgoing(size_t const i) { return m_outPipes.Get(i); }

	Pipe const & GetIncoming(size_t const i) const { return m_inPipes .Get(i); }
	Pipe const & GetOutgoing(size_t const i) const { return m_outPipes.Get(i); }

	Pipe& GetFirstIncoming() { return m_inPipes.GetFirst(); }
	Pipe& GetFirstOutgoing() { return m_outPipes.GetFirst(); }

	Pipe const& GetFirstIncoming() const { return m_inPipes.GetFirst(); }
	Pipe const& GetFirstOutgoing() const { return m_outPipes.GetFirst(); }

	void AddIncoming(Pipe & pipe) { m_inPipes .Add(pipe); }
	void AddOutgoing(Pipe & pipe) { m_outPipes.Add(pipe); }

	void RemoveIncoming(Pipe & pipe) { m_inPipes .Remove(pipe); }
	void RemoveOutgoing(Pipe & pipe) { m_outPipes.Remove(pipe); }

	void ReplaceIncoming(Pipe * const pDel, Pipe * const pAdd) { m_inPipes .Replace(pDel, pAdd); }
	void ReplaceOutgoing(Pipe * const pDel, Pipe * const pAdd) { m_outPipes.Replace(pDel, pAdd); }

	void SetIncoming(PipeList const & l) { m_inPipes  = l; }
	void SetOutgoing(PipeList const & l) { m_outPipes = l; }

	void SetIncoming(BaseKnot const & b) { SetIncoming(b.m_inPipes); }
	void SetOutgoing(BaseKnot const & b) { SetOutgoing(b.m_outPipes); }

	void Apply2AllInPipes       (PipeFunc const & f) const { m_inPipes.Apply2All(f); }
	void Apply2AllOutPipes      (PipeFunc const & f) const { m_outPipes.Apply2All(f); }
	void Apply2AllConnectedPipes(PipeFunc const & f) const;

	bool Apply2AllInPipesB       (PipeCrit const & c) const { return m_inPipes .Apply2AllB(c); }
	bool Apply2AllOutPipesB      (PipeCrit const & c) const { return m_outPipes.Apply2AllB(c); }
	bool Apply2AllConnectedPipesB(PipeCrit const & c) const;

	MicroMeterRect GetRect4Text() const;

	void EvaluateSelectionStatus();

protected:

	void FillExternalCircle(DrawContext const&, tHighlight const) const;
	void FillInternalCircle(DrawContext const&, tHighlight const) const;

private:

	PipeList         m_inPipes;
	PipeList         m_outPipes;
	MicroMeterCircle m_circle;
};

export BaseKnot const * Cast2BaseKnot(Nob const *);
export BaseKnot       * Cast2BaseKnot(Nob       *);

export template <typename T>
concept BaseKnot_t = is_base_of<BaseKnot, remove_pointer_t<T>>::value;
