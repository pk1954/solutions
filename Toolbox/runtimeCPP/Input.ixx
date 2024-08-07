// File: Input.ixx
//                                                                        
// Toolbox\runtimeCPP

export module RunTime:Input;

import std;

using std::wstring;
using std::wifstream;
using std::streampos;
using std::function;

export class InputBuffer
{
public:

    explicit InputBuffer();
    void Open(wstring const&);
    void Close();
    virtual ~InputBuffer();

    void            SetStartMarker();
    void            UnreadLastChar();
    wchar_t         ReadNextChar();
    double          ReadFloat();
    unsigned long   ReadNumber();
    int             GetActStartPos() const;
    int             GetActEndPos()   const;
    bool            IsFloat()        const;
    wstring const & GetActLine()     const { return m_wstrLine; };
    int             GetActLineNr()   const { return m_iLineNr; };
    bool            IsActive()       const { return m_ifstream.is_open(); }
    streampos       GetFilePos()           { return m_ifstream.tellg(); };

    static void SetNewLineTrigger(function<void(void)> const&);

private:
    inline static function<void(void)> m_newLineTrigger{ nullptr };

    wstring   m_wstrLine  { };          // buffer for script line
    int       m_iLineNr   { 0 };        // actual line number  
    wchar_t * m_pwchStart { nullptr };  // pointer to start of current token 
    wchar_t * m_pwchRead  { nullptr };  // pointer to next char in line 
    wifstream m_ifstream  {};
};
