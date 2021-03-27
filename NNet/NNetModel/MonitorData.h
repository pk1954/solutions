// MonitorData.h : 
//
// NNetModel

#pragma once

#include <vector>
#include "NamedType.h"
#include "Signal.h"
#include "Track.h"
#include "SignalId.h"

using std::vector;

using TrackNrFunc = function<void(TrackNr const)>;

class MonitorData
{
public:
	MonitorData()                                = default;  // constructor   
	~MonitorData()                               = default;  // destructor
	MonitorData( MonitorData&& rhs )             = delete;   // move constructor
	MonitorData& operator=( const MonitorData& ) = delete;   // copy assignment operator
	
	MonitorData(const MonitorData& );                        // copy constructor
	MonitorData& operator=(MonitorData&&) noexcept;          // move assignment operator

	bool operator== (MonitorData const & ) const;

	void CheckTracks() const;  // for debugging

	void Reset();

	int  GetNrOfTracks() const;
	bool NoTracks     () const;

	bool const IsValid( TrackNr const trackNr ) const;
	bool const IsValid( SignalId const & ) const;

	void InsertTrack( TrackNr const );
	void DeleteTrack( TrackNr const );

	SignalId const SetSelectedSignalId( SignalId const );
	SignalId const ResetSelectedSignal();

	SignalNr       const AddSignal   ( TrackNr  const, MicroMeterCircle const & );
	SignalNr       const AddSignal   ( TrackNr  const, unique_ptr<Signal> );
	SignalId       const MoveSelectedSignal( TrackNr const );
	Signal const * const GetSignalPtr( SignalId const & ) const;
	Signal       * const GetSignalPtr( SignalId const & );
	Signal const * const GetSelectedSignalPtr() const;
	Signal       * const GetSelectedSignalPtr();

	unique_ptr<Signal> DeleteSignal( SignalId const & );

	void Apply2AllTracks        ( TrackNrFunc const & ) const;
	void Apply2AllSignalsInTrack( TrackNr const, SignalNrFunc const & ) const;
	void Apply2AllSignals       ( SignalIdFunc const & ) const;
	void Apply2AllSignals       ( SignalFunc   const & ) const;

	Signal * const FindSignal( SignalCrit      const & ) const;
	Signal * const FindSensor( MicroMeterPoint const & ) const;

	SignalId const GetSelectedSignalId()          const { return m_idSigSelected; }
	TrackNr  const GetSelectedTrackNr ()          const { return m_idSigSelected.GetTrackNr(); }
	bool     const IsAnySignalSelected()          const { return m_idSigSelected != SignalIdNull; }
	bool     const IsSelected(SignalId const &id) const { return m_idSigSelected == id; }

	bool const IsEmptyTrack(TrackNr const) const;

private:
	Track       * const getTrack( TrackNr const );
	Track const * const getTrack( TrackNr const ) const;
	unique_ptr<Signal> removeSignal( SignalId const & );

	SignalId                  m_idSigSelected {};
	vector<unique_ptr<Track>> m_tracks        {};
};
