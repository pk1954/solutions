// MoveSensorCmd.ixx
//
// NNetCommands

export module NNetCommands:MoveSensorCmd;

import std;
import Debug;
import IoUtil;
import IoConstants;
import :NNetCommand;

export class MoveSensorCmd : public NNetCommand
{
public:
	MoveSensorCmd
	(
		SensorId      const & id,
		MicroMeterPnt const & delta
	)
	  : m_delta(delta)
	{
		Sensor * pSensor { m_pNMWI->GetSensorList().GetSensor(id) };
		Assert(pSensor);
		Assert(pSensor->IsSensor());
		m_pSensor = static_cast<Sensor *>(pSensor);
	}

	void Do() final 
	{ 
		m_pSensor->MoveSensor(m_pNMWI->GetUPNobsC(), m_delta);
	}

	void Undo() final 
	{ 
		m_pSensor->MoveSensor(m_pNMWI->GetUPNobsC(), -m_delta);
	}

	bool CombineCommands(BaseCommand const & src) final
	{ 
		MoveSensorCmd const & cmdSrc { static_cast<MoveSensorCmd const &>(src) };
		if (m_pSensor != cmdSrc.m_pSensor)
			return false;
		m_delta += cmdSrc.m_delta;
		return true; 
	};

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, &m_wrapper);
	}

	static void Push(SensorId const id, MicroMeterPnt const& delta)
	{
		if (IsTraceOn())
			TraceStream() << NAME << id << SPACE << delta << endl;
		PushCommand(make_unique<MoveSensorCmd>(id, delta));
	}

private:

	inline static const wstring NAME { L"MoveSensor" };

	inline static struct myWrapper : public Wrapper
	{
		using Wrapper::Wrapper;
		void operator() (Script& script) const final
		{
			SensorId      const id      { script.ScrReadInt() };
			MicroMeterPnt const umDelta { ScrReadMicroMeterPnt(script) };
			MoveSensorCmd::Push(id, umDelta);
		}
	} m_wrapper { NAME };

	Sensor * m_pSensor;
	MicroMeterPnt m_delta;
};
