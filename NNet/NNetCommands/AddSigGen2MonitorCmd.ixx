// AddSigGen2MonitorCmd.ixx
//
// NNetCommands

export module NNetCommands:AddSigGen2MonitorCmd;

import std;
import Signals;
import :NNetCommand;

export class AddSigGen2MonitorCmd : public NNetCommand
{
public:
	explicit AddSigGen2MonitorCmd(TrackNr const trackNr)
		: m_trackNr(trackNr)
	{
		m_pSigGen  = m_pNMWI->GetSigGenSelected();
	    m_upSignal = make_unique<NNetSignal>(m_pSigGen);
	}

	void Do() final
	{
		m_pNMWI->InsertTrack(m_trackNr);
		m_signalNr = m_pNMWI->AddSignal(move(m_upSignal), m_trackNr);
	}

	void Undo() final
	{
		m_upSignal = m_pNMWI->RemoveSignal(SignalId(m_trackNr, m_signalNr));
		m_pNMWI->DeleteTrack(m_trackNr);
	};

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, &m_wrapper);
	}

	static void Push(TrackNr const trackNr)
	{
		if (IsTraceOn())
			TraceStream() << NAME << trackNr.GetValue() << endl;
		PushCommand(make_unique<AddSigGen2MonitorCmd>(trackNr));
	}

private:

	inline static const wstring NAME { L"AddSigGen2Monitor" };

	inline static struct myWrapper : public Wrapper
	{
		using Wrapper::Wrapper;
		void operator() (Script& script) const final
		{
			AddSigGen2MonitorCmd(ScrReadTrackNr(script));
		}
	} m_wrapper { NAME };

	SignalGenerator *  m_pSigGen;
	TrackNr      const m_trackNr;
	SignalNr           m_signalNr;
	unique_ptr<Signal> m_upSignal;
};