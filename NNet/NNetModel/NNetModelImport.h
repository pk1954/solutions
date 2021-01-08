// NNetModelImport.h 
//
// NNetModel

#pragma once

#include <vector>
#include <fstream>
#include "ObserverInterface.h"
#include "MoreTypes.h"
#include "ShapeId.h"
#include "NNetModelStorage.h"
#include "NNetModelWriterInterface.h"

using std::wostream;
using std::wstring;
using std::vector;

class Param;
class Script;
class Pipe;
class Shape;
class ShapeList;
class Observable;

class NNetModelImport : public ObserverInterface
{
public:
	void Initialize
	( 
		NNetModelWriterInterface * const, 
		Script                   * const,       
		ReadModelResult          * const
	);

	virtual void Notify( bool const bImmediate ) { m_pMWI->SetUnsavedChanges( true ); }

	bool Import( wstring const, ShapeList * const, bool const );

	NNetModelWriterInterface & GetWriterInterface() { return * m_pMWI; }
	MonitorData              & GetMonitorData()     { return m_pMWI->GetMonitorData(); }

private:
	NNetModelWriterInterface * m_pMWI    { nullptr };
	Script                   * m_pScript { nullptr };
	ReadModelResult          * m_pResult { nullptr };

	bool            m_bPreparedForReading { false };
	vector<ShapeId> m_CompactIds;

	void prepareForReading( );
	bool readModel( wstring const, NNetModelWriterInterface & );

	friend static unsigned int __stdcall readModelThreadProc( void * );
};
