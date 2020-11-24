// NNetModelStorage.h 
//
// NNetModel

#pragma once

#include <vector>
#include <fstream>
#include "ObserverInterface.h"
#include "MoreTypes.h"
#include "ShapeId.h"

using std::wostream;
using std::wstring;
using std::vector;

class Param;
class Script;
class Pipe;
class Shape;
class Observable;
class ModelDescription;
class NNetModelReaderInterface;
class NNetModelWriterInterface;

class ReadModelResult
{
public:
	virtual ~ReadModelResult() {};

	enum class tResult
	{
		ok,
		fileNotFound,
		errorInFile
	};

	virtual void Reaction( tResult const, wstring const = wstring() ) = 0;
};

class NNetModelStorage : public ObserverInterface
{
public:
	void Initialize
	( 
		NNetModelReaderInterface * const, 
		NNetModelWriterInterface * const, 
		Param                    * const, 
		Observable               * const,
		Script                   * const,       
		ReadModelResult          * const,
		ModelDescription         * const
	);

	virtual void Notify( bool const bImmediate ) { setUnsavedChanges( true ); }

	void Write( wostream & );
	bool Read( bool const, wstring const = L"" );
	void ReadAsync( wstring const = L"" );

	bool    const UnsavedChanges( ) const { return m_bUnsavedChanges; }
	wstring const GetModelPath  ( ) const { return m_wstrPathOfOpenModel; }

	void ResetModelPath( );
	bool AskAndSave  ( );
	bool AskModelFile( );
	bool SaveModel   ( );
	bool SaveModelAs ( );

private:
	inline static float const PROTOCOL_VERSION { 1.6f };

	mutable bool m_bUnsavedChanges { false };  // can be changed in const functions

	HWND                       m_hwndApp                  { nullptr };
	NNetModelReaderInterface * m_pModelReaderInterface    { nullptr };
	NNetModelWriterInterface * m_pModelWriterInterface    { nullptr };
	Param                    * m_pParam                   { nullptr };
	Observable               * m_unsavedChangesObservable { nullptr };
	Script                   * m_pScript                  { nullptr };
	ReadModelResult          * m_pResult                  { nullptr };
	ModelDescription         * m_pDescription             { nullptr };

	bool            m_bPreparedForReading { false };
	wstring         m_wstrPathOfOpenModel { L"" };
	wstring         m_wstrPathOfNewModel  { L"" };
	wstring         m_wstrDescription     { L"" };
	vector<ShapeId> m_CompactIds;

	long getCompactIdVal( ShapeId const id ) { return m_CompactIds[ id.GetValue() ].GetValue();	}

	void prepareForReading( );
	bool readModel( );
	void writeModel( );
	void writeShape( wostream &, Shape & );
	void writePipe( wostream &, Pipe const & );
	void setUnsavedChanges( bool const );
	void writeDescription( wostream & );
	void writeMonitorData( wostream & );
	void writeTriggerSounds( wostream & );
	void writeShapeParameters( wostream & );
	void writeGlobalParameters( wostream & );
	void writeShapes( wostream & );
	void writeHeader( wostream & );

	friend static unsigned int __stdcall readModelThreadProc( void * );
};
