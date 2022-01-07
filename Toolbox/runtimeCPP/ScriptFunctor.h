// ScriptFunctor.h
//
// runtimeCPP

#pragma once

class Script;

class ScriptFunctor
{
public:
    virtual ~ScriptFunctor() = default;
    virtual void operator() (Script & script) const = 0;
};
