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

using std::unique_ptr;
using std::wostream;
using std::wstring;
using std::vector;

class Param;
class Script;
class Pipe;
class Shape;
class NNetModel;
class ShapeList;
class Observable;

class NNetModelImport : public ObserverInterface
{
public:
	void Initialize
	( 
		Script          * const,       
		ReadModelResult * const
	);

	virtual void Notify( bool const bImmediate ) { m_pMWI->SetUnsavedChanges( true ); }

	unique_ptr<NNetModel> Import( wstring const, bool const );

	NNetModelWriterInterface & GetWriterInterface() { return * m_pMWI; }
	MonitorData              & GetMonitorData()     { return m_pMWI->GetMonitorData(); }
	ShapeList                & GetShapes()          { return m_pMWI->GetShapes(); }

private:
	NNetModelWriterInterface * m_pMWI    { nullptr };
	Script                   * m_pScript { nullptr };
	ReadModelResult          * m_pResult { nullptr };

	vector<ShapeId> m_CompactIds;

	void prepareForReading( );
	bool readModel( wstring const, NNetModelWriterInterface & );

	friend static unsigned int __stdcall readModelThreadProc( void * );
};
