// BoolType.ixx
//
// Toolbox\Utilities

export module BoolType;

import Observable;

export class BoolType: public Observable
{
public:
    BoolType(bool const bValue)
        : m_bValue(bValue)
    {}

    virtual bool Set(bool const bNew)
    {
        bool const bOld { m_bValue };
        m_bValue = bNew;
        NotifyAll(true);
        return bOld;
    }

    virtual bool Get() const
    {
        return m_bValue;
    }

private:
    bool m_bValue;
};