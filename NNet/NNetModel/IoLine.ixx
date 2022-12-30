// IoLine.ixx
//
// NNetModel

module;

#include <vector>

export module NNetModel:IoLine;

import Types;
import :NNetParameters;
import :MicroMeterPosDir;
import :NobType;
import :PosNob;
import :Nob;

export class IoLine : public PosNob
{
public:
	static bool TypeFits(NobType const type) { return type.IsIoLineType(); }

	IoLine(MicroMeterPnt const & upCenter, NobType const type)
		: PosNob(type),
		  m_circle(upCenter, NEURON_RADIUS)
	{}

	bool CompStep() final { return false; }

	MicroMeter    GetExtension() const final { return m_circle.GetRadius(); }
	MicroMeterPnt GetPos()       const final { return m_circle.GetPos(); }
	Radian        GetDir()       const final;

	void SetPos   (MicroMeterPnt const&)               final;
	void SetDir   (Radian const)                       final;
	void MoveNob  (MicroMeterPnt const&)               final;
	void RotateNob(MicroMeterPnt const&, Radian const) final;
	void Link     (Nob const&, Nob2NobFunc const&)     final;

	void SetDirVector(MicroMeterPnt const &p) { SetDir(Vector2Radian(p)); }

	// IoLines are in one of three states:
	//
	// 1: standard - direction is determined by vector of m_pPipe
	//             HasParentNob() is false
	//             m_radDirection is Radian::NULL_VAL 
	// 2: connected - IoLine is in IoConnector. Direction is determined by vector of IoConnector
	//             HasParentNob() is true
	//             ParentNob points to IoConnector 
	//             m_radDirection is Radian::NULL_VAL 
	// 3: locked - direction is explicitely defined in m_radDirection. Only during animations.
	//             HasParentNob() is false
	//             m_radDirection is not Radian::NULL_VAL 

	void StandardDirection  ()             { SetDir(Radian::NULL_VAL()); }
	void Connect2IoConnector(Nob* const p) { m_pIoConnector = p; }
	void LockDirection      ()             { SetDir(GetDir()); }
	bool IsDirLocked() const { return m_radDirection.IsNotNull(); }

	MicroMeterPnt GetDirVector() const;

	Pipe       * GetPipe ()       { return m_pPipe; }
	Pipe const * GetPipeC() const { return m_pPipe; }

	void SetPipe(Pipe * pPipe) { m_pPipe = pPipe; }

	void SelectAllConnected(bool const bFirst, bool const bOn) final
	{
		if ((IsSelected() != bOn) || bFirst)
		{
			Nob::Select(bOn);
			m_pPipe->SelectAllConnected(false, bOn);
			if (HasParentNob())
				GetParentNob()->SelectAllConnected(false, bOn);
		}
	}

	void DirectionDirty() final 
	{
		if (HasParentNob())
			m_pIoConnector->DirectionDirty();
	}

	bool  HasParentNob() const final { return m_pIoConnector != nullptr; }
	Nob * GetParentNob() const final { return m_pIoConnector; }

private:
	Radian getDirection() const;

	MicroMeterCircle m_circle;
	Radian           m_radDirection { Radian::NULL_VAL() };
	Pipe           * m_pPipe        { nullptr };
	Nob            * m_pIoConnector { nullptr };
};
