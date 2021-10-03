//                                                                          
//  File:     SCRIPT.C                                                      
//                                                                          
// 
//  script.c - main loop for processing test scripts
// 

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <filesystem>
#include <limits.h>  
#include <string.h>
#include <assert.h>
#include "util.h"
#include "scanner.h"
#include "errhndl.h"
#include "script.h"
#include "symtab.h"

using std::numeric_limits;
using std::filesystem::file_size;

//   ScrSetWrapHook: Set hook function, which will be called in processScript
   
void Script::ScrSetWrapHook(ScriptFunctor const * const pHook)
{
    m_pWrapHook = pHook;
}

void Script::ScrReadSpecial(wchar_t const wchExpected)
{
	static wchar_t strExpected[2] = L"";
	strExpected[0] = wchExpected;
	m_scanner.SetExpectedToken(strExpected);

	switch (m_scanner.NextToken(true))
	{
	case tTOKEN::End:         // end of file reached 
		ScriptErrorHandler::eofError();
		break;

	case tTOKEN::Special:
		if (m_scanner.GetCharacter() != wchExpected)
			ScriptErrorHandler::tokenError();
		break;

	default: 
		ScriptErrorHandler::tokenError();
	}             
}

void Script::ScrReadString(wstring const wstrExpected)
{
	m_scanner.SetExpectedToken(wstrExpected);

	switch (m_scanner.NextToken(true))
	{
	case tTOKEN::End:         // end of file reached 
		ScriptErrorHandler::eofError();
		break;

	case tTOKEN::Name:
		if (m_scanner.GetString() != wstrExpected)
			ScriptErrorHandler::stringError();
		break;

	default: 
		ScriptErrorHandler::stringError();
	}             
}

//   readSign: Read '+' or '-'

bool Script::readSign()
{
   wchar_t const wch  = m_scanner.GetCharacter();
   bool          fNeg = false;   

   if ((wch == L'+') || (wch == L'-'))
   {
      if (wch == L'-')
         fNeg = true;
                  
      switch (m_scanner.NextToken(false))
      {
         case tTOKEN::End:         // end of file reached 
             ScriptErrorHandler::eofError();
             break;
                     
         case tTOKEN::Number:     // unsigned integer constant 
             break;
         
		 case tTOKEN::Special:
             ScriptErrorHandler::charError();
             break;
                     
         default: 
             ScriptErrorHandler::tokenError();
      }             
   }
   else
       ScriptErrorHandler::charError();
      
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

   m_scanner.SetExpectedToken(wstrExpected);

   *pfNeg = false;

   switch (m_scanner.NextToken(true))    
   {
      case tTOKEN::End:         // end of file reached 
         ScriptErrorHandler::eofError();
         break;
      
      case tTOKEN::Number:    // unsigned integer constant 
         ulValue = m_scanner.GetUlong();      
         break;

      case tTOKEN::Special:
         *pfNeg  = readSign();
         ulValue = m_scanner.GetUlong();      
         break;
         
      case tTOKEN::Name: // may be symbolic constant 
         {
            Symbol const & symbol = SymbolTable::GetSymbolFromName(m_scanner.GetString());
            switch (symbol.GetSymbolType())
            {
               case tSTYPE::ULongConst:
                  ulValue = symbol.GetUlongConst();
                  break;

               case tSTYPE::LongConst:
                  {
                     long const lValue = symbol.GetLongConst();
                     *pfNeg  = lValue < 0L;
                     ulValue = static_cast<unsigned long>(*pfNeg ? -lValue : lValue);
                  }
                  break;

               default:
                   ScriptErrorHandler::typeError();
            }
         }
         break;
         
      default: 
          ScriptErrorHandler::tokenError();
   }            
   
   if (ulValue > ulMax)
       ScriptErrorHandler::numericError();

   if (*pfNeg && !fNegAllowed)
       ScriptErrorHandler::negativeError();

   return ulValue;
}

//  ScrReadFloat: Try to read a float from open test script

double Script::ScrReadFloat(void)
{ 
   bool   fNeg = false;   
   double dRes = 0.0;
         
   m_scanner.SetExpectedToken(L"float");

   switch (m_scanner.NextToken(true))
   {
      case tTOKEN::End:            // end of file reached 
          ScriptErrorHandler::eofError();
         break;
      
      case tTOKEN::Name: // may be symbolic constant 
         {
            Symbol const & symbol = SymbolTable::GetSymbolFromName(m_scanner.GetString());
            switch (symbol.GetSymbolType())
            {
               case tSTYPE::ULongConst:
                  dRes = symbol.GetUlongConst();
                  break;

               case tSTYPE::LongConst:
                  dRes = symbol.GetLongConst();
                  break;

               case tSTYPE::FloatConst:
                  dRes = symbol.GetFloatConst();
                  break;

               default:
                   ScriptErrorHandler::typeError();
            }
         }
         break;
         
      case tTOKEN::Number:     // unsigned integer constant
         dRes = static_cast<double>(m_scanner.GetUlong());
         break;
         
      case tTOKEN::Float:       // floating point constant 
         dRes = m_scanner.GetFloat();
         break;

      case tTOKEN::Special:
         {
            wchar_t const wch = m_scanner.GetCharacter();
            if ((wch == L'+') || (wch == L'-'))
            {
               if (wch == L'-')
                  fNeg = true;
               switch (m_scanner.NextToken(false))
               {
                  case tTOKEN::End:         // end of file reached 
                     ScriptErrorHandler::eofError();
                     break;
                     
                  case tTOKEN::Number:     // unsigned integer constant
                     dRes = m_scanner.GetUlong();
                     break;
         
                  case tTOKEN::Float:       // floating point constant 
                     dRes = m_scanner.GetFloat();
                     break;

                  case tTOKEN::Special:
                     ScriptErrorHandler::charError();
                     break;
                     
                  default: 
                     ScriptErrorHandler::tokenError();
               }             

               if (fNeg)
                  dRes = -dRes;
            }
            else
               ScriptErrorHandler::charError();
         }   
         break;
         
      default: 
          ScriptErrorHandler::tokenError();
   }

   return dRes; 
}

// ScrReadUlong: Try to read an unsigned long from open test script
   
unsigned long Script::ScrReadUlong(void)
{ 
   bool                fNeg    = false;
   unsigned long const ulValue = numeric(L"unsigned long", ULONG_MAX, &fNeg);
   return ulValue; 
}

//  ScrReadLong: Try to read a long from open test script
   
long Script::ScrReadLong(void)
{ 
   bool fNeg = true;
   long lRes = static_cast<long>(numeric(L"long", LONG_MAX, &fNeg));

   if (fNeg)
      lRes = -lRes;
      
   return lRes;  
} 

//  ScrReadInt: Try to read an integer from open test script
   
int Script::ScrReadInt(void)
{ 
   bool fNeg = true;
   int  iRes = static_cast<int>(numeric(L"int", INT_MAX, &fNeg));
   
   if (fNeg)
      iRes = -iRes;
      
   return iRes;  
} 

// ScrReadUint: Try to read an unsigned integer from open test script
   
unsigned int Script::ScrReadUint(void)
{ 
   bool               fNeg  = false;
   unsigned int const uiRes = static_cast<unsigned int>(numeric(L"unsigned int", UINT_MAX, &fNeg));
   return uiRes;  
} 

//  ScrReadShort: Try to read a short from open test script
   
short Script::ScrReadShort(void)
{ 
   bool  fNeg = true;
   short sRes = static_cast<short>(numeric(L"short", SHRT_MAX, &fNeg));

   if (fNeg)
      sRes = -sRes;

   return sRes;  
} 

//  ScrReadUshort: Try to read an unsigned short from open test script
   
unsigned short Script::ScrReadUshort(void)
{ 
   bool                 fNeg  = false;
   unsigned short const usRes = static_cast<unsigned short>(numeric(L"unsigned short", USHRT_MAX, &fNeg));
   return usRes;  
} 

//  ScrReadUchar: Try to read an unsigned character from open test script
   
unsigned char Script::ScrReadUchar(void)
{ 
   bool                fNeg   = false;
   unsigned char const uchRes = static_cast<unsigned char>(numeric(L"unsigned char", UCHAR_MAX, &fNeg));
   return uchRes;  
}

// ScrReadChar: Try to read a character from open test script
   
wchar_t Script::ScrReadChar(void)
{ 
   bool    fNeg   = false;   
   wchar_t wchRes = L'\0'; 
               
   m_scanner.SetExpectedToken(L"char");
   
   switch (m_scanner.NextToken(true))
   {
      case tTOKEN::End:         // end of file reached 
         ScriptErrorHandler::eofError();
         break;
      
      case tTOKEN::Character:   // character constant found  
         wchRes = m_scanner.GetCharacter();
         break;

      case tTOKEN::Special:
         fNeg = readSign();
         [[fallthrough]];

      case tTOKEN::Number:    // unsigned integer constant 
      {
         unsigned long const ulValue = m_scanner.GetUlong();
   
         if (ulValue > CHAR_MAX)
            ScriptErrorHandler::numericError();

         wchRes = static_cast<wchar_t>(ulValue);
         if (fNeg)
            wchRes = -wchRes;

         break;
      }
      
      default: 
         ScriptErrorHandler::tokenError();
   }

   return wchRes; 
}

//   ScrReadString: Try to read a string from open test script
//                  Return code is a pointer to a string in an internal buffer 
//                  This pointer is valid until next call of a Scr.. function
   
wstring const Script::ScrReadString(void)
{ 
   m_scanner.SetExpectedToken(L"string");

   switch (m_scanner.NextToken(true))
   {
      case tTOKEN::End:         // end of file reached 
         ScriptErrorHandler::eofError();
         break;
      
      case tTOKEN::String:      // string constant 
         return m_scanner.GetString();

      case tTOKEN::Special:
         ScriptErrorHandler::charError();
         break;
         
      case tTOKEN::Name: // may be symbolic constant 
         {
            Symbol const & symbol = SymbolTable::GetSymbolFromName(m_scanner.GetString());
            if (symbol.GetSymbolType() != tSTYPE::StringConst)
                ScriptErrorHandler::typeError();
            return symbol.GetStringConst();
         }
         [[fallthrough]];
         
      default: 
         ScriptErrorHandler::tokenError();
         break;
   }

   return wstring(L"");
}
            
bool const Script::ScrOpen(wstring const & wstrPath)
{ 
    m_bStop = false;
    try 
    {  
        m_scanner.OpenInputFile(wstrPath); // open script file 
        m_fileSize = file_size(wstrPath);
    }
    catch (ScriptErrorHandler::ScriptException const & errInfo)
    {
        ScriptErrorHandler::HandleScriptError(m_scanner, errInfo);
        return false;
    }
    return true;
}

bool const Script::ReadNextToken()  // returns true, if token was successfully read
{                                   // or end of file reached         
    if (m_bStop)                    // false, if token was bad 
        return false;
    try 
    {  
        m_token = m_scanner.NextToken(true);
        if (m_pWrapHook)
            (* m_pWrapHook)(* this);            // call hook function 
    }
    catch (ScriptErrorHandler::ScriptException const & errInfo)
    {
        ScriptErrorHandler::HandleScriptError(m_scanner, errInfo);
        return false;
    }
    return true;
}

bool const Script::ProcessToken()
{
    try 
    {  
        m_scanner.SetExpectedToken(L"function name");

        if (m_token == tTOKEN::Name)
        {
            wstring const & wstrName = m_scanner.GetString();
            Symbol  const & symbol   = SymbolTable::GetSymbolFromName(wstrName); // find entry in symbol table 

            if (symbol.GetSymbolType() != tSTYPE::Function)
                ScriptErrorHandler::typeError();    // wrong symbol type 

            symbol.GetFunction()(* this);           // call wrapper function 
        }   
        else if (m_token == tTOKEN::End)
            return false;                           // normal termination 
        else if (m_token == tTOKEN::Special)
            ScriptErrorHandler::charError();
        else
            ScriptErrorHandler::tokenError();
    }
    catch (ScriptErrorHandler::ScriptException const & errInfo)
    {
        ScriptErrorHandler::HandleScriptError(m_scanner, errInfo);
        return false;
    }

    return true;
}         

bool const Script::ScrClose()
{ 
    try 
    {  
        m_scanner.CloseInputFile();
        if (m_pWrapHook)
            (* m_pWrapHook)(* this);                // call hook function 
    }
    catch (ScriptErrorHandler::ScriptException const & errInfo)
    {
        ScriptErrorHandler::HandleScriptError(m_scanner, errInfo);
        return false;
    }
    return true;
}

//  ScrProcess: Process a test script

bool const Script::ScrProcess(wstring const & wstrPath)
{ 
    if (!ScrOpen(wstrPath))   // could not open file 
        return false;          
    while (ReadNextToken() && (m_token != tTOKEN::End))
    {
        if (!ProcessToken())   
        {
            ScrClose();
            return false;     // error in file
        }
    }
    ScrClose();
    return true;             // processed file successfully
}

void Script::Clear()
{
	ScrSetWrapHook(nullptr);
	SymbolTable::Clear();
}

long const Script::GetPercentRead()
{
    long long const filePos = GetFilePos();
    if ((filePos < 0) || (numeric_limits<long>::max() / 100 < filePos))
        return 100L;
    else if (m_fileSize == 0)
        return 0L;
    else
        return Cast2Long(filePos * 100 / m_fileSize);
}
