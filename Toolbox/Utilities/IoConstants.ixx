// IoConstants.ixx
//
// ToolBox/Utilities

module;

#include <string>

export module IoConstants;

using std::wstring;

export inline wstring const PIPE_FROM          { L"<-" };
export inline wstring const PIPE_TO            { L"->" };
export inline wchar_t const SEPARATOR          { L'|' };
export inline wchar_t const NR_SEPARATOR       { L':' };
export inline wchar_t const ID_SEPARATOR       { L',' };
export inline wchar_t const OPEN_BRACKET       { L'(' };
export inline wchar_t const CLOSE_BRACKET      { L')' };
export inline wchar_t const LIST_OPEN_BRACKET  { L'{' };
export inline wchar_t const LIST_CLOSE_BRACKET { L'}' };
export inline wchar_t const EQUALS             { L'=' };
export inline wchar_t const SPACE              { L' ' };
export inline wstring const TAB                { L"   "};
export inline wchar_t const DOUBLE_QUOTE       { L'\"' };
export inline wchar_t const BACKSLASH          { L'\\' };
export inline wchar_t const COMMENT_SYMBOL     { L'#' };
export inline wstring const COMMENT_START      { L"# *** " };

