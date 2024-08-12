// IoLine.ixx
//
// NNetModel

export module NNetModel:IoLine;

import std;
import Types;
import :NNetParameters;
import :NobType;
import :PosNob;
import :Pipe;
import :Nob;

using std::is_base_of;
using std::remove_pointer_t;

export class IoLine : public PosNob
{
public:
	static bool TypeFits(NobType const type) { return type.IsIoLineType(); }

	IoLine(MicroMeterPnt const & upCenter)
	  : m_pos(upCenter)
	{}

	void Dump () const override;
	void Check() const override;

	bool CompStep() final { return false; }

	MicroMeter    GetExtension() const final { return NEURON_RADIUS; }
	MicroMeterPnt GetPos()       const final { return m_pos; }
	Radian        GetDir()       const final;

	void SetPosNoFix(MicroMeterPnt const& pos) final { m_pos = pos; }

	void Recalc     ()                                   final;
	void MoveNob    (MicroMeterPnt const&)               final;
	void RotateNob  (MicroMeterPnt const&, Radian const) final;
	void Link       (Nob const&, Nob2NobFunc const&)     final;

	// IoLines are in one of two states:
	//
	// 1: standard - direction is determined by vector of m_pPipe
	//             HasParentNob() is false
	// 2: connected - IoLine is in IoConnector. Direction is determined by vector of IoConnector
	//             HasParentNob() is true
	//             ParentNob points to IoConnector 

	void Connect2IoConnector(Nob* const p)
	{
		m_pIoConnector = p;
	}

	void DisconnectFromIoConnector()
	{
		m_pIoConnector = nullptr;
	}

	MicroMeterPnt GetDirVector() const;

	Pipe       * GetPipe ()       { return m_pPipe; }
	Pipe const * GetPipeC() const { return m_pPipe; }

	void SetPipe(Pipe * pPipe) { m_pPipe = pPipe; }

	void SelectAllConnected(bool const) final;
	void DirectionDirty    ()           final;

	bool  HasParentNob() const final { return m_pIoConnector != nullptr; }
	Nob * GetParentNob() const final { return m_pIoConnector; }

private:
	Pipe        * m_pPipe        { nullptr };
	Nob         * m_pIoConnector { nullptr };
	MicroMeterPnt m_pos;
};

export template <typename T>
concept IoLine_t = is_base_of<IoLine, remove_pointer_t<T>>::value;
