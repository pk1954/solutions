/****************************************************************************
 *                                                                          *
 *  File:     OUTPUT.H                                                      *
 *                                                                          *
 *  Author:   P. Kraus         3SOFT GmbH                                   *
 *                                                                          *
 *  Project:  TESTTOOL  COMPARE                                             *
 *                                                                          *
 ****************************************************************************
 *
 * output.h - interface for output driver
 *
 */

#pragma once

#include <string>

enum class tOUTPUT
{
    Nothing,
    Text,
    RTF
};

extern void InitOutput( tOUTPUT );
extern void StartParagraph( int iCount = 1 );
extern void Bold( wstring const &  );
extern void BoldUnderlined( wstring const & );
extern void Italics( wstring const &  );
extern void LightGrey( void );
extern void Standard( void );
extern void Output( wstring const &  );
extern void TerminateOutput( void );
