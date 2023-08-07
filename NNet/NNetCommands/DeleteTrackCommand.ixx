// DeleteTrackCommand.ixx
//
// NNetCommands

module;

#include <cassert>
#include <iostream>

export module NNetCommands:DeleteTrackCommand;

import IoUtil;
import :NNetCommand;

export class DeleteTrackCommand: public NNetCommand
{
public:

    explicit DeleteTrackCommand(TrackNr const trackNr)
        : m_trackNr(trackNr)
    {}

    void Do() final 
    {
        MonitorData & monitorData { m_pNMWI->GetMonitorData() };
        assert(monitorData.IsEmptyTrack(m_trackNr));
        monitorData.DeleteTrack(m_trackNr);
        PlaySound(L"DISAPPEAR_SOUND");
    };

    void Undo() final
    { 
        m_pNMWI->GetMonitorData().InsertTrack(m_trackNr);
        PlaySound(L"SNAP_IN_SOUND");
    };

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, &m_wrapper);
	}

	static void Push(TrackNr const trackNr)
	{
		if (IsTraceOn())
			TraceStream() << NAME << trackNr << endl;
		PushCommand(make_unique<DeleteTrackCommand>(trackNr));
	}

private:

	inline static const wstring NAME { L"DeleteTrack" };

	inline static struct myWrapper : public Wrapper
	{
		using Wrapper::Wrapper;
		void operator() (Script& script) const final
		{
			DeleteTrackCommand::Push(ScrReadTrackNr(script));
		}
    } m_wrapper { NAME };

    TrackNr m_trackNr;
};
