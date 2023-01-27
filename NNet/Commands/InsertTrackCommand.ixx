// InsertTrackCommand.ixx
//
// Commands

module;

#include <memory>
#include <iostream>

export module InsertTrackCommand;

import Types;
import NNetCommand;
import NNetModel;

using std::unique_ptr;
using std::make_unique;

export class InsertTrackCommand: public NNetCommand
{
public:

    InsertTrackCommand(TrackNr const trackNr)
        : m_trackNr(trackNr)
    {}

    void Do() final 
    {
        m_pNMWI->GetMonitorData().InsertTrack(m_trackNr);
        m_pSound->Play(L"SNAP_IN_SOUND");
    };

    void Undo() final
    { 
        m_pNMWI->GetMonitorData().DeleteTrack(m_trackNr);
        m_pSound->Play(L"DISAPPEAR_SOUND");
    };

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, new Wrapper);
	}

	static void Push(TrackNr const trackNr)
	{
		if (IsTraceOn())
			TraceStream() << NAME << trackNr << endl;
		m_pStack->PushCommand(make_unique<InsertTrackCommand>(trackNr));
	}

private:

	inline static const wstring NAME { L"InsertTrack" };

	class Wrapper : public ScriptFunctor
	{
	public:
		void operator() (Script& script) const final
		{
			InsertTrackCommand::Push(ScrReadTrackNr(script));
		}
	};

	TrackNr m_trackNr;
};
