// IoConstants.ixx
//
// ToolBox/Utilities

export module IoConstants;

import std;

using std::wstring;

export inline wstring const PIPE_FROM           { L"<-" };
export inline wstring const PIPE_TO             { L"->" };
export inline wchar_t const SEPARATOR           { L'|' };
export inline wchar_t const NR_SEPARATOR        { L':' };
export inline wchar_t const ID_SEPARATOR        { L',' };
export inline wchar_t const OPEN_BRACKET        { L'(' };
export inline wchar_t const CLOSE_BRACKET       { L')' };
export inline wchar_t const CURLY_OPEN_BRACKET  { L'{' };
export inline wchar_t const CURLY_CLOSE_BRACKET { L'}' };
export inline wchar_t const EQUALS              { L'=' };
export inline wchar_t const SPACE               { L' ' };
export inline wstring const TAB                 { L"   "};
export inline wchar_t const DOUBLE_QUOTE        { L'\"' };
export inline wchar_t const BACKSLASH           { L'\\' };
export inline wchar_t const COMMENT_SYMBOL      { L'#' };
export inline wstring const COMMENT_START       { L"# *** " };

export wstring Quoted       (wstring const wstr) { return DOUBLE_QUOTE       + wstr + DOUBLE_QUOTE; }
export wstring Brackets     (wstring const wstr) { return OPEN_BRACKET       + wstr + CLOSE_BRACKET; }
export wstring CurlyBrackets(wstring const wstr) { return CURLY_OPEN_BRACKET + wstr + CURLY_CLOSE_BRACKET; }
