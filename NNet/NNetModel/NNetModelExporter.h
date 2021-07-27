// NNetModelExporter.h 
//
// NNetModel

#pragma once

#include <vector>
#include <fstream>
#include "MoreTypes.h"
#include "NobId.h"
#include "NobIdList.h"

using std::wostream;
using std::wstring;
using std::vector;

class Param;
class Script;
class Pipe;
class Nob;
class Connector;
class ClosedConnector;
class UPNobList;
class Observable;
class ModelDescription;
class NNetModelReaderInterface;

class NNetModelExporter
{
public:
	void Initialize( NNetModelReaderInterface * const );
	void WriteModel();

private:
	NNetModelReaderInterface * m_pNMRI                    { nullptr };
	Observable               * m_unsavedChangesObservable { nullptr };

	NobIdList m_CompactIds;

	long const getCompactIdVal(NobId const);

	void writeNob             ( wostream &, Nob  const & );
	void writePipe            ( wostream &, Pipe const & );
	void writeDescription     ( wostream & );
	void writeMonitorData     ( wostream & );
	void writeTriggerSounds   ( wostream & );
	void writeNobParameters   ( wostream & );
	void writeGlobalParameters( wostream & );
	void writeNobs            ( wostream & );
	void writeHeader          ( wostream & );
	void write                ( wostream & );
};
