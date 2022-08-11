//                                                                        
//  File:     Input.cpp                                                   
//                                                                        
//  low level input from script 
//

#include <string>
#include <cassert>
#include <ctype.h>
#include "Input.h"
#include "Script.h"

import ErrHndl;

InputBuffer::InputBuffer() 
{
    m_wstrLine.clear();
    m_wstrLine  += L'\0';
    m_pwchRead   = &m_wstrLine.front();
}

InputBuffer::~InputBuffer() 
{
    m_pwchStart = nullptr;
    m_pwchRead  = nullptr;
};

void InputBuffer::SetNewLineTrigger(function<void(void)> const & pFunc)
{
    m_newLineTrigger = pFunc;
}

// InputBuffer::Open - Open script file and initialize according variables
//
//   Error conditions: File name (including path) too long
//                     File could not be opened

void InputBuffer::Open(wstring const & wstrFile)  // path of file to be opened 
{
    m_ifstream.open(wstrFile, std::ios::in);
    if (! m_ifstream)
        ScriptErrorHandler::inputFileError();
}                                                  // end OpenInputBuffer 

// ReadNextChar: Read next character from script file
//
//   Return code: Char from script file or (char)0, if end of file reached
//                 
//   Writes to:   szLine, iLineNr, pchRead           
//                 
//   Error conditions: File not open
//                     File error
//                     Line too long

wchar_t InputBuffer::ReadNextChar()
{             
   assert(!m_ifstream.bad());
   
   if ((m_pwchRead == nullptr) || (L'\0' == *m_pwchRead))   // end of line reached
   {      
      getline(m_ifstream, m_wstrLine);
      m_wstrLine += L'\n';

      if (m_ifstream.bad())
          ScriptErrorHandler::inputFileError();

      else if (m_ifstream.eof())  // end of file reached
         return L'\0';

      else // new line successfully read
      {         
         m_iLineNr++;
         m_pwchRead = &m_wstrLine.front();
         if (m_newLineTrigger)
             m_newLineTrigger();
      }   
   }

   return *(m_pwchRead++);  // normal exit, deliver next char from line buffer
}                                        

// IsFloat - Look ahead in input buffer, to check if actual token is a float
//           number or not. The function does not check exact syntax, it just
//           gives a hint. If IsFloat returns false, the actual token is
//           definitly not a correct float. If it returns true it might be a
//           valid integer or a syntax error.
//
//   Return code: true  if actual token in input buffer looks like a float
//                false otherwise 
//                    
//   Writes to:   ---
//                 
//   Error conditions: ---

bool InputBuffer::IsFloat() const
{             
   wchar_t const * pwchRun = m_pwchRead;

   assert(isdigit(*(pwchRun-1)));

   //lint -e661  access of out-of-bounds pointer 
   //            no problem, because there is at least a "/n" after the digit

   while (isdigit(*pwchRun))   // search first non digit 
      pwchRun++;  

   return (*pwchRun == L'e') || (*pwchRun == L'E') || (*pwchRun == L'.');

   //lint -e661        
}                                            // end IsFloat 

// ReadFloat - Read float value from input buffer and set read pointer
//               to first char, which is not part of number
//
// Writes to: pchRead           


double InputBuffer::ReadFloat()
{
   wchar_t const * const pwchRun = m_pwchRead - 1;
   wchar_t       *       pwchStop;
   double    const dValue = wcstod(pwchRun, &pwchStop);
   m_pwchRead = pwchStop;
   return dValue;
}                                 

// ReadNumber - Read unsigned integer value from input buffer and set read 
//                pointer to first char, which is not part of number
//
//   Writes to: pchRead           
  
unsigned long InputBuffer::ReadNumber()
{
   wchar_t const * const pwchRun = m_pwchRead - 1;
   wchar_t       *       pwchStop;
   unsigned long   const ulValue = wcstoul(pwchRun, &pwchStop, 0);
   m_pwchRead = pwchStop;
   return ulValue;
}                                         

// SetStartMarker: Mark start of token in input line
//                 Used for display in case of error in script file

void InputBuffer::SetStartMarker()
{
   m_pwchStart = m_pwchRead-1;
}                                          

// UnreadLastChar:

void InputBuffer::UnreadLastChar()
{
    assert(m_pwchRead > &m_wstrLine.front());
    --m_pwchRead;
}

int InputBuffer::GetActStartPos() const
{
    if (m_pwchStart == nullptr)
        return -1;

    assert(m_wstrLine[0] != L'\0');
    assert(m_pwchRead > &m_wstrLine.front());

    return static_cast<int>(m_pwchStart - &m_wstrLine.front());
}          

int InputBuffer::GetActEndPos() const
{
    if (m_pwchStart == nullptr)
        return -1;

    assert(m_wstrLine[0] != L'\0');
    assert(m_pwchRead > &m_wstrLine[0]);

    return static_cast<int>(m_pwchRead - &m_wstrLine.front());
}                  

void InputBuffer::Close()
{
    try
    {
        m_ifstream.close();
    }
    catch (...)
    {
        exit(1);
    };

    m_pwchRead  = nullptr;
    m_pwchStart = nullptr;
}                                          
