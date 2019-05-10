//                                                                          
//  File:     SCRIPT.C                                                      
//                                                                          
// 
//  script.c - main loop for processing test scripts
// 

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>  
#include <string.h>
#include <assert.h>
#include "scanner.h"
#include "errhndl.h"
#include "script.h"
#include "symtab.h"

bool                   Script::m_bStop     = false;
Script_Functor const * Script::m_pWrapHook = nullptr;

//   ScrSetWrapHook: Set hook function, which will be called in processScript
   
void Script::ScrSetWrapHook( Script_Functor const * const pHook )
{
    m_pWrapHook = pHook;
}
  
//   readSign: Read '+' or '-'

bool Script::readSign( )
{
   wchar_t const wch  = m_pScanAct->GetCharacter( );
   bool          fNeg = false;   

   if ( (wch == L'+') || (wch == L'-') )
   {
      if ( wch == L'-' )
         fNeg = true;
                  
      switch ( m_pScanAct->NextToken( false ) )
      {
         case tTOKEN::End:         // end of file reached 
             ScriptErrorHandler::eofError( );
            break;
                     
         case tTOKEN::Number:     // unsigned integer constant 
            break;
         
		 case tTOKEN::Special:
             ScriptErrorHandler::charError( );
            break;
                     
         default: 
             ScriptErrorHandler::tokenError( );
      }             
   }
   else
       ScriptErrorHandler::charError( );
      
   return fNeg;   
}   

//  numeric: Process numeric value

unsigned long Script::numeric
( 
   wstring       const & wstrExpected, // name of expected token
   unsigned long const   ulMax,        // maximum absolute value
   bool        * const   pfNeg         // negative value allowed?
)
{                                  
   bool    const fNegAllowed = * pfNeg;
   unsigned long ulValue = 0;

   m_pScanAct->SetExpectedToken( wstrExpected );

   *pfNeg = false;

   switch ( m_pScanAct->NextToken( true ) )    
   {
      case tTOKEN::End:         // end of file reached 
         ScriptErrorHandler::eofError( );
         break;
      
      case tTOKEN::Number:    // unsigned integer constant 
         ulValue = m_pScanAct->GetUlong( );      
         break;

      case tTOKEN::Special:
         *pfNeg  = readSign( );
         ulValue = m_pScanAct->GetUlong( );      
         break;
         
      case tTOKEN::Name: // may be symbolic constant 
         {
            Symbol const & symbol = SymbolTable::GetSymbolFromName( m_pScanAct->GetString( ) );
            switch ( symbol.GetSymbolType( ) )
            {
               case tSTYPE::ULongConst:
                  ulValue = symbol.GetUlongConst( );
                  break;

               case tSTYPE::LongConst:
                  {
                     long const lValue = symbol.GetLongConst( );
                     *pfNeg  = lValue < 0L;
                     ulValue = static_cast<unsigned long>( *pfNeg ? -lValue : lValue );
                  }
                  break;

               default:
                   ScriptErrorHandler::typeError( );
            }
         }
         break;
         
      default: 
          ScriptErrorHandler::tokenError( );
   }            
   
   if ( ulValue > ulMax )
       ScriptErrorHandler::numericError( );

   if ( *pfNeg && !fNegAllowed )
       ScriptErrorHandler::negativeError( );

   return ulValue;
}

//  ScrReadFloat: Try to read a float from open test script

double Script::ScrReadFloat( void )
{ 
   bool   fNeg = false;   
   double dRes = 0.0;
         
   m_pScanAct->SetExpectedToken( L"float" );

   switch ( m_pScanAct->NextToken( true ) )
   {
      case tTOKEN::End:            // end of file reached 
          ScriptErrorHandler::eofError( );
         break;
      
      case tTOKEN::Name: // may be symbolic constant 
         {
            Symbol const & symbol = SymbolTable::GetSymbolFromName( m_pScanAct->GetString( ) );
            switch ( symbol.GetSymbolType( ) )
            {
               case tSTYPE::ULongConst:
                  dRes = symbol.GetUlongConst( );
                  break;

               case tSTYPE::LongConst:
                  dRes = symbol.GetLongConst( );
                  break;

               case tSTYPE::FloatConst:
                  dRes = symbol.GetFloatConst( );
                  break;

               default:
                   ScriptErrorHandler::typeError( );
            }
         }
         break;
         
      case tTOKEN::Number:     // unsigned integer constant
         dRes = static_cast<double>(m_pScanAct->GetUlong( ));
         break;
         
      case tTOKEN::Float:       // floating point constant 
         dRes = m_pScanAct->GetFloat( );
         break;

      case tTOKEN::Special:
         {
            wchar_t const wch = m_pScanAct->GetCharacter( );
            if ( (wch == L'+') || (wch == L'-') )
            {
               if ( wch == L'-' )
                  fNeg = true;
               switch ( m_pScanAct->NextToken( false ) )
               {
                  case tTOKEN::End:         // end of file reached 
                     ScriptErrorHandler::eofError( );
                     break;
                     
                  case tTOKEN::Number:     // unsigned integer constant
                     dRes = m_pScanAct->GetUlong( );
                     break;
         
                  case tTOKEN::Float:       // floating point constant 
                     dRes = m_pScanAct->GetFloat( );
                     break;

                  case tTOKEN::Special:
                     ScriptErrorHandler::charError( );
                     break;
                     
                  default: 
                     ScriptErrorHandler::tokenError( );
               }             

               if ( fNeg )
                  dRes = -dRes;
            }
            else
               ScriptErrorHandler::charError( );
         }   
         break;
         
      default: 
          ScriptErrorHandler::tokenError( );
   }

   return dRes; 
}

// ScrReadUlong: Try to read an unsigned long from open test script
   
unsigned long Script::ScrReadUlong( void )
{ 
   bool                fNeg    = false;
   unsigned long const ulValue = numeric( L"unsigned long", ULONG_MAX, &fNeg );
   return ulValue; 
}

//  ScrReadLong: Try to read a long from open test script
   
long Script::ScrReadLong( void )
{ 
   bool fNeg = true;
   long lRes = static_cast<long>(numeric( L"long", LONG_MAX, &fNeg ));

   if ( fNeg )
      lRes = -lRes;
      
   return lRes;  
} 

//  ScrReadInt: Try to read an integer from open test script
   
int Script::ScrReadInt( void )
{ 
   bool fNeg = true;
   int  iRes = static_cast<int>(numeric( L"int", INT_MAX, &fNeg ));
   
   if ( fNeg )
      iRes = -iRes;
      
   return iRes;  
} 

// ScrReadUint: Try to read an unsigned integer from open test script
   
unsigned int Script::ScrReadUint( void )
{ 
   bool               fNeg  = false;
   unsigned int const uiRes = static_cast<unsigned int>(numeric( L"unsigned int", UINT_MAX, &fNeg ));
   return uiRes;  
} 

//  ScrReadShort: Try to read a short from open test script
   
short Script::ScrReadShort( void )
{ 
   bool  fNeg = true;
   short sRes = static_cast<short>(numeric( L"short", SHRT_MAX, &fNeg ));

   if ( fNeg )
      sRes = -sRes;

   return sRes;  
} 

//  ScrReadUshort: Try to read an unsigned short from open test script
   
unsigned short Script::ScrReadUshort( void )
{ 
   bool                 fNeg  = false;
   unsigned short const usRes = static_cast<unsigned short>(numeric( L"unsigned short", USHRT_MAX, &fNeg ));
   return usRes;  
} 

//  ScrReadUchar: Try to read an unsigned character from open test script
   
unsigned char Script::ScrReadUchar( void )
{ 
   bool                fNeg   = false;
   unsigned char const uchRes = static_cast<unsigned char>(numeric( L"unsigned char", UCHAR_MAX, &fNeg ));
   return uchRes;  
}

// ScrReadChar: Try to read a character from open test script
   
wchar_t Script::ScrReadChar( void )
{ 
   bool    fNeg   = false;   
   wchar_t wchRes = L'\0'; 
               
   m_pScanAct->SetExpectedToken( L"char" );
   
   switch ( m_pScanAct->NextToken( true ) )
   {
      case tTOKEN::End:         // end of file reached 
         ScriptErrorHandler::eofError( );
         break;
      
      case tTOKEN::Character:   // character constant found  
         wchRes = m_pScanAct->GetCharacter( );
         break;

      case tTOKEN::Special:
         fNeg = readSign( );
         //lint -fallthrough

      case tTOKEN::Number:    // unsigned integer constant 
      {
         unsigned long const ulValue = m_pScanAct->GetUlong( );
   
         if ( ulValue > CHAR_MAX )
            ScriptErrorHandler::numericError( );

         wchRes = static_cast<wchar_t>(ulValue);
         if ( fNeg )
            wchRes = -wchRes;

         break;
      }
      
      default: 
         ScriptErrorHandler::tokenError( );
   }

   return wchRes; 
}

//   ScrReadString: Try to read a string from open test script
//                  Return code is a pointer to a string in an internal buffer 
//                  This pointer is valid until next call of a Scr.. function
   
wstring const Script::ScrReadString( void )
{ 
   m_pScanAct->SetExpectedToken( L"string" );

   switch ( m_pScanAct->NextToken( true ) )
   {
      case tTOKEN::End:         // end of file reached 
         ScriptErrorHandler::eofError( );
         break;
      
      case tTOKEN::String:      // string constant 
         return m_pScanAct->GetString( );

      case tTOKEN::Special:
         ScriptErrorHandler::charError( );
         break;
         
      case tTOKEN::Name: // may be symbolic constant 
         {
            Symbol const & symbol = SymbolTable::GetSymbolFromName( m_pScanAct->GetString( ) );
            if ( symbol.GetSymbolType( ) != tSTYPE::StringConst )
                ScriptErrorHandler::typeError( );
            return symbol.GetStringConst( );
         }
         //lint -fallthrough
         
      default: 
         ScriptErrorHandler::tokenError( );
         break;
   }

   return nullptr;
}
            
//  ScrProcess: Process a test script

bool Script::ScrProcess
(
    wstring const & wstrPath  // name of test script to be processed
)
{ 
    Scanner scan;          //lint -esym( 1414, scan )   Assigning address of auto variable 'scan' to member
    m_pScanAct = & scan;   // This is save. m_pScanAct is set to nullptr, before leaving the scope of this function
	m_bStop = false;
    try 
    {  
        scan.OpenInputFile( wstrPath ); // open script file 

		for (;;)
        {
	        if( m_bStop )
				break;

            tTOKEN const token = scan.NextToken( true );
         
            m_pScanAct->SetExpectedToken( L"function name" );

            if ( token == tTOKEN::Name )
            {
                wstring const & wstrName = m_pScanAct->GetString( );

				if ( m_pWrapHook != nullptr )
                    (* m_pWrapHook)( * this );                // call hook function 
            
                Symbol  const & symbol = SymbolTable::GetSymbolFromName( wstrName );       // find entry in symbol table 

                if ( symbol.GetSymbolType( ) != tSTYPE::Function )
                   ScriptErrorHandler::typeError( );          // wrong symbol type 

                (symbol.GetFunction( ))( * this );             // call wrapper function 
                m_pScanAct = & scan;
            }   
            else if ( token == tTOKEN::End )
                break;                                        // normal termination 

            else if ( token == tTOKEN::Special )
                ScriptErrorHandler::charError( );

            else
                ScriptErrorHandler::tokenError( );
        }         

        scan.CloseInputFile( );
        m_pScanAct = nullptr;
		if ( m_pWrapHook != nullptr )
			(* m_pWrapHook)( * this );                // call hook function 
    }
    catch ( ScriptErrorHandler::ScriptErrorInfo const & errInfo )
    {
        ScriptErrorHandler::handleScriptError( errInfo, * m_pScanAct );
        return false;
    }

    return true;
}

bool Script::ProcessScript( wstring const & wstrScript )
{
    Script script;
    return script.ScrProcess( wstrScript );
}

void Script::Clear()
{
	ScrSetWrapHook( nullptr );
	SymbolTable::Clear();
}
