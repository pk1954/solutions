// BaseCommand.ixx
//
// Toolbox\Commands

module;

#include <memory>
#include <functional>
#include <vector>
#include <string>
#include <iostream>

export module BaseCommand;

import SaveCast;
import SoundInterface;

using std::wcout;
using std::wostream;
using std::function;
using std::vector;
using std::unique_ptr;
using std::wstring;

export class BaseCommand
{
public:

    BaseCommand();

    virtual ~BaseCommand() = default;

    virtual void UpdateUI() = 0;
    virtual void CallUI(bool const) = 0; // called by Animation

    virtual bool CombineCommands(BaseCommand const& src) { return false; };
    virtual bool IsAsyncCommand()                        { return false; };

    virtual void NextScriptCommand() const = 0;

    static void Initialize(Sound* const);

    void TargetReached();

    virtual void Do();
    virtual void Undo();

    static void PlaySound(wstring const& sound) { m_pSound->Play(sound); }
    static void PlayWarningSound() { m_pSound->Warning(); }

protected:

    void AddPhase(unique_ptr<BaseCommand>);

    virtual void BlockUI()   const = 0;
    virtual void UnblockUI() const = 0;

    static bool      IsTraceOn()   { return m_bTrace; }
    static wostream& TraceStream() { return wcout; }

private:

    void doPhase  (); // runs in UI thread
    void undoPhase(); // runs in UI thread

    function<void(BaseCommand*)>    m_targetReachedFunc { nullptr };
    vector<unique_ptr<BaseCommand>> m_phases            { };
    unsigned int                    m_uiPhase           { 0 };

    inline static Sound* m_pSound { nullptr };
    inline static bool   m_bTrace { true };
};
