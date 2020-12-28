// NNetModelStorage.h 
//
// NNetModel

#pragma once

#include <vector>
#include <fstream>
#include "ObserverInterface.h"
#include "MoreTypes.h"
#include "ShapeId.h"
#include "NNetModelWriterInterface.h"

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
	bool Read     ( wstring const, bool const, bool const );
	void ReadAsync( wstring const, bool const );

	bool    const UnsavedChanges( ) const { return m_bUnsavedChanges; }
	wstring const GetModelPath  ( ) const { return m_wstrPathOfOpenModel; }

	void AddDescription( wstring const wstr ) {	m_wstrDescription += wstr + L"\r\n"; }

	NNetModelWriterInterface & GetWriterInterface() { return * m_pMWI; }
	MonitorData              & GetMonitorData()     { return * m_pMWI->GetMonitorData(); }
	template <Shape_t T>
	T GetShapePtr( ShapeId const id ) {	return GetWriterInterface().GetShapePtr<T>( id ); }

	void ResetModelPath( );
	bool AskAndSave    ( );
	bool AskModelFile  ( );
	bool SaveModel     ( );
	bool SaveModelAs   ( );

	inline static unsigned long const SIGSRC_SHAPE_NR { 100 };  // legacy
	inline static unsigned long const SIGSRC_CIRCLE   { 101 };

	ShapeId ScrReadShapeId( Script & script )
	{
		return ShapeId { ::ScrReadShapeId( script ).GetValue() + m_lShapeIdOffset };
	}

	bool const AddMode() { return m_lShapeIdOffset > 0; }

private:
	inline static double const PROTOCOL_VERSION { 1.7 };

	mutable bool m_bUnsavedChanges { false };  // can be changed in const functions

	HWND                       m_hwndApp                  { nullptr };
	NNetModelReaderInterface * m_pMRI                     { nullptr };
	NNetModelWriterInterface * m_pMWI                     { nullptr };
	Param                    * m_pParam                   { nullptr };
	Observable               * m_unsavedChangesObservable { nullptr };
	Script                   * m_pScript                  { nullptr };
	ReadModelResult          * m_pResult                  { nullptr };
	ModelDescription         * m_pDescription             { nullptr };

	bool            m_bPreparedForReading { false };
	wstring         m_wstrPathOfOpenModel { L"" };
	wstring         m_wstrPathOfNewModel  { L"" };
	wstring         m_wstrDescription     { L"" };
	double          m_dVersion            { 0.0 };
	long            m_lShapeIdOffset      { 0L };
	vector<ShapeId> m_CompactIds;

	long const getCompactIdVal( ShapeId const id ) { return m_CompactIds[ id.GetValue() ].GetValue();	}

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
