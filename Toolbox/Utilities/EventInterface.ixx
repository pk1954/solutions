// EventInterface.ixx : 
//
// Toolbox\Utilities

export module EventInterface;

export class EventInterface
{
public:
    virtual ~EventInterface() = default;

    virtual void Wait() = 0;
    virtual void Continue() = 0;
};
