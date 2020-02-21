// observerInterface.h : 
//
// Utilities

#pragma once

// Observers need a way to be notified, if the object they observe has changed
// They have to implement the pure virtual function "Notify" for this purpose

// The bool Parameter of Notify: 
//
// In some cases, the observed object distinguishes between changes, 
// which should be processed immediately (Parameter = true) and other changes (false).
// E.g. changes happening with high frequence should be notified with "false"
// so that the observer has a change to process the changes in longer intervalls

class ObserverInterface
{
public:
    virtual ~ObserverInterface( ) {};

    virtual void Notify( bool const ) = 0;
};
