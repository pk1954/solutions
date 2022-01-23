//                                                                          
// Datei:    WrapInclude.cpp                                                 
//                                                                          
//   Standard wrapper function implementing an "include" mechanism in test 
//   scripts similar to the preprocessor instruction "#include" in C.
//   For usage call DEF_FUNC(Include) in your main program and add lines like
//
//   Include "C:\MyProject\Test\Standard.dat" 
//
//   inside your test script. The path name can be absolute, like in the 
//   example or relative.

#include "stdafx.h"
#include <string>
#include "SCRIPT.H"

//void WrapInclude::operator() (Script & script) const
//{
//    wstring const & wstrPath = script.ScrReadString();
//    script.ScrProcess(wstrPath);
//}
