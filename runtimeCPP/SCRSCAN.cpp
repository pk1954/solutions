//                                                                        
//  File:     SCRSCAN.C                                                    
//                                                                         
//
// scrscan.c - scanner for test script 
//

#include "stdafx.h"
#include <limits.h>   
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "errhndl.h"
#include "scanner.h"
#include "Input.h"
                  
#define IS_ONE_OF(S,C)  (((C)!=L'\0')&&(wcschr((S),static_cast<int>(C))!=nullptr))
#define HEX2DIGIT(C)    ((C)-(isdigit(C)?'0':(((C)>='a')?'a':'A')))
                                    

Scanner::Scanner( )
: m_wstrPath( L"" ),
  m_wstrToken( ),
  m_inbuf( ),
  m_wchValue( L'\0' ),
  m_dValue( 0.0 ),
  m_wstrExpected( )
{ }

// OpenInputFile
//
//   Error conditions: File name (including path) too long
//                     File could not be opened                 

void Scanner::OpenInputFile( wstring const & wstrFile ) // (path) name of file to be opened
{
    m_wstrPath = wstrFile;
    m_inbuf.Open( m_wstrPath );
}                                           

void Scanner::CloseInputFile( )
{
    m_inbuf.Close( );
}

Scanner::~Scanner(  )
{
}                                           

// SkipSpace: Skips white space, newlines and comments 
// returns (char)0 or some significant character

wchar_t Scanner::SkipSpace( )
{
   static wchar_t const * const wszDelimiters = L" \t\f\n\r";

   wchar_t wchAct;
   for (;;)
   {
      do 
         wchAct = m_inbuf.ReadNextChar( );
      while ( IS_ONE_OF( wszDelimiters, wchAct ) );
      if ( wchAct == '#' )            // comment symbol              
      { 
         do                          // loop until end of line/file 
         {
            wchAct = m_inbuf.ReadNextChar( );
            if ( wchAct == L'\0' )
               return wchAct;         // end of file reached         
         } while( wchAct != L'\n' );
      }   
      else                           // significant character found 
         return wchAct;               // or end of file reached      
   }
} 
                              
// precondition:  valid start of name has been read
// postcondition: char has been read, which is not part of name

tTOKEN Scanner::ScanName( 
                           wchar_t wchAct   // first char of name 
                        )
{              
   do
   {
      m_wstrToken += wchAct ;  
      wchAct = m_inbuf.ReadNextChar(  );
   } while ( isalnum( wchAct ) || ( wchAct == '_' ) || ( wchAct == ':' ) );

   m_inbuf.SetReadAheadFlag( true );

   return tTOKEN::Name;
}

// precondition:  digit has been read
// postcondition: last character of numeric value has been read

tTOKEN Scanner::ScanUnsigned( )
{
   int const iErrnoSav = errno;  // standard C runtime lib
   tTOKEN    tokRes;

   errno = 0;

   if ( m_inbuf.IsFloat( ) )
   {
      tokRes = tTOKEN::Float;
      m_dValue = m_inbuf.ReadFloat( );
   }
   else
   {
      tokRes = tTOKEN::Number;
      m_ulValue = m_inbuf.ReadNumber( );
   }

   if ( errno )
       ScriptErrorHandler::numericValueError( );

   errno = iErrnoSav;
   return tokRes;
}

// precondition:  opening " has been read
// postcondition: closing " has been read

tTOKEN Scanner::ScanString( )
{                        
   for (;;)
   {
      wchar_t wchAct = m_inbuf.ReadNextChar( );
      if ( (wchAct == L'\n') || (wchAct == L'\0') )
          ScriptErrorHandler::stringConstError( );
      else if ( wchAct == '"' )
         break;                       // closing "" found
      if ( wchAct == L'\\' )          // special handling of backslash  
      {
         wchAct = m_inbuf.ReadNextChar( );   // see what comes after backslash
         if ( wchAct != L'"' )        // " in string must be preceded by backslash 
            m_wstrToken += L'\\';     // other characters leave backslash unchanged 
      }
      m_wstrToken += wchAct;  
   }   

   return tTOKEN::String;
}

// precondition:  opening ' has been read
// postcondition: closing ' has been read

tTOKEN Scanner::ScanCharacter( )
{  
   wchar_t wchAct = m_inbuf.ReadNextChar( );
   m_wstrToken += L'\'';  
   m_wstrToken += wchAct;
   if ( wchAct == L'\\' )
   {
      switch ( wchAct = m_inbuf.ReadNextChar( ) )
      {
         case L'x' : 
         {
            wchar_t wch = m_inbuf.ReadNextChar( );
            if ( isxdigit( wch ) )
            {                                  
               wchar_t wchValue = HEX2DIGIT( wch );   
               wchValue *= 16;
               wch = m_inbuf.ReadNextChar( );
               if ( isxdigit( wch ) )
               {
                  wchValue += HEX2DIGIT( wch );
                  m_wchValue = wchValue;
                  break;
               }
            }       
            m_inbuf.SetStartMarker( );
            ScriptErrorHandler::hexCharError( );
            break;
         }   
         case L'n'  : m_wchValue = L'\n';  break;
         case L't'  : m_wchValue = L'\t';  break;
         case L'v'  : m_wchValue = L'\v';  break;
         case L'b'  : m_wchValue = L'\b';  break;
         case L'r'  : m_wchValue = L'\r';  break;
         case L'f'  : m_wchValue = L'\f';  break;
         case L'a'  : m_wchValue = L'\a';  break;
         case L'\'' : m_wchValue = L'\'';  break;
         case L'"'  : m_wchValue = L'\"';  break;
         case L'\\' : m_wchValue = L'\\';  break;
         default    : m_wchValue = wchAct; break;
      }                            
   }
   else  // normal character constant 
   {                
      m_wchValue = wchAct;
   }

   if ( m_inbuf.ReadNextChar( ) != L'\'')
   {
      m_inbuf.SetStartMarker( );
      ScriptErrorHandler::charConstError( );
   }
    
   return tTOKEN::Character;
}

// ReadOneOf: Try to read one of a group of given characters
   
wchar_t Scanner::ReadOneOf( wstring const & strValid ) 
{ 
	wchar_t chRes = (wchar_t)0;                      

	SetExpectedToken( L"one of \"" + strValid + L"\"" );

	switch ( NextToken( true ) )
	{
		case tTOKEN::End:         // end of file reached 
            ScriptErrorHandler::eofError( );
			break;
      
		case tTOKEN::Special:
			{
				chRes = GetCharacter( );                      
				if ( strValid.find( chRes ) == wstring::npos )
					ScriptErrorHandler::charError( );
				break;
			}
			break;
         
		default: 
			ScriptErrorHandler::tokenError( );
			break;
	}

   return chRes; 
}

//   NextToken: Try to read next token and return type of token
//   if Parameter fStartMarker is true, the marker, which is displayed in case
//   of error is reset, otherwise the marker continues from the previous token.
//   This feature applies only to error display. The various Get... functions 
//   return always the new token.

tTOKEN Scanner::NextToken( bool const fStartMarker )
{
   wchar_t const wchAct = SkipSpace( );         // try to find new token 
   m_inbuf.SetReadAheadFlag( false );
   
   if ( fStartMarker )
      m_inbuf.SetStartMarker( ); // mark start of token in input buffer  

   if ( wchAct == L'\0' )  
   {
      return tTOKEN::End;       // end of file found     
   }

   m_wstrToken.clear( );

   if ( isalpha( wchAct ) || (wchAct == L'_') )
   {           
      return ScanName( wchAct );
   }
   else if ( isdigit( wchAct ) )
   {                                   
      return ScanUnsigned( );
   }
   else if ( wchAct == L'"' )                 
   {
      return ScanString( );
   }   
   else if ( wchAct == L'\'' )
   {
      return ScanCharacter( );
   }
   else // any other character 
   {
      m_wstrToken += wchAct;
      m_wchValue = wchAct;
      return tTOKEN::Special;
   }  
}

