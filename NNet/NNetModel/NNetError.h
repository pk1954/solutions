// NNetError.h 
//
// NNetModel

#pragma once

#include <exception>
#include "script.h"
#include "errhndl.h"
#include "NNetModelWriterInterface.h"
#include "UPShapeList.h"

using std::to_wstring;
using std::wcout;
using std::endl;

struct ShapeException: public exception
{
};

class NNetErrorHandler : public ShapeErrorHandler
{
public:
    NNetErrorHandler( Script * const pScript, UPShapeList const * const pList )
        : m_pScript( pScript ),
          m_pList( pList )
    {}

    static void CheckShapeId
    ( 
        Script            & script, 
        UPShapeList const & list,
        ShapeId     const   id 
    )
    {        
        Scanner & scanner    { script.GetScanner() };
        wstring   strShapeId { to_wstring( id.GetValue() ) };
        if ( IsUndefined(id) )
        {
            scanner.SetExpectedToken( L"ShapeId != NO_SHAPE" );
            throw ScriptErrorHandler::ScriptException( 999, wstring( L"Invalid shape id: " ) + strShapeId );
        }
        else if ( ! list.IsValidShapeId( id ) )
        {
            scanner.SetExpectedToken( L"id < " + to_wstring( list.Size() ) );
            throw ScriptErrorHandler::ScriptException( 999, wstring( L"Invalid shape id: " ) + strShapeId );
        }
        else if ( list.IsShapeDefined( id ) )
        {
            scanner.SetExpectedToken( L"Defined ShapeId" );
            throw ScriptErrorHandler::ScriptException( 999, wstring( L"Shape is not defined: " ) + strShapeId );
        }
    };

    virtual void operator()( ShapeId const id ) 
    {
        CheckShapeId( * m_pScript, * m_pList, id );
        throw ShapeException();
    }

private:
    Script            * const m_pScript;
    UPShapeList const * const m_pList;
};

inline bool ProcessNNetScript
( 
    Script        & script,
    UPShapeList   & shapeList,
    wstring   const wstrPath
) 
{
    bool bSuccess { false };
    if ( ! wstrPath.empty() )
    {
        NNetErrorHandler errHndl { & script, & shapeList };
        shapeList.SetErrorHandler( & errHndl );
        try
        {
            wcout << L"*** Processing script file " << wstrPath << endl;
            bSuccess = script.ScrProcess( wstrPath );
        }
        catch ( ShapeException e ) 
        { 
            return false;
        }
        shapeList.SetErrorHandler( nullptr );
    }
    return bSuccess;
}

