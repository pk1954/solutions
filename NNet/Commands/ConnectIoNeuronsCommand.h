// ConnectIoNeuronsCommand.h
//
// NNetCommands

#pragma once

class Neuron;
class IoNeuron;
class NNetModelWriterInterface;

using std::unique_ptr;

class ConnectIoNeuronsCommand
{
public:
    ConnectIoNeuronsCommand(IoNeuron &, IoNeuron &);

    void Do  (NNetModelWriterInterface &);
    void Undo(NNetModelWriterInterface &);

private:
    IoNeuron           & m_nobTarget;
    IoNeuron           & m_nobAnimated;
    unique_ptr<IoNeuron> m_upNobAnimated;
    unique_ptr<IoNeuron> m_upNobTarget;
    unique_ptr<Neuron>   m_upClosedNeuron {};
};
