// ClosedConnector.h
//
// NNetModel

#pragma once

#include "BoolOp.h"
#include "MoreTypes.h"
#include "MicroMeterPosDir.h"
#include "NobType.h"
#include "UPNobList.h"
#include "NobPtrList.h"
#include "Connector.h"

class ClosedConnector: public Nob
{
public:

	static bool    const TypeFits(NobType const type) { return type.IsClosedConnectorType(); }
	static NobType const GetNobType()                 { return NobType::Value::closedConnector; }

	ClosedConnector(MicroMeterPnt const &, Connector &, Connector &);
	virtual ~ClosedConnector() {}

	virtual void Check() const;
	virtual void Dump () const;

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

	virtual NobIoMode const GetIoMode() const { return NobIoMode::internal; }

	virtual bool const IsCompositeNob() { return true; }

	void SetParentPointers();
	void ClearParentPointers();

	size_t const Size() const { return m_pInputConnector->Size(); };

	virtual Radian const GetDir() const { return m_pInputConnector->GetDir(); };

	inline static wchar_t const SEPARATOR     { L':' };
	inline static wchar_t const OPEN_BRACKET  { L'{' };
	inline static wchar_t const CLOSE_BRACKET { L'}' };

	Connector const & GetInputConnector () const { return * m_pInputConnector;  }
	Connector const & GetOutputConnector() const { return * m_pOutputConnector; }

private:
	Connector * m_pInputConnector  { nullptr };
	Connector * m_pOutputConnector { nullptr };
};

ClosedConnector const * Cast2ClosedConnector(Nob const *);
ClosedConnector       * Cast2ClosedConnector(Nob       *);
