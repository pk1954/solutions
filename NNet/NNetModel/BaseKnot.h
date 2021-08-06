// BaseKnot.h
//
// NNetModel

#pragma once

#include "PixelTypes.h"
#include "CircleType.h"
#include "MoreTypes.h"
#include "PipeList.h"
#include "Pipe.h"
#include "Nob.h"

class DrawContext;
class NNetModel;

struct IDWriteTextFormat;

class BaseKnot : public Nob
{
public:

	BaseKnot(MicroMeterPnt const &, NobType const, MicroMeter const);
	virtual ~BaseKnot() {}

	virtual bool operator==(Nob const &) const override;

	virtual BaseKnot & operator*=(float const);
	virtual BaseKnot & operator+=(BaseKnot const &);
	virtual BaseKnot & operator-=(BaseKnot const &);

	virtual MicroMeterPnt const GetPos() const { return m_circle.GetPos(); }

	virtual void       Dump         () const;
	virtual void       Check        () const;
	virtual void       Prepare      ();
	virtual void       Reconnect    ();
	virtual mV   const GetNextOutput() const = 0;
	virtual void       SetPos       (MicroMeterPnt const &);
	virtual bool const IsIncludedIn (MicroMeterRect  const &) const; 
	virtual void       Expand       (MicroMeterRect        &) const;
	virtual void       RotateNob    (MicroMeterPnt const &, Radian const);
	virtual void       Link         (Nob const &, Nob2NobFunc const &);
	virtual void       MoveNob      (MicroMeterPnt const &);

	static bool const TypeFits(NobType const type) { return type.IsBaseKnotType(); }

	MicroMeterCircle const GetCircle   () const { return m_circle; }
	MicroMeter       const GetExtension() const { return m_circle.GetRadius(); }
	mV               const GetVoltage  () const { return m_mVinputBuffer; }

	bool const Includes(MicroMeterPnt const &) const;
	bool const IsPrecursorOf(Pipe const &) const;
	bool const IsSuccessorOf(Pipe const &) const ;

	bool const HasIncoming() const { return m_inPipes.IsNotEmpty (); }
	bool const HasOutgoing() const { return m_outPipes.IsNotEmpty(); }
	bool const IsOrphan()    const { return !(HasIncoming() || HasOutgoing()); }

	void AddIncoming(BaseKnot const &);
	void AddOutgoing(BaseKnot const &); 

	void SetConnections(BaseKnot const &); 
	void ClearConnections();

	size_t GetNrOfIncomingConnections() const { return m_inPipes .Size(); }
	size_t GetNrOfOutgoingConnections() const { return m_outPipes.Size(); }
	size_t GetNrOfConnections()         const { return m_inPipes .Size() + m_outPipes.Size(); }

	Pipe & GetFirstIncoming() { return m_inPipes .GetFirst(); }
	Pipe & GetFirstOutgoing() { return m_outPipes.GetFirst(); }

	Pipe const & GetFirstIncoming() const { return m_inPipes .GetFirst(); }
	Pipe const & GetFirstOutgoing() const { return m_outPipes.GetFirst(); }

	void AddIncoming(Pipe * const p) { m_inPipes .Add(p); }
	void AddOutgoing(Pipe * const p) { m_outPipes.Add(p); }

	void RemoveIncoming(Pipe * const p) { m_inPipes .Remove(p); } 
	void RemoveOutgoing(Pipe * const p) { m_outPipes.Remove(p); }

	void ReplaceIncoming(Pipe * const pDel, Pipe * const pAdd) { m_inPipes .Replace(pDel, pAdd); }
	void ReplaceOutgoing(Pipe * const pDel, Pipe * const pAdd) { m_outPipes.Replace(pDel, pAdd); }

	void SetIncoming(PipeList const & l) { m_inPipes  = l; }
	void SetOutgoing(PipeList const & l) { m_outPipes = l; }

	void SetIncoming(BaseKnot const & b) { SetIncoming(b.m_inPipes); }
	void SetOutgoing(BaseKnot const & b) { SetOutgoing(b.m_outPipes); }

	void Apply2AllInPipes        (PipeFunc const &f) const { m_inPipes .Apply2All(f); }
	void Apply2AllOutPipes       (PipeFunc const &f) const { m_outPipes.Apply2All(f); }
	void Apply2AllConnectedPipes (PipeFunc const &f) const; 

	bool Apply2AllInPipesB       (PipeCrit const &c) const { return m_inPipes .Apply2AllB(c); }
	bool Apply2AllOutPipesB      (PipeCrit const &c) const { return m_outPipes.Apply2AllB(c); }
	bool Apply2AllConnectedPipesB(PipeCrit const &c) const;

	MicroMeterRect const GetRect4Text() const;

	PipeList const & GetIncoming() const { return m_inPipes; }
	PipeList const & GetOutgoing() const { return m_outPipes; }

	inline static wchar_t const OPEN_BRACKET  { L'{' };
	inline static wchar_t const NR_SEPARATOR  { L':' };
	inline static wchar_t const ID_SEPARATOR  { L',' };
	inline static wchar_t const CLOSE_BRACKET { L'}' };

private:

	void drawCircle(DrawContext const &, D2D1::ColorF const, MicroMeterCircle const) const;
	void drawCircle(DrawContext const &, D2D1::ColorF const, MicroMeter       const) const;

	PipeList         m_inPipes;
	PipeList         m_outPipes;
	MicroMeterCircle m_circle;
};

BaseKnot const * Cast2BaseKnot(Nob const *);
BaseKnot       * Cast2BaseKnot(Nob       *);

template <typename T> 
concept BaseKnot_t = is_base_of<BaseKnot, remove_pointer_t<T>>::value;

template <BaseKnot_t T>
MicroMeterPnt const CalcOrthoVector(vector<T *> const & list, MicroMeterLine const & line)
{
	unsigned int uiLeftConnections  { 0 };
	unsigned int uiRightConnections { 0 };
	for (auto pBaseKnot : list)
	{ 
		pBaseKnot->Apply2AllInPipes
		(
			[&](Pipe & pipe) 
			{ 
				MicroMeterPnt pnt { pipe.GetStartPoint() };
				if (PointToLine(line, pnt) < 0.0_MicroMeter)
					++uiLeftConnections;
				else
					++uiRightConnections;
			}
		);
		pBaseKnot->Apply2AllOutPipes
		(
			[&](Pipe & pipe) 
			{ 
				MicroMeterPnt pnt { pipe.GetEndPoint() };
				if (PointToLine(line, pnt) < 0.0_MicroMeter)
					++uiRightConnections;
				else
					++uiLeftConnections;
			}
		);
	}	

	MicroMeterPnt orthoVector = line.OrthoVector();
	if (uiRightConnections < uiLeftConnections)
		orthoVector = -orthoVector;
	return orthoVector;
}