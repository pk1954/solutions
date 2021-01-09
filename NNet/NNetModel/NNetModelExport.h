// NNetModelExport.h 
//
// NNetModel

#pragma once

#include <vector>
#include <fstream>
#include "MoreTypes.h"
#include "ShapeId.h"

using std::wostream;
using std::wstring;
using std::vector;

class Param;
class Script;
class Pipe;
class Shape;
class ShapeList;
class Observable;
class ModelDescription;
class NNetModelReaderInterface;

class NNetModelExport
{
public:
	void Initialize( NNetModelReaderInterface * const );
	void WriteModel( );

private:
	NNetModelReaderInterface * m_pMRI                     { nullptr };
	ModelDescription         * m_pDescription             { nullptr };
	Observable               * m_unsavedChangesObservable { nullptr };

	vector<ShapeId> m_CompactIds;

	long const getCompactIdVal(ShapeId const id) { return m_CompactIds[id.GetValue()].GetValue(); }

	void writeShape( wostream &, Shape const & );
	void writePipe ( wostream &, Pipe  const & );
	void writeDescription( wostream & );
	void writeMonitorData( wostream & );
	void writeTriggerSounds( wostream & );
	void writeShapeParameters( wostream & );
	void writeGlobalParameters( wostream & );
	void writeShapes( wostream & );
	void writeHeader( wostream & );
	void write( wostream & );
};
