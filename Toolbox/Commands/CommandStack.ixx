// CommandStack.ixx
//
// Toolbox\Commands

export module Commands:CommandStack;

import std;
import Script;
import Scanner;
import :BaseCommand;
import Observable;
import ScriptStack;

using std::wstring;
using std::vector;
using std::wcout;
using std::endl;
using std::wostream;
using std::function;
using std::unique_ptr;

export class CommandStack
{
public:

    virtual void Check() {};

    void Initialize(Observable* const);
    bool UndoStackEmpty() const;
    bool RedoStackEmpty() const;
    void Push(unique_ptr<BaseCommand>);
    void PushStackCommand(unique_ptr<BaseCommand>);
    bool UndoStackCommand();
    bool RedoStackCommand();
    void Clear();
    void DoAll();
    void UndoAll();

private:

    vector<unique_ptr<BaseCommand>> m_CommandStack { };
    size_t                          m_iIndex { 0 }; // index into m_Commandstack

    Observable* m_pStaticModelObservable { nullptr };

    BaseCommand* getCmdPtr(size_t const) const;
    BaseCommand& currentCmd() const;
    BaseCommand& previousCmd() const;

    void set2OlderCmd();
    void set2YoungerCmd();

    void notify() const;
    void clearRedoStack();
    bool canBeCombined(BaseCommand const*) const;
};

