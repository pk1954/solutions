// WrapColorLUT.ixx
//
// ModelIO

module;

export module WrapColorLUT;

import Wrapper;

export class WrapColorLUT : public Wrapper
{
public:
    WrapColorLUT()
     : Wrapper(NAME)
    {}

    void operator() (Script& script) const final
    {
    }

    void Write(wostream& out) const final
    {
    }

private:
    inline static const wstring NAME { L"ColorLUT" };

};