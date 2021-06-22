// Connector.h
//
// NNetModel

#pragma once

#include "BoolOp.h"
#include "MoreTypes.h"
#include "MicroMeterPosDir.h"
#include "NobType.h"
#include "UPNobList.h"
#include "NobPtrList.h"
#include "IoNeuron.h"

class Connector: public Nob
{
public:

	static bool    const TypeFits(NobType const type) { return type.IsConnectorType(); }
	static NobType const GetNobType()                 { return NobType::Value::connector; }

	Connector();
	Connector(NobPtrList<IoNeuron> const &);

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

	virtual bool const IsCompositeNob() { return true; }

	void Push(IoNeuron * const p) { m_list.Add(p); }
	IoNeuron * const Pop();

	bool const IsInputConnector () const { return m_list.GetFirst().IsInputNeuron (); }
	bool const IsOutputConnector() const { return m_list.GetFirst().IsOutputNeuron(); }

	size_t const Size() const { return m_list.Size(); }

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

	inline static wchar_t const SEPARATOR     { L':' };
	inline static wchar_t const OPEN_BRACKET  { L'{' };
	inline static wchar_t const CLOSE_BRACKET { L'}' };

	NobPtrList<IoNeuron> const & GetIoNeurons() const { return m_list; }

private:
	NobPtrList<IoNeuron> m_list {};
};

Connector const * Cast2Connector( Nob const * );
Connector       * Cast2Connector( Nob       * );
